#ifndef QSE_RESOURCECACHE_HPP
#define QSE_RESOURCECACHE_HPP

#include "../Utilities/Interfaces.hpp"
#include <cctype>


namespace Quicksand
{
	using std::shared_ptr;
	using std::weak_ptr;

	class CResCache;

	//useful for getting more information about the resource
	class IResourceExtraData
	{
	public:
		virtual std::string VToString( ) = 0;
	};


	class CResource
	{
	public:
		std::string m_Name;
		CResource( const std::string &name )
		{
			m_Name = name;
			std::transform( m_Name.begin( ), m_Name.end( ), m_Name.begin( ), (int( *)(int)) std::tolower );
		}
	};

	class CResHandle
	{
		friend class CResCache;
		friend class CFreeRomeResCache;

	protected:
		CResource m_Resource;
		char* m_Buffer;
		unsigned int m_Size;
		shared_ptr<IResourceExtraData> m_Extra;
		CResCache *m_pResCache;

	public:
		CResHandle( CResource& resource, char* buffer, unsigned int size, CResCache* pResCache );
		virtual ~CResHandle( void );

		unsigned int Size( void ) const { return m_Size; }
		char *Buffer( void ) const { return m_Buffer; }
		char *WritibleBuffer( void ) { return m_Buffer; }
		shared_ptr<IResourceExtraData> GetExtra() { return m_Extra; }
		void SetExtra( shared_ptr<IResourceExtraData> extra ) { m_Extra = extra; }
	};





	//a simple resource cache

	//useful typedef's
	typedef std::list<shared_ptr<CResHandle> >				ResHandleList;
	typedef std::map<std::string, shared_ptr<CResHandle> >	ResHandleMap;
	typedef std::list<shared_ptr<IResourceLoader> >			ResourceLoaders;


	class CResCache
	{
		friend CResHandle;

	protected:
		ResHandleList m_LRU;// a list of the LRU (least recently used) resources
		ResHandleMap m_Resources;//stl map to make accessing faster
		ResourceLoaders m_ResourceLoaders;

		IResourceFile *m_pFile;	// object that impliments IResourceFile ( this is the file it loads e.g. '.zip')

		unsigned int m_CacheSize;	//total memory size
		unsigned int m_Allocated;	//total memory used(allocated)

		shared_ptr<CResHandle> Find( CResource &r );
		shared_ptr<CResHandle> Load( CResource &r );
		void Free( shared_ptr<CResHandle> gonner );
		void Update( shared_ptr<CResHandle> handle );

		bool MakeRoom( unsigned int size );
		char* Allocate( unsigned int size );
		void FreeOneResource( );
		void MemoryHasBeenFreed( unsigned int size );

	public:
		CResCache( const unsigned int sizeInMb, IResourceFile* resFile );
		~CResCache( );

		bool Init( );
		void RegisterLoader( shared_ptr<IResourceLoader> loader );

		shared_ptr <CResHandle> GetHandle( CResource *r );
		int Preload( const std::string pattern, void( *progressCallback )(int, bool&) );
		void Flush( void );
	};



	//a default handler to extract data as is and requires no processing at all and is directly loaded into data
	class CDefaultResourceLoader : public IResourceLoader
	{
	public:
		virtual bool VUseRawFile( ) { return true; }
		virtual bool VDiscardRawBufferAfterLoad( ) { return true; }
		virtual unsigned int VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize ) { return rawSize; }
		virtual bool VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<CResHandle> handle ) { return true; }
		virtual std::string VGetPattern( ) { return "*"; }

	};







}

#endif