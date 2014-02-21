#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../3rdParty/glm-0.9.5.2/glm/glm.hpp"

namespace Quicksand
{
	typedef glm::vec4 ColorRGBA;

	struct MaterialStruct
	{
		ColorRGBA Diffuse;
		ColorRGBA Ambient;
		ColorRGBA Specular;
		ColorRGBA Emissive;
		float Power;
	};

	class Material
	{
		MaterialStruct m_Material;

	public:
		Material( void );
		~Material( void );

		void SetAmbient( const ColorRGBA& color );
		const ColorRGBA& GetAmbient( void ) const;

		void SetDiffuse( const ColorRGBA& color );
		const ColorRGBA& GetDiffuse( void ) const;

		void SetSpecular( const ColorRGBA& color );
		const ColorRGBA& GetSpecular( void ) const;

		void SetEmissive( const ColorRGBA& color );
		const ColorRGBA& GetEmissive( void ) const;

		void SetAlpha( const float alpha );
		bool HasAlpha( void ) const;
		float GetAlpha( void ) const;


	};

}


#endif