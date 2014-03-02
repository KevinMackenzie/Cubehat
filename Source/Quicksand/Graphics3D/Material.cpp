#include "stdafx.h"
#include "Material.hpp"

namespace Quicksand
{


	CMaterial::CMaterial( void )
	{
		ZeroMemory( &m_Material, sizeof(MaterialStruct) );
		m_Material.Diffuse = g_White;
		m_Material.Ambient = ColorRGBA( 0.10f, 0.10f, 0.10f, 1.0f );
		m_Material.Specular = g_White;
		m_Material.Emissive = g_Black;
	}

	CMaterial::~CMaterial( void )
	{

	}


	void CMaterial::SetAmbient( const ColorRGBA& color )
	{
		m_Material.Ambient = color;
	}

	const ColorRGBA& CMaterial::GetAmbient( void ) const
	{
		return m_Material.Ambient;
	}


	void CMaterial::SetDiffuse( const ColorRGBA& color )
	{
		m_Material.Diffuse = color;
	}

	const ColorRGBA& CMaterial::GetDiffuse( void ) const
	{
		return m_Material.Diffuse;
	}


	void CMaterial::SetSpecular( const ColorRGBA& color )
	{
		m_Material.Specular = color;
	}

	const ColorRGBA& CMaterial::GetSpecular( void ) const
	{
		return m_Material.Specular;
	}


	void CMaterial::SetEmissive( const ColorRGBA& color )
	{
		m_Material.Emissive = color;
	}

	const ColorRGBA& CMaterial::GetEmissive( void ) const
	{
		return m_Material.Emissive;
	}


	void CMaterial::SetAlpha( const float alpha )
	{
		m_Material.Diffuse.a = alpha;
	}

	bool CMaterial::HasAlpha( void ) const
	{
		return GetAlpha() != fOPAQUE;
	}

	float CMaterial::GetAlpha( void ) const
	{
		return m_Material.Diffuse.a;
	}




}