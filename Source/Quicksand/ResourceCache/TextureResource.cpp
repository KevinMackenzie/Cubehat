#include "stdafx.h"
#include <cstring>
#include "TextureResource.hpp"



namespace Quicksand
{
	namespace KTX
	{
		static const unsigned char identifier[] =
		{
			0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A
		};

		static const unsigned int Swap32(const unsigned int u32)
		{
			union
			{
				unsigned int u32;
				unsigned char u8[4];
			} a, b;

			a.u32 = u32;
			b.u8[0] = a.u8[3];
			b.u8[1] = a.u8[2];
			b.u8[2] = a.u8[1];
			b.u8[3] = a.u8[0];

			return b.u32;
		}

		static const unsigned short Swap16(const unsigned short u16)
		{
			union
			{
				unsigned short u16;
				unsigned char u8[2];
			} a, b;

			a.u16 = u16;
			b.u8[0] = a.u8[1];
			b.u8[1] = a.u8[0];

			return b.u16;
		}

		static unsigned int CalculateStride(const HeaderData& h, unsigned int width, unsigned int pad = 4)
		{
			unsigned int channels = 0;

			switch (h.glbaseinternalformat)
			{
			case GL_RED:    channels = 1;
				break;
			case GL_RG:     channels = 2;
				break;
			case GL_BGR:
			case GL_RGB:    channels = 3;
				break;
			case GL_BGRA:
			case GL_RGBA:   channels = 4;
				break;
			}

			unsigned int stride = h.gltypesize * channels * width;

			stride = (stride + (pad - 1)) & ~(pad - 1);

			return stride;
		}

		static unsigned int CalculateFaceSize(const HeaderData& h)
		{
			unsigned int stride = CalculateStride(h, h.pixelwidth);

			return stride * h.pixelheight;
		}
	}




