#include "stdafx.h"
#include "ResourceCache.hpp"
#include "../Utilities/String.hpp"

namespace Quicksand
{


	ResHandle::ResHandle(
		Resource& resource, char* buffer, unsigned int size, ResCache* pResCache ) :
		m_Resource( resource )
	{
		m_Buffer = buffer;
		m_Size = size;
		m_Extra = 0;
		m_pResCache = pResCache;
	}


	ResHandle::~ResHandle( void )
	{
		SAFE_DELETE_ARRAY( m_Buffer );
		m_pResCache->MemoryHasBeenFreed( m_Size );
	}

	ResCache::ResCache( const unsigned int sizeInMb, IResourceFile* resFile )
	{
		m_CacheSize = sizeInMb * 1024 * 1024;
		m_pFile = resFile;
		m_Allocated = 0;
	}

	shared_ptr<ResHandle> ResCache::Load( Resource &r )
	{
		shared_ptr < IResourceLoader> loader;
		shared_ptr <ResHandle> handle;

		for (ResourceLoaders::iterator it = m_ResourceLoaders.begin(); it != m_ResourceLoaders.end(); ++it)
		{
			shared_ptr<IResourceLoader> testLoader = *it;
			if (WildcardMatch( testLoader->VGetPattern( ).c_str( ), r.m_Name.c_str( ) ))
			{
				loader = testLoader;
				break;
			}
		}

		if (!loader)
		{
			assert( loader && _T( "Default resource loader not found!" ) );
			return handle;
		};

		const Resource& iR = r;

		unsigned int rawSize = m_pFile->VGetRawResourceSize( iR );

		char *rawBuffer = loader->VUseRawFile() ? Allocate( rawSize ) : QSE_NEW char[rawSize];

		if (rawBuffer == NULL)
		{
			//resource cache ran out of memeory
			return shared_ptr<ResHandle>();
		}

		m_pFile->VGetRawResource( iR, rawBuffer );
		char* buffer = NULL;
		unsigned int size = 0;

		if (loader->VUseRawFile())
		{
			buffer = rawBuffer;
			handle = shared_ptr<ResHandle>( QSE_NEW ResHandle( r, buffer, rawSize, this ) );
		}
		else
		{
			size = loader->VGetLoadedResourceSize( rawBuffer, rawSize );
			buffer = Allocate( size );
			if (rawBuffer == NULL || buffer == NULL)
			{
				//resource cach out of memory
				return shared_ptr<ResHandle>();
			}
			handle = shared_ptr<ResHandle>( QSE_NEW ResHandle( r, buffer, size, this ) );
			bool success = loader->VLoadResource( rawBuffer, rawSize, handle );
			SAFE_DELETE_ARRAY( rawBuffer );

			if (!success)
			{
				//resource cache out of memory
				return shared_ptr<ResHandle>();

			}
		}

		if (handle)
		{
			m_LRU.push_front( handle );
			m_Resources[r.m_Name] = handle;

		}

		assert( loader && _T( "Default resource loader not fount!" ) );
		return handle; //resource cache out of memory
	}


	char* ResCache::Allocate( unsigned int size )
	{
		if (!MakeRoom( size ))
			return NULL;

		char* mem = QSE_NEW char[size];
		if (mem)
			m_Allocated += size;

		return mem;
	}

	bool ResCache::MakeRoom( unsigned int size )
	{
		if (size > m_CacheSize)
			return false;

		//return null if there is no possible way to make memory
		while (size > (m_CacheSize - m_Allocated))
		{
			//the cache is empty, and there's still not enough room
			if (m_LRU.empty())
				return false;

			FreeOneResource();
		}

		return true;
	}

	void ResCache::FreeOneResource()
	{
		ResHandleList::iterator gonner = m_LRU.end();
		gonner--;

		shared_ptr<ResHandle> handle = *gonner;

		m_LRU.pop_back();
		m_Resources.erase( handle->m_Resource.m_Name );
	}


	shared_ptr<ResHandle> ResCache::Find( Resource& res )
	{
		ResHandleMap::iterator it = m_Resources.find( res.m_Name );
		if (it == m_Resources.end( ))
			return shared_ptr<ResHandle>( );

		return it->second;
	}


	bool ResCache::Init( )
	{
		bool retValue = false;
		if (m_pFile->VOpen( ))
		{
			RegisterLoader( shared_ptr<IResourceLoader>( QSE_NEW DefaultResourceLoader( ) ) );
			retValue = true;
		}
		return retValue;
	}

	void ResCache::RegisterLoader( shared_ptr<IResourceLoader> loader )
	{
		m_ResourceLoaders.push_front( loader );
	}

	shared_ptr <ResHandle> ResCache::GetHandle( Resource *r )
	{
		shared_ptr<ResHandle> handle( Find( *r ) );
		if (handle == NULL)
		{
			handle = Load( *r );
			QSE_ASSERT( handle );
		}
		else
		{
			Update( handle );
		}
		return handle;
	}

	int ResCache::Preload( const std::string pattern, void( *progressCallback )(int, bool&) )
	{
		if (m_pFile == NULL)
			return 0;

		int numFiles = m_pFile->VGetNumResources( );
		int loaded = 0;
		bool cancel = false;
		for (int i = 0; i<numFiles; ++i)
		{
			Resource resource( m_pFile->VGetResourceName( i ) );

			if (WildcardMatch( pattern.c_str( ), resource.m_Name.c_str( ) ))
			{
				shared_ptr<ResHandle> handle = g_pApp->m_pResCache->GetHandle( &resource );
				++loaded;
			}

			if (progressCallback != NULL)
			{
				progressCallback( i * 100 / numFiles, cancel );
			}
		}
		return loaded;
	}

	void ResCache::Flush( void )
	{
		while (!m_LRU.empty())
		{
			shared_ptr<ResHandle> handle = *(m_LRU.begin());
			Free( handle );
			m_LRU.pop_front();
		}
	}


	void ResCache::MemoryHasBeenFreed( unsigned int size )
	{
		m_Allocated -= size;
	}

	void ResCache::Free( shared_ptr<ResHandle> gonner )
	{
		m_LRU.remove( gonner );
		m_Resources.erase( gonner->m_Resource.m_Name );
		// Note - the resource might still be in use by something,
		// so the cache can't actually count the memory freed until the
		// ResHandle pointing to it is destroyed.

		//m_allocated -= gonner->m_resource.m_size;
		//delete gonner;
	}

	void ResCache::Update( shared_ptr<ResHandle> handle )
	{
		m_LRU.remove( handle );
		m_LRU.push_front( handle );
	}
}