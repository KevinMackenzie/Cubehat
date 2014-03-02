#include "stdafx.h"
#include "Logger.hpp"

#include "../Multicore/CriticalSection.hpp"
#include "../Utilities/String.hpp"


using std::string;
using namespace Quicksand;

#pragma region Constants, Statics, and Globals
//------------------------------------------------------------------------------------------------------------------------------------
// Some constants
//------------------------------------------------------------------------------------------------------------------------------------

// the log filename
static const char* ERRORLOG_FILENAME = "error.log";

// default display flags
#ifdef _DEBUG
const unsigned char ERRORFLAG_DEFAULT = LOGFLAG_WRITE_TO_DEBUGGER;
const unsigned char WARNINGFLAG_DEFAULT = LOGFLAG_WRITE_TO_DEBUGGER;
const unsigned char LOGFLAG_DEFAULT = LOGFLAG_WRITE_TO_DEBUGGER;
#elif NDEBUG
const unsigned char ERRORFLAG_DEFAULT = (LOGFLAG_WRITE_TO_DEBUGGER | LOGFLAG_WRITE_TO_LOG_FILE);
const unsigned char WARNINGFLAG_DEFAULT = (LOGFLAG_WRITE_TO_DEBUGGER | LOGFLAG_WRITE_TO_LOG_FILE);
const unsigned char LOGFLAG_DEFAULT = (LOGFLAG_WRITE_TO_DEBUGGER | LOGFLAG_WRITE_TO_LOG_FILE);
#else
const unsigned char ERRORFLAG_DEFAULT = 0;
const unsigned char WARNINGFLAG_DEFAULT = 0;
const unsigned char LOGFLAG_DEFAULT = 0;
#endif

// CSingleton
class CLogMgr;
static CLogMgr* s_pLogMgr = NULL;
#pragma endregion

#pragma region CLogMgr class
//------------------------------------------------------------------------------------------------------------------------------------
// CLogMgr class
//------------------------------------------------------------------------------------------------------------------------------------
class CLogMgr
{
public:
	enum ErrorDialogResult
	{
		LOGMGR_ERROR_ABORT,
		LOGMGR_ERROR_RETRY,
		LOGMGR_ERROR_IGNORE
	};

	typedef std::map<string, unsigned char> Tags;
	typedef std::list<NLogger::CErrorMessenger*> ErrorMessengerList;

	Tags m_tags;
	ErrorMessengerList m_errorMessengers;

	// thread safety
	CCriticalSection m_tagCriticalSection;
	CCriticalSection m_messengerCriticalSection;

public:
	// construction
	CLogMgr( void );
	~CLogMgr( void );
	void Init( const char* loggingConfigFilename );

	// logs
	void Log( const string& tag, const string& message, const char* funcName, const char* sourceFile, unsigned int lineNum );
	void SetDisplayFlags( const std::string& tag, unsigned char flags );

	// error messengers
	void AddErrorMessenger( NLogger::CErrorMessenger* pMessenger );
	CLogMgr::ErrorDialogResult Error( const std::string& errorMessage, bool isFatal, const char* funcName, const char* sourceFile, unsigned int lineNum );

private:
	// log helpers
	void OutputFinalBufferToLogs( const string& finalBuffer, unsigned char flags );
	void WriteToLogFile( const string& data );
	void GetOutputBuffer( std::string& outOutputBuffer, const string& tag, const string& message, const char* funcName, const char* sourceFile, unsigned int lineNum );
};
#pragma endregion

#pragma region CLogMgr class Cdefinition
//------------------------------------------------------------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------------------------------------------------------------
CLogMgr::CLogMgr( void )
{
	// set up the default log tags
	SetDisplayFlags( "ERROR", ERRORFLAG_DEFAULT );
	SetDisplayFlags( "WARNING", WARNINGFLAG_DEFAULT );
	SetDisplayFlags( "INFO", LOGFLAG_DEFAULT );
}


//------------------------------------------------------------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------------------------------------------------------------
CLogMgr::~CLogMgr( void )
{
	m_messengerCriticalSection.Lock( );
	for (auto it = m_errorMessengers.begin( ); it != m_errorMessengers.end( ); ++it)
	{
		NLogger::CErrorMessenger* pMessenger = (*it);
		delete pMessenger;
	}
	m_errorMessengers.clear( );
	m_messengerCriticalSection.Unlock( );
}


