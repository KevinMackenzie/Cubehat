#include "stdafx.h"
#include "ResourceCache.hpp"
#include "../Utilities/String.hpp"

using namespace Quicksand;

ResCache::ResCache( const unsigned int sizeInMb, IResourceFile* resFile )
{
	m_CacheSize = sizeInMb * 1024 * 1024;
	m_pFile = resFile;
	m_Allocated = 0;
}

shared_ptr<ResHandle> ResCache::Load( Resource* r )
{
	shared_ptr <IResourceLoader loader;
	shared_ptr <ResHandle> handle;

	for (ResourceLoaders::iterator it = m_ResourceLoaders.begin(); it != m_ResourceLoaders.end(); ++it)
	{
		shared_ptr<IResourceLoader> testLoader = *it;
		if (WildCardMatch( testLoader->VGetPattern().c_str(), r->m_Name.c_str() ))
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

	unsigned int rawSize = m_pFile->VGetRawResourceSize( *r );

	char *rawBuffer = loader->VUseRawFile() ? Allocate( rawSize ) : QSE_NEW char[rawSize];

	if (rawBuffer == NULL)
	{
		//resource cache ran out of memeory
		return shared_ptr<ResHandle>();
	}

	m_pFile->VGetRawResource( *r, rawBuffer );
	char* buffer = NULL;
	unsigned int size = 0;

	if (loader->VUseRawFile())
	{
		buffer = rawBuffer;
		handle = shared_ptr<ResHandle>( QSE_NEW ResHandle( *r, buffer, rawSize, this ) );
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
		handle = shared_ptr<ResHandle>( QSE_NEW ResHandle( *r, buffer, size, this ) );
		bool success = loader->VLoaderResource( rawBuffer, rawSize, handle );
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
		m_Resources[r->m_Name] = handle;

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