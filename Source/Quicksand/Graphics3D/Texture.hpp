#ifndef QSE_TEXTURE_HPP
#define QSE_TEXTURE_HPP

namespace Quicksand
{
	//a texture helper class
	class CGLTexture
	{
		GLuint m_TextureId;

	public:
		CGLTexture(void);
		~CGLTexture(void);

		void Create(void) { glGenTextures(1, &m_TextureId); }


		GLuint GetId(void){ return m_TextureId; }

		//TODO: create a texture resource for the resource cache
		//      this will impliment the gli library found at http://gli.g-truc.net/


	};



}


#endif