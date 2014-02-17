#include "stdafx.h"
#include "../ResourceCache/ResourceCache.hpp"
#include "../ResourceCache/XmlResource.hpp"
#include "../ResourceCache/ZipResource.hpp"
#include <shlobj.h>
#include <direct.h>
#include <sstream>


//TODO: impliment all of this to resolve errors, but i will not delete any of this code, maybe comment it out though



//link the necisary libraries

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "glew32s.lib")

#pragma comment(lib, "libogg_static.lib")
#pragma comment(lib, "libvorbis_static.lib")
#pragma comment(lib, "libvorbisfile_static.lib")

#pragma comment(lib, "tinyxml.lib")

#pragma comment(lib, "luaplus51-1201.lib")
#pragma comment(lib, "zdll.lib")
/*
#if defined(_M_IX86)
#if defined(_DEBUG)
#pragma comment(lib, "bulletcollision_debug.lib")
#pragma comment(lib, "bulletdynamics_debug.lib")
#pragma comment(lib, "linearmath_debug.lib")
#else
#pragma comment(lib, "bulletcollision.lib")
#pragma comment(lib, "bulletdynamics.lib")
#pragma comment(lib, "linearmath.lib")
#endif
#elif defined(_M_X64)
#if defined(_DEBUG)
#pragma comment(lib, "bulletcollision_x64__debug.lib")
#pragma comment(lib, "bulletdynamics_x64__debug.lib")
#pragma comment(lib, "linearmath_x64_debug.lib")
#else
#pragma comment(lib, "bulletcollision_x64.lib")
#pragma comment(lib, "bulletdynamics_x64.lib")
#pragma comment(lib, "linearmath_x64.lib")
#endif
#else
#error Preprocessor defines can't figure out which Bullet library to use.
#endif
*/


namespace Quicksand
{

	///////////////////////////////////////////
	//
	//
	//	The one and only app instance
	//
	//
	//
	QuicksandApp* g_pApp;

	//
	// CheckStorage
	//
	bool CheckStorage( const DWORDLONG diskSpaceNeeded )
	{
		// Check for enough free disk space on the current disk.
		int const drive = _getdrive();
		struct _diskfree_t diskfree;

		_getdiskfree( drive, &diskfree );

		unsigned __int64 const neededClusters =
			diskSpaceNeeded / (diskfree.sectors_per_cluster*diskfree.bytes_per_sector);

		if (diskfree.avail_clusters < neededClusters)
		{
			// if you get here you don’t have enough disk space!
			QSE_ERROR( "CheckStorage Failure: Not enough physical storage." );
			return false;
		}
		return true;
	}

	//
	// CheckMemory
	//
	bool CheckMemory( const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded )
	{
		MEMORYSTATUSEX status;
		GlobalMemoryStatusEx( &status );
		if (status.ullTotalPhys < physicalRAMNeeded)
		{
			// you don’t have enough physical memory. Tell the player to go get a real 
			// computer and give this one to his mother. 
			QSE_ERROR( "CheckMemory Failure: Not enough physical memory." );
			return false;
		}

		// Check for enough free memory.
		if (status.ullAvailVirtual < virtualRAMNeeded)
		{
			// you don’t have enough virtual memory available. 
			// Tell the player to shut down the copy of Visual Studio running in the
			// background, or whatever seems to be sucking the memory dry.
			QSE_ERROR( "CheckMemory Failure: Not enough virtual memory." );
			return false;
		}

		char *buff = QSE_NEW char[(unsigned int)virtualRAMNeeded];
		if (buff)
			delete[] buff;
		else
		{
			// even though there is enough memory, it isn’t available in one 
			// block, which can be critical for games that manage their own memory
			QSE_ERROR( "CheckMemory Failure: Not enough contiguous available memory." );
			return false;
		}
		return true;
	}

	//
	// ReadCPUSpeed
	//
	DWORD ReadCPUSpeed()
	{
		DWORD BufSize = sizeof(DWORD);
		DWORD dwMHz = 0;
		DWORD type = REG_DWORD;
		HKEY hKey;

		// open the key where the proc speed is hidden:
		long lError = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
			L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
			0, KEY_READ, &hKey );

		if (lError == ERROR_SUCCESS)
		{
			// query the key:
			RegQueryValueEx( hKey, L"~MHz", NULL, &type, (LPBYTE)&dwMHz, &BufSize );
		}
		return dwMHz;
	}


	GameOptions::GameOptions()
	{
		// set all the options to decent default valu
		m_Level = "";
		m_Renderer = "Direct3D 9";
		m_runFullSpeed = false;
		m_soundEffectsVolume = 1.0f;
		m_musicVolume = 1.0f;
		m_expectedPlayers = 1;
		m_listenPort = -1;
		std::string m_gameHost = "MrMike-m1710";
		m_numAIs = 1;
		m_maxAIs = 4;
		m_maxPlayers = 4;
		m_ScreenSize = Point( 1024, 768 );
		m_useDevelopmentDirectories = false;

		m_pDoc = NULL;
	}


	void GameOptions::Init( const char* xmlFileName, LPWSTR lpCmdLine )
	{
		// read the XML file
		// if needed, override the XML file with options passed in on the command line.

		m_pDoc = new TiXmlDocument( xmlFileName );
		if (m_pDoc && m_pDoc->LoadFile())
		{
			TiXmlElement *pRoot = m_pDoc->RootElement();
			if (!pRoot)
				return;

			// Loop through each child element and load the component
			TiXmlElement* pNode = NULL;
			pNode = pRoot->FirstChildElement( "Graphics" );
			if (pNode)
			{
				std::string attribute;
				attribute = pNode->Attribute( "renderer" );
				if (attribute != "Direct3D 9" && attribute != "Direct3D 11")
				{
					QSE_ASSERT( 0 && "Bad Renderer setting in Graphics options." );
				}
				else
				{
					m_Renderer = attribute;
				}

				if (pNode->Attribute( "width" ))
				{
					m_ScreenSize.x = atoi( pNode->Attribute( "width" ) );
					if (m_ScreenSize.x < 800) m_ScreenSize.x = 800;
				}

				if (pNode->Attribute( "height" ))
				{
					m_ScreenSize.y = atoi( pNode->Attribute( "height" ) );
					if (m_ScreenSize.y < 600) m_ScreenSize.y = 600;
				}

				if (pNode->Attribute( "runfullspeed" ))
				{
					attribute = pNode->Attribute( "runfullspeed" );
					m_runFullSpeed = (attribute == "yes") ? true : false;
				}
			}

			pNode = pRoot->FirstChildElement( "Sound" );
			if (pNode)
			{
				m_musicVolume = atoi( pNode->Attribute( "musicVolume" ) ) / 100.0f;
				m_soundEffectsVolume = atoi( pNode->Attribute( "sfxVolume" ) ) / 100.0f;
			}

			pNode = pRoot->FirstChildElement( "Multiplayer" );
			if (pNode)
			{
				m_expectedPlayers = atoi( pNode->Attribute( "expectedPlayers" ) );
				m_numAIs = atoi( pNode->Attribute( "numAIs" ) );
				m_maxAIs = atoi( pNode->Attribute( "maxAIs" ) );
				m_maxPlayers = atoi( pNode->Attribute( "maxPlayers" ) );

				m_listenPort = atoi( pNode->Attribute( "listenPort" ) );
				m_gameHost = pNode->Attribute( "gameHost" );
			}

			pNode = pRoot->FirstChildElement( "ResCache" );
			if (pNode)
			{
				std::string attribute( pNode->Attribute( "useDevelopmentDirectories" ) );
				m_useDevelopmentDirectories = ((attribute == "yes") ? (true) : (false));
			}
		}
	}




	//
	// IsOnlyInstance							- Chapter 5, page 137
	//
	bool IsOnlyInstance( LPCTSTR gameTitle )
	{
		// Find the window.  If active, set and return false
		// Only one game instance may have this mutex at a time...

		HANDLE handle = CreateMutex( NULL, TRUE, gameTitle );

		// Does anyone else think 'ERROR_SUCCESS' is a bit of an oxymoron?
		if (GetLastError() != ERROR_SUCCESS)
		{
			HWND hWnd = FindWindow( gameTitle, NULL );
			if (hWnd)
			{
				// An instance of your game is already running.
				ShowWindow( hWnd, SW_SHOWNORMAL );
				SetFocus( hWnd );
				SetForegroundWindow( hWnd );
				SetActiveWindow( hWnd );
				return false;
			}
		}
		return true;
	}

	//
	// GetSaveGameDirectory - Chapter 5, page 146
	//
	const TCHAR *GetSaveGameDirectory( HWND hWnd, const TCHAR *gameAppDirectory )
	{
		HRESULT hr;
		static TCHAR m_SaveGameDirectory[MAX_PATH];
		TCHAR userDataPath[MAX_PATH];

		hr = SHGetSpecialFolderPath( hWnd, userDataPath, CSIDL_APPDATA, true );

		_tcscpy_s( m_SaveGameDirectory, userDataPath );
		_tcscat_s( m_SaveGameDirectory, _T( "\\" ) );
		_tcscat_s( m_SaveGameDirectory, gameAppDirectory );

		// Does our directory exist?
		if (0xffffffff == GetFileAttributes( m_SaveGameDirectory ))
		{
			if (SHCreateDirectoryEx( hWnd, m_SaveGameDirectory, NULL ) != ERROR_SUCCESS)
				return false;
		}

		_tcscat_s( m_SaveGameDirectory, _T( "\\" ) );

		return m_SaveGameDirectory;
	}


	QuicksandApp::QuicksandApp()
	{
		g_pApp = this;
		m_pGame = NULL;

		m_rcDesktop.bottom = m_rcDesktop.left = m_rcDesktop.right = m_rcDesktop.top = 0;
		m_ScreenSize = Point( 0, 0 );

		m_bIsRunning = false;
		m_bIsEditorRunning = false;

		//TODO: event manager
		//m_pEventManager = NULL;
		m_pResCache = NULL;

		//TODO: networked gameplay
		//m_pNetworkEventForwarder = NULL;
		//m_pBaseSocketManager = NULL;

		m_bQuitRequested = false;
		m_bQuitting = false;
		//m_HasModalDialog = 0;

		m_fTimeElapsed = 0.0f;
	}


	//===================================================================
	// Win32 Specific Stuff
	//
	// InitInstance - this checks system resources, creates your window, and launches the game
	//
	// preprocessor macro setting in the C++ options of the project provides the base macro
	// C preprocessor string concatenation takes care of the rest.
	//
	// QuicksandApp::InitInstance - Chapter 5, page 136
	//
	//===================================================================

	bool QuicksandApp::InitInstance( HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int ScreenWidth, int ScreenHeight )
	{
		// Check for existing instance of the same window
		// 
#ifndef _DEBUG
		// Note - it can be really useful to debug network code to have
		// more than one instance of the game up at one time - so
		// feel free to comment these lines in or out as you wish!
		if (!IsOnlyInstance( VGetGameTitle( ) ))
			return false;
#endif

		// We don't need a mouse cursor by default, let the game turn it on
		SetCursor( NULL );

		// Check for adequate machine resources.
		bool resourceCheck = false;
		while (!resourceCheck)
		{
			const DWORDLONG physicalRAM = 512 * MEGABYTE;
			const DWORDLONG virtualRAM = 1024 * MEGABYTE;
			const DWORDLONG diskSpace = 10 * MEGABYTE;
			if (!CheckStorage( diskSpace ))
				return false;

			const DWORD minCpuSpeed = 1300;			// 1.3Ghz
			DWORD thisCPU = ReadCPUSpeed();
			if (thisCPU < minCpuSpeed)
			{
				QSE_ERROR( "GetCPUSpeed reports CPU is too slow for this game." );
				return false;
			}

			resourceCheck = true;
		}

		m_hInstance = hInstance;

		// register all events
		RegisterEngineEvents();
		VRegisterGameEvents();

		//
		// Initialize the ResCache - Chapter 5, page 141
		//
		//    Note - this is a little different from the book. Here we have a speccial resource ZIP file class, DevelopmentResourceZipFile,
		//    that actually reads directly from the source asset files, rather than the ZIP file. This is MUCH better during development, since
		//    you don't want to have to rebuild the ZIP file every time you make a minor change to an asset.
		//
		//IResourceFile *zipFile = (m_bIsEditorRunning || m_Options.m_useDevelopmentDirectories) ?
		//	QSE_NEW DevelopmentResourceZipFile( L"Assets.zip", DevelopmentResourceZipFile::Editor ) :
		//	QSE_NEW ResourceZipFile( L"Assets.zip" );

		//NOTE: the zip file may not be named Assets.zip
		IResourceFile *zipFile = QSE_NEW ZipResourceFile( L"Assets.zip" );

		m_pResCache = QSE_NEW ResCache( 50, zipFile );

		if (!m_pResCache->Init())
		{
			QSE_ERROR( "Failed to initialize resource cache!  Are your paths set up correctly?" );
			return false;
		}

		//TODO: impliment these methods
		//extern shared_ptr<IResourceLoader> CreateWAVResourceLoader();
		//extern shared_ptr<IResourceLoader> CreateOGGResourceLoader();
		//extern shared_ptr<IResourceLoader> CreateDDSResourceLoader();
		//extern shared_ptr<IResourceLoader> CreateJPGResourceLoader();
		//extern shared_ptr<IResourceLoader> CreateXmlResourceLoader();
		//extern shared_ptr<IResourceLoader> CreateSdkMeshResourceLoader();
		//extern shared_ptr<IResourceLoader> CreateScriptResourceLoader();

		// Note - register these in order from least specific to most specific! They get pushed onto a list.
		// RegisterLoader is discussed in Chapter 5, page 142
		/*m_pResCache->RegisterLoader( CreateWAVResourceLoader() );
		m_pResCache->RegisterLoader( CreateOGGResourceLoader() );
		m_pResCache->RegisterLoader( CreateDDSResourceLoader() );
		m_pResCache->RegisterLoader( CreateJPGResourceLoader() );
		m_pResCache->RegisterLoader( CreateXmlResourceLoader() );
		m_pResCache->RegisterLoader( CreateSdkMeshResourceLoader() );
		m_pResCache->RegisterLoader( CreateScriptResourceLoader() );*/

		if (!LoadStrings( "English" ))
		{
			QSE_ERROR( "Failed to load strings" );
			return false;
		}

		//TODO: create lua state manager
		//if (!LuaStateManager::Create( ))
		//{
		//	QSE_ERROR( "Failed to initialize Lua" );
		//	return false;
		//}

		// Load the preinit file.  This is within braces to create a scope and destroy the resource once it's loaded.  We
		// don't need to do anything with it, we just need to load it.
		//{
		//	TODO: uncomment this once the script is implimentedx
		//	Resource resource( SCRIPT_PREINIT_FILE );
		//	shared_ptr<ResHandle> pResourceHandle = m_pResCache->GetHandle( &resource );  // this actually loads the XML file from the zip file
		//}

		//TODO: create script parser and exicuter
		// Register function exported from C++
		//ScriptExports::Register( );
		//ScriptProcess::RegisterScriptClass( );
		//BaseScriptComponent::RegisterScriptFunctions( );

		// The event manager should be created next so that subsystems can hook in as desired.
		// Discussed in Chapter 5, page 144
		//TODO: create event manager
		//m_pEventManager = QSE_NEW EventManager( "QuicksandApp Event Mgr", true );
		//if (!m_pEventManager)
		//{
		//	QSE_ERROR( "Failed to create EventManager." );
		//	return false;
		//}

		//set GLFW error callback before init
		glfwSetErrorCallback( QuicksandApp::ErrorCallbackProc );

		//GLFW creation
		int Success = glfwInit();

		if (Success == GL_FALSE)
			return false;

		m_ScreenSize = Point( ScreenWidth, ScreenHeight );

		//set window creation flags
		glfwDefaultWindowHints();

		if (hWnd == NULL)
		{
			m_pOpenGLWindow = glfwCreateWindow( m_ScreenSize.x, m_ScreenSize.y, VGetGameTitle(), m_Options.m_RunFullScreen ? glfwGetPrimaryMonitor() : NULL, NULL );
		}
		else
		{
			QSE_ERROR( "Window Already Created, Failing" );
		}

		if (!m_pOpenGLWindow)
		{
			return FALSE;
		}


		// initialize the directory location you can store save game files
		_tcscpy_s( m_SaveGameDirectory, GetSaveGameDirectory( glfwGetWin32Window( m_pOpenGLWindow ), VGetGameAppDirectory() ) );

		// DXUTCreateDevice - Chapter 5 - page 139
		m_ScreenSize = Point( ScreenWidth, ScreenHeight );

		//TODO: create renderer
		//m_Renderer->VSetBackgroundColor( 255, 20, 20, 200 );
		//m_Renderer->VOnRestore();


		// You usually must have an HWND to initialize your game views...
		//    VCreateGameAndView			- Chapter 5, page 145
		//TODO: create game and view function
		//m_pGame = VCreateGameAndView();
		//if (!m_pGame)
		//	return false;

		// now that all the major systems are initialized, preload resources 
		//    Preload calls are discussed in Chapter 5, page 148
		//m_pResCache->Preload( "*.ogg", NULL );
		//m_pResCache->Preload( "*.dds", NULL );
		//m_pResCache->Preload( "*.jpg", NULL );
		//m_pResCache->Preload( "*.obj", NULL );//TODO:


		m_bIsRunning = true;

		return TRUE;
	}

	bool QuicksandApp::VLoadGame( void )
	{
		// Read the game options and see what the current game
		// needs to be - all of the game graphics are initialized by now, too...
		//TODO: Create game logic
		//return m_pGame->VLoadGame( m_Options.m_Level.c_str( ) );
		//for now
		return false;
	}

	void QuicksandApp::RegisterEngineEvents( void )
	{
		//REGISTER_EVENT( EvtData_Environment_Loaded );
		//REGISTER_EVENT( EvtData_New_Actor );
		//REGISTER_EVENT( EvtData_Move_Actor );
		//REGISTER_EVENT( EvtData_Destroy_Actor );
		//REGISTER_EVENT( EvtData_Request_New_Actor );
		//REGISTER_EVENT( EvtData_Network_Player_Actor_Assignment );
	}

	//
	// QuicksandApp::LoadStrings										- Chapter 5, page 143
	//
	bool QuicksandApp::LoadStrings( std::string language )
	{
		std::string languageFile = "Strings\\";
		languageFile += language;
		languageFile += ".xml";

		TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( languageFile.c_str() );
		if (!pRoot)
		{
			QSE_ERROR( "Strings are missing." );
			return false;
		}

		// Loop through each child element and load the component
		for (TiXmlElement* pElem = pRoot->FirstChildElement(); pElem; pElem = pElem->NextSiblingElement())
		{
			const char *pKey = pElem->Attribute( "id" );
			const char *pText = pElem->Attribute( "value" );
			const char *pHotkey = pElem->Attribute( "hotkey" );
			if (pKey && pText)
			{
				wchar_t wideKey[64];
				wchar_t wideText[1024];
				AnsiToWideCch( wideKey, pKey, 64 );
				AnsiToWideCch( wideText, pText, 1024 );
				m_TextResource[std::wstring( wideKey )] = std::wstring( wideText );

				if (pHotkey)
				{
					m_Hotkeys[std::wstring( wideKey )] = MapCharToKeycode( *pHotkey );
				}
			}
		}
		return true;
	}

	UINT QuicksandApp::MapCharToKeycode( const char pHotKey )
	{
		if (pHotKey >= '0' && pHotKey <= '9')
			return 0x30 + pHotKey - '0';

		if (pHotKey >= 'A' && pHotKey <= 'Z')
			return 0x41 + pHotKey - 'A';

		QSE_ASSERT( 0 && "Platform specific hotkey is not defined" );
		return 0;
	}


	//----------------------------------------------------------
	// QuicksandApp::GetString								- Chapter 5, page 144
	//
	// creates a string from a string resource ID in the string table
	// stored in a special DLL, LANG.DLL, so game text strings
	// can be language independant
	//
	std::wstring QuicksandApp::GetString( std::wstring sID )
	{
		auto localizedString = m_TextResource.find( sID );
		if (localizedString == m_TextResource.end())
		{
			QSE_ASSERT( 0 && "String not found!" );
			return L"";
		}
		return localizedString->second;
	}



	///////////////////////////////////////////
	//	This manages the main loop and 
	//  handles windows messages
	//
	//
	void QuicksandApp::MainLoop( void )
	{
		// Begin the messages pump.
		bool bGotMsg;
		MSG msg;
		msg.message = WM_NULL;
		PeekMessage( &msg, nullptr, 0U, 0U, PM_NOREMOVE );

		while (WM_QUIT != msg.message)
		{
			// Use PeekMessage() so we can use idle time to render the scene. 
			bGotMsg = (PeekMessage( &msg, nullptr, 0U, 0U, PM_REMOVE ) != 0);

			if (bGotMsg)
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				// Render a frame during idle time (no messages are waiting)

				//update the last update time
				m_fTimeElapsed = (float)glfwGetTime() - m_fTimeElapsed;

				//call the callback functions

				//call the game even process method
				OnUpdateGame( (float)glfwGetTime(), m_fTimeElapsed );

				//now call the render method
				OnUpdateFrame( (float)glfwGetTime(), m_fTimeElapsed );

			}
		}
	}



	////////////////////////////////////////
	//	This does all necisary things to shutdown the opeGL and other contexts
	//
	//
	//	This will destroy various systems
	//
	//
	void QuicksandApp::ShutDown( void )
	{
		//for now
		glfwTerminate();
	}

	//a method that will be called to update the game data, and tick the game logic
	void QuicksandApp::OnUpdateGame( float fTime, float fEllapsedTime )
	{

	}

	//this is called specifically to update rendering and graphics data
	void QuicksandApp::OnUpdateFrame( float fTime, float fEllapsedTime )
	{

	}


	void QuicksandApp::VRegisterGameEvents( void )
	{

	}

	LRESULT QuicksandApp::OnPowerBroadcast( int event )
	{
		return 0;
	}

	LRESULT QuicksandApp::OnSysCommand( WPARAM wParam, LPARAM lParam )
	{
		return 0;
	}

	LRESULT QuicksandApp::OnClose( )
	{
		return 0;
	}

	LRESULT QuicksandApp::OnAltEnter( )
	{
		return 0;
	}

	void QuicksandApp::ErrorCallbackProc( int error, const char* errorMessage )
	{
		std::stringstream ss;

		ss << error;
		ss << ": ";
		ss << errorMessage;

		std::string str;

		ss >> str;

		QSE_ERROR( str );
	}


	//The entry point
	extern INT WINAPI QuicksandProc( HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPWSTR    lpCmdLine,
		int       nCmdShow )
	{
		// Set up checks for memory leaks.
		// Game Coding Complete reference - Chapter 21, page 834
		//
		int tmpDbgFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );

		// set this flag to keep memory blocks around
		tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;	       // this flag will cause intermittent pauses in your game and potientially cause it to run out of memory!

		// perform memory check for each alloc/dealloc
		tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;		   // remember this is VERY VERY SLOW!

		//_CRTDBG_LEAK_CHECK_DF is used at program initialization to force a 
		//   leak check just before program exit. This is important because
		//   some classes may dynamically allocate memory in globally constructed
		//   objects.
		tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;

		_CrtSetDbgFlag( tmpDbgFlag );

		//the first to initialize
		NLogger::Init( "logging.xml" );



		g_pApp->m_Options.Init( "PlayerOptions.xml", lpCmdLine );

		// Set the callback functions. 

		//Initialize

		// Show the cursor and clip it when in full Screen
		//DXUTSetCursorSettings( true, true );

		// Perform application initialization
		if (!g_pApp->InitInstance( hInstance, lpCmdLine, 0, g_pApp->m_Options.m_ScreenSize.x, g_pApp->m_Options.m_ScreenSize.y ))
		{
			return FALSE;
		}

		// Pass control to the sample framework for handling the message pump and 
		// dispatching render calls. The sample framework will call your FrameMove 
		// and FrameRender callback when there is idle time between handling window messages.

		g_pApp->MainLoop();
		g_pApp->ShutDown();

		//Destroy the logging system at the last possible moment
		NLogger::Destroy();

		return 0;
	}


	LRESULT CALLBACK QuicksandApp::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
	{
		// Always allow dialog resource manager calls to handle global messages
		// so GUI state is updated correctly
		//*pbNoFurtherProcessing = D3DRenderer::g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
		//if (*pbNoFurtherProcessing)
		//	return 0;

		LRESULT result = 0;

		switch (uMsg)
		{
		case WM_POWERBROADCAST:
		{
								  int event = (int)wParam;
								  result = g_pApp->OnPowerBroadcast( event );
								  break;
		}

		case WM_DISPLAYCHANGE:
		case WM_SYSCOMMAND:
		{
							  result = g_pApp->OnSysCommand( wParam, lParam );
							  if (result)
							  {
								  *pbNoFurtherProcessing = true;
							  }
							  break;
		}

		case WM_SYSKEYDOWN:
		{
							  if (wParam == VK_RETURN)
							  {
								  *pbNoFurtherProcessing = true;
								  return g_pApp->OnAltEnter();
							  }
							  return DefWindowProc( hWnd, uMsg, wParam, lParam );
		}


		case WM_CLOSE:
		{
						 if (g_pApp->m_bQuitting)
						 {
							 result = g_pApp->OnClose();
						 };
						 break;
		}


		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case MM_JOY1BUTTONDOWN:
		case MM_JOY1BUTTONUP:
		case MM_JOY1MOVE:
		case MM_JOY1ZMOVE:
		case MM_JOY2BUTTONDOWN:
		case MM_JOY2BUTTONUP:
		case MM_JOY2MOVE:
		case MM_JOY2ZMOVE:

		default:
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
		}

		return result;
	}

}