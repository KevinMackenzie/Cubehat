#ifndef QSE_FREEROMECACHE_HPP
#define QSE_FREEROMECACHE_HPP

#include "ResourceCache.hpp"

namespace Quicksand
{

	//this is a resource chache responsible for loading
	//necisary resources from the disk when approaching the 
	//area.  There will be different sectors of the map called
	// "chunk"s and they will fit together like a puzzel.  None
	// of them have a predefined size or shape, but the creation of chunks
	// will be in the world editor.  


	class CFreeRoamResCache : public CResCache
	{
		friend CResHandle;

		//the chunk has to be one Km or less away to be loaded
		unsigned int m_ChunkLoadRadius;

	public:
		CFreeRoamResCache(void);
		~CFreeRoamResCache(void);

		//TODO: once game logic is done, register a process that processes every 1-2 seconds for updating any necisary resource loading
		//      for the chunks that are not loaded

		void UpdateChunks(void);
		

	};


}


#endif