//------------------------------------------------------------------------------------------------------------------------------------
// Initializes the NLogger.
//------------------------------------------------------------------------------------------------------------------------------------
void CLogMgr::Init( const char* loggingConfigFilename )
{
	if (loggingConfigFilename)
	{
		TiXmlDocument loggingConfigFile( loggingConfigFilename );
		if (loggingConfigFile.LoadFile( ))
		{
			TiXmlElement* pRoot = loggingConfigFile.RootElement( );
			if (!pRoot)
				return;

			// Loop through each child element and load the component
			for (TiXmlElement* pNode = pRoot->FirstChildElement( ); pNode; pNode = pNode->NextSiblingElement( ))
			{
				unsigned char flags = 0;
				std::string tag( pNode->Attribute( "tag" ) );

				int debugger = 0;
				pNode->Attribute( "debugger", &debugger );
				if (debugger)
					flags |= LOGFLAG_WRITE_TO_DEBUGGER;

				int logfile = 0;
				pNode->Attribute( "file", &logfile );
				if (logfile)
					flags |= LOGFLAG_WRITE_TO_LOG_FILE;

				SetDisplayFlags( tag, flags );
			}
		}
	}
}


//------------------------------------------------------------------------------------------------------------------------------------
// This function builds up the log string and outputs it to various places based on the display flags (m_displayFlags).
//------------------------------------------------------------------------------------------------------------------------------------
void CLogMgr::Log( const string& tag, const string& message, const char* funcName, const char* sourceFile, unsigned int lineNum )
{
	m_tagCriticalSection.Lock( );
	Tags::iterator findIt = m_tags.find( tag );
	if (findIt != m_tags.end( ))
	{
		m_tagCriticalSection.Unlock( );

		string buffer;
		GetOutputBuffer( buffer, tag, message, funcName, sourceFile, lineNum );
		OutputFinalBufferToLogs( buffer, findIt->second );
	}
	else
	{
		// Critical section is exited in the if statement above, so we need to exit it here if that didn't 
		// get executed.
		m_tagCriticalSection.Unlock( );
	}
}  // end CLogMgr::Log()


//------------------------------------------------------------------------------------------------------------------------------------
// Sets one or more display flags
//------------------------------------------------------------------------------------------------------------------------------------
void CLogMgr::SetDisplayFlags( const std::string& tag, unsigned char flags )
{
	m_tagCriticalSection.Lock( );
	if (flags != 0)
	{
		Tags::iterator findIt = m_tags.find( tag );
		if (findIt == m_tags.end( ))
			m_tags.insert( std::make_pair( tag, flags ) );
		else
			findIt->second = flags;
	}
	else
	{
		m_tags.erase( tag );
	}
	m_tagCriticalSection.Unlock( );
}


//------------------------------------------------------------------------------------------------------------------------------------
// Adds an error messenger to the list
//------------------------------------------------------------------------------------------------------------------------------------
void CLogMgr::AddErrorMessenger( NLogger::CErrorMessenger* pMessenger )
{
	m_messengerCriticalSection.Lock( );
	m_errorMessengers.push_back( pMessenger );
	m_messengerCriticalSection.Unlock( );
}


//------------------------------------------------------------------------------------------------------------------------------------
// Helper for CErrorMessenger.
//------------------------------------------------------------------------------------------------------------------------------------
CLogMgr::ErrorDialogResult CLogMgr::Error( const std::string& errorMessage, bool isFatal, const char* funcName, const char* sourceFile, unsigned int lineNum )
{
	string tag = ((isFatal) ? ("FATAL") : ("ERROR"));

	// buffer for our final output string
	string buffer;
	GetOutputBuffer( buffer, tag, errorMessage, funcName, sourceFile, lineNum );

	// write the final buffer to all the various logs
	m_tagCriticalSection.Lock( );
	Tags::iterator findIt = m_tags.find( tag );
	if (findIt != m_tags.end( ))
		OutputFinalBufferToLogs( buffer, findIt->second );
	m_tagCriticalSection.Unlock( );

	// show the dialog box
	int result = ::MessageBoxA( NULL, buffer.c_str( ), tag.c_str( ), MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_DEFBUTTON3 );

	// act upon the choice
	switch (result)
	{
	case IDIGNORE: return CLogMgr::LOGMGR_ERROR_IGNORE;
	case IDABORT: __debugbreak( ); return CLogMgr::LOGMGR_ERROR_RETRY;  // assembly language instruction to break into the debugger
	case IDRETRY:	return CLogMgr::LOGMGR_ERROR_RETRY;
	default:       return CLogMgr::LOGMGR_ERROR_RETRY;
	}
}


//------------------------------------------------------------------------------------------------------------------------------------
// This is a helper function that checks all the display flags and outputs the passed in finalBuffer to the appropriate places.
// 
// IMPORTANT: The two places this function is called from wrap the code in the tag critical section (m_pTagCriticalSection), 
// so that makes this call thread safe.  If you call this from anywhere else, make sure you wrap it in that critical section.
//------------------------------------------------------------------------------------------------------------------------------------
void CLogMgr::OutputFinalBufferToLogs( const string& finalBuffer, unsigned char flags )
{
	// Write the log to each display based on the display flags
	if ((flags & LOGFLAG_WRITE_TO_LOG_FILE) > 0)  // log file
		WriteToLogFile( finalBuffer );
	if ((flags & LOGFLAG_WRITE_TO_DEBUGGER) > 0)  // debugger output window
		::OutputDebugStringA( finalBuffer.c_str( ) );
}


//------------------------------------------------------------------------------------------------------------------------------------
// This is a helper function that writes the data string to the log file.
//------------------------------------------------------------------------------------------------------------------------------------
void CLogMgr::WriteToLogFile( const string& data )
{
	FILE* pLogFile = NULL;
	fopen_s( &pLogFile, ERRORLOG_FILENAME, "a+" );
	if (!pLogFile)
		return;  // can't write to the log file for some reason

	fprintf_s( pLogFile, data.c_str( ) );

	fclose( pLogFile );
}

//------------------------------------------------------------------------------------------------------------------------------------
// Fills outOutputBuffer with the find error string.
//------------------------------------------------------------------------------------------------------------------------------------
void CLogMgr::GetOutputBuffer( std::string& outOutputBuffer, const string& tag, const string& message, const char* funcName, const char* sourceFile, unsigned int lineNum )
{
	if (!tag.empty( ))
		outOutputBuffer = "[" + tag + "] " + message;
	else
		outOutputBuffer = message;

	if (funcName != NULL)
	{
		outOutputBuffer += "\nFunction: ";
		outOutputBuffer += funcName;
	}
	if (sourceFile != NULL)
	{
		outOutputBuffer += "\n";
		outOutputBuffer += sourceFile;
	}
	if (lineNum != 0)
	{
		outOutputBuffer += "\nLine: ";
		char lineNumBuffer[11];
		memset( lineNumBuffer, 0, sizeof(char) );
		outOutputBuffer += _itoa( lineNum, lineNumBuffer, 10 );
	}

	outOutputBuffer += "\n";
}

#pragma endregion

#pragma region CErrorMessenger definition
//-----------------------------------------------------------------------------------------------------------------------
// CErrorMessenger
//-----------------------------------------------------------------------------------------------------------------------
NLogger::CErrorMessenger::CErrorMessenger( void )
{
	s_pLogMgr->AddErrorMessenger( this );
	m_enabled = true;
}

void NLogger::CErrorMessenger::Show( const std::string& errorMessage, bool isFatal, const char* funcName, const char* sourceFile, unsigned int lineNum )
{
	if (m_enabled)
	{
		if (s_pLogMgr->Error( errorMessage, isFatal, funcName, sourceFile, lineNum ) == CLogMgr::LOGMGR_ERROR_IGNORE)
			m_enabled = false;
	}
}
#pragma endregion

#pragma region CPlane Interface
//-----------------------------------------------------------------------------------------------------------------------
// CPlane Interface
//-----------------------------------------------------------------------------------------------------------------------


void NLogger::Init( const char* loggingConfigFilename )
{
	if (!s_pLogMgr)
	{
		s_pLogMgr = QSE_NEW CLogMgr;
		s_pLogMgr->Init( loggingConfigFilename );
	}
}

void NLogger::Destroy( void )
{
	delete s_pLogMgr;
	s_pLogMgr = NULL;
}

void NLogger::Log( const string& tag, const string& message, const char* funcName, const char* sourceFile, unsigned int lineNum )
{
	QSE_ASSERT( s_pLogMgr );
	s_pLogMgr->Log( tag, message, funcName, sourceFile, lineNum );
}

void NLogger::SetDisplayFlags( const std::string& tag, unsigned char flags )
{
	QSE_ASSERT( s_pLogMgr );
	s_pLogMgr->SetDisplayFlags( tag, flags );
}

#pragma endregion