	bool CTextureResourceLoader::VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<CResHandle> handle)
	{
		//start by filling the header
		KTX::HeaderData h;

		if (memcpy_s(&h, sizeof(h), rawBuffer, sizeof(h)) != 0)
			return false;

		if (h.endianness == 0x04030201)
		{
			// No swap needed
		}
		else if (h.endianness == 0x01020304)
		{
			// Swap needed
			h.endianness = KTX::Swap32(h.endianness);
			h.gltype = KTX::Swap32(h.gltype);
			h.gltypesize = KTX::Swap32(h.gltypesize);
			h.glformat = KTX::Swap32(h.glformat);
			h.glinternalformat = KTX::Swap32(h.glinternalformat);
			h.glbaseinternalformat = KTX::Swap32(h.glbaseinternalformat);
			h.pixelwidth = KTX::Swap32(h.pixelwidth);
			h.pixelheight = KTX::Swap32(h.pixelheight);
			h.pixeldepth = KTX::Swap32(h.pixeldepth);
			h.arrayelements = KTX::Swap32(h.arrayelements);
			h.faces = KTX::Swap32(h.faces);
			h.miplevels = KTX::Swap32(h.miplevels);
			h.keypairbytes = KTX::Swap32(h.keypairbytes);
		}
		else
		{
			return false;
		};

		GLuint target = GL_NONE;

		// Guess target (texture type)
		if (h.pixelheight == 0)
		{
			if (h.arrayelements == 0)
			{
				target = GL_TEXTURE_1D;
			}
			else
			{
				target = GL_TEXTURE_1D_ARRAY;
			}
		}
		else if (h.pixeldepth == 0)
		{
			if (h.arrayelements == 0)
			{
				if (h.faces == 0)
				{
					target = GL_TEXTURE_2D;
				}
				else
				{
					target = GL_TEXTURE_CUBE_MAP;
				}
			}
			else
			{
				if (h.faces == 0)
				{
					target = GL_TEXTURE_2D_ARRAY;
				}
				else
				{
					target = GL_TEXTURE_CUBE_MAP_ARRAY;
				}
			}
		}
		else
		{
			target = GL_TEXTURE_3D;
		}

		// Check for insanity...
		if (target == GL_NONE ||                                    // Couldn't figure out target
			(h.pixelwidth == 0) ||                                  // Texture has no width???
			(h.pixelheight == 0 && h.pixeldepth != 0))              // Texture has depth but no height???
		{
			return false;
		}


		//generate the texture

		shared_ptr<CTextureResourceExtraData> tex = shared_ptr<CTextureResourceExtraData>(QSE_NEW CTextureResourceExtraData);
		tex->CreateTexture();

		glBindTexture(target, tex->GetTextureId());

		unsigned int dataSize = rawSize - sizeof(KTX::HeaderData);

		unsigned char* data = new unsigned char[dataSize];
		memcpy_s(data, dataSize, rawBuffer + sizeof(KTX::HeaderData), dataSize);


		if (h.miplevels == 0)
		{
			h.miplevels = 1;
		}

		switch (target)
		{
		case GL_TEXTURE_1D:
			glTexStorage1D(GL_TEXTURE_1D, h.miplevels, h.glinternalformat, h.pixelwidth);
			glTexSubImage1D(GL_TEXTURE_1D, 0, 0, h.pixelwidth, h.glformat, h.glinternalformat, data);
			break;
		case GL_TEXTURE_2D:
			glTexStorage2D(GL_TEXTURE_2D, h.miplevels, h.glinternalformat, h.pixelwidth, h.pixelheight);
			{
				unsigned char * ptr = data;
				unsigned int height = h.pixelheight;
				unsigned int width = h.pixelwidth;
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				for (unsigned int i = 0; i < h.miplevels; i++)
				{
					glTexSubImage2D(GL_TEXTURE_2D, i, 0, 0, width, height, h.glformat, h.gltype, ptr);
					ptr += height * KTX::CalculateStride(h, width, 1);
					height >>= 1;
					width >>= 1;
					if (!height)
						height = 1;
					if (!width)
						width = 1;
				}
			}
			break;
		case GL_TEXTURE_3D:
			glTexStorage3D(GL_TEXTURE_3D, h.miplevels, h.glinternalformat, h.pixelwidth, h.pixelheight, h.pixeldepth);
			glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, h.pixelwidth, h.pixelheight, h.pixeldepth, h.glformat, h.gltype, data);
			break;
		case GL_TEXTURE_1D_ARRAY:
			glTexStorage2D(GL_TEXTURE_1D_ARRAY, h.miplevels, h.glinternalformat, h.pixelwidth, h.arrayelements);
			glTexSubImage2D(GL_TEXTURE_1D_ARRAY, 0, 0, 0, h.pixelwidth, h.arrayelements, h.glformat, h.gltype, data);
			break;
		case GL_TEXTURE_2D_ARRAY:
			glTexStorage3D(GL_TEXTURE_2D_ARRAY, h.miplevels, h.glinternalformat, h.pixelwidth, h.pixelheight, h.arrayelements);
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, h.pixelwidth, h.pixelheight, h.arrayelements, h.glformat, h.gltype, data);
			break;
		case GL_TEXTURE_CUBE_MAP:
			glTexStorage2D(GL_TEXTURE_CUBE_MAP, h.miplevels, h.glinternalformat, h.pixelwidth, h.pixelheight);
			// glTexSubImage3D(GL_TEXTURE_CUBE_MAP, 0, 0, 0, 0, h.pixelwidth, h.pixelheight, h.faces, h.glformat, h.gltype, data);
			{
				unsigned int face_size = KTX::CalculateFaceSize(h);
				for (unsigned int i = 0; i < h.faces; i++)
				{
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, h.pixelwidth, h.pixelheight, h.glformat, h.gltype, data + face_size * i);
				}
			}
			break;
		case GL_TEXTURE_CUBE_MAP_ARRAY:
			glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, h.miplevels, h.glinternalformat, h.pixelwidth, h.pixelheight, h.arrayelements);
			glTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, 0, 0, 0, h.pixelwidth, h.pixelheight, h.faces * h.arrayelements, h.glformat, h.gltype, data);
			break;
		default:                                               // Should never happen
			return false;
		}

		if (h.miplevels == 1)
		{
			glGenerateMipmap(target);
		}

		//lastly, set the extra data
		handle->SetExtra(tex);

		//nothing went wrong
		return true;
	}

}