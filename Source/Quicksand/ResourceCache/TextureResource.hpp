#ifndef QSE_TEXTURERESOURCE_HPP
#define QSE_TEXTURERESOURCE_HPP

#include "ResourceCache.hpp"
#include "../Graphics3D/Texture.hpp"

namespace Quicksand
{

	namespace KTX
	{

		struct HeaderData
		{
			unsigned char       identifier[12];
			unsigned int        endianness;
			unsigned int        gltype;
			unsigned int        gltypesize;
			unsigned int        glformat;
			unsigned int        glinternalformat;
			unsigned int        glbaseinternalformat;
			unsigned int        pixelwidth;
			unsigned int        pixelheight;
			unsigned int        pixeldepth;
			unsigned int        arrayelements;
			unsigned int        faces;
			unsigned int        miplevels;
			unsigned int        keypairbytes;
		};

		union KeyValuePair
		{
			unsigned int        size;
			unsigned char       rawbytes[4];
		};
	}

	class CTextureResourceExtraData : IResourceExtraData
	{

		CGLTexture m_Texture;

	public:

		void CreateTexture(void) { m_Texture.Create(); }

		GLuint GetTextureId(void) { return m_Texture.GetId(); }

		virtual std::string VToString(void);
	};

	class CTextureResourceLoader : public IResourceLoader
	{
	public:
		virtual bool VUseRawFile() { return false; }
		virtual bool VDiscardRawBufferAfterLoad() { return true; }
		virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) { return rawSize; }
		virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<CResHandle> handle);
		virtual std::string VGetPattern() { return "*.ktx"; }
	};

	//TODO:
	//create system that can be loaded into memory, but not into GPU memory
	//until needed/specified
	//this could be implimented in a CGLResourceCache class this would be responsible for
	//any memory that goes into the GPU
}


#endif