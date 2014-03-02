#include "stdafx.h"
#include "Texture.hpp"

namespace Quicksand
{

	CGLTexture::CGLTexture(void)
	{
		m_TextureId = 0;
	}

	CGLTexture::~CGLTexture(void)
	{
		glDeleteTextures(1, &m_TextureId);
	}

}