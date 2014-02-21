#include "stdafx.h"
#include "Material.hpp"

namespace Quicksand
{


	Material::Material( void )
	{
		ZeroMemory( &m_Material, sizeof(MaterialStruct) );
		m_Material.Diffuse = g_White;
		m_Material.Ambient = ColorRGBA( 0.10f, 0.10f, 0.10f, 1.0f );
		m_Material.Specular = g_White;
		m_Material.Emissive = g_Black;
	}

	Material::~Material( void )
	{

	}


	void Material::SetAmbient( const ColorRGBA& color )
	{
		m_Material.Ambient = color;
	}

	const ColorRGBA& Material::GetAmbient( void ) const
	{
		return m_Material.Ambient;
	}


	void Material::SetDiffuse( const ColorRGBA& color )
	{
		m_Material.Diffuse = color;
	}

	const ColorRGBA& Material::GetDiffuse( void ) const
	{
		return m_Material.Diffuse;
	}


	void Material::SetSpecular( const ColorRGBA& color )
	{
		m_Material.Specular = color;
	}

	const ColorRGBA& Material::GetSpecular( void ) const
	{
		return m_Material.Specular;
	}


	void Material::SetEmissive( const ColorRGBA& color )
	{
		m_Material.Emissive = color;
	}

	const ColorRGBA& Material::GetEmissive( void ) const
	{
		return m_Material.Emissive;
	}


	void Material::SetAlpha( const float alpha )
	{
		m_Material.Diffuse.a = alpha;
	}

	bool Material::HasAlpha( void ) const
	{
		return GetAlpha() != fOPAQUE;
	}

	float Material::GetAlpha( void ) const
	{
		return m_Material.Diffuse.a;
	}




}