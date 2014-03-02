#ifndef QSE_ZIPRESOURCE_HPP
#define QSE_ZIPRESOURCE_HPP

namespace Quicksand
{

	typedef std::map<std::string, int> ZipContentsMap;

	class CZipFile
	{
	public:
		CZipFile() { m_nEntries = 0; m_pFile = NULL; m_pDirData = NULL; }
		virtual ~CZipFile() { End(); fclose( m_pFile ); }

		bool Init( const std::wstring &resFileName );
		void End();

		int GetNumFiles()const { return m_nEntries; }
		std::string GetFilename( int i ) const;
		int GetFileLen( int i ) const;
		bool ReadFile( int i, void *pBuf );

		// Added to show multi-threaded decompression
		bool ReadLargeFile( int i, void *pBuf, void( *progressCallback )(int, bool &) );

		int Find( const std::string &path ) const;

		ZipContentsMap m_ZipContentsMap;

	private:
		struct TZipDirHeader;
		struct TZipDirFileHeader;
		struct TZipLocalHeader;

		FILE *m_pFile;		// Zip file
		char *m_pDirData;	// Raw data buffer.
		int  m_nEntries;	// Number of entries.

		// CPointers to the dir entries in pDirData.
		const TZipDirFileHeader **m_papDir;
	};

	class CZipResourceFile : public IResourceFile
	{
		CZipFile *m_pZipFile;
		std::wstring m_ResFileName;

	public:
		CZipResourceFile( const std::wstring resFileName ) { m_pZipFile = NULL; m_ResFileName = resFileName; }
		virtual ~CZipResourceFile( );

		virtual bool VOpen();
		virtual int VGetRawResourceSize( const CResource &r );
		virtual int VGetRawResource( const CResource &r, char *buffer );
		virtual int VGetNumResources() const;
		virtual std::string VGetResourceName( int num ) const;
		virtual bool VIsUsingDevelopmentDirectories( void ) const { return false; }
	};

	//TODO: make a development zip file class Cso we can work nicely with the future editor

}

#endif