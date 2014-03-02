#include "stdafx.h"
#include "XmlResource.hpp"


namespace Quicksand
{
	TiXmlElement* CXmlResourceLoader::LoadAndReturnRootXmlElement( const char* resourceString )
	{
		CResource resource( resourceString );
		shared_ptr<CResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle( &resource );  // this actually loads the XML file from the zip file
		shared_ptr<CXmlResourceExtraData> pExtraData = static_pointer_cast<CXmlResourceExtraData>(pResourceHandle->GetExtra( ));
		return pExtraData->GetRoot( );
	}


	bool CXmlResourceLoader::VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<CResHandle> handle )
	{
		if (rawSize < 0)
			return false;

		shared_ptr<CXmlResourceExtraData> pExtraData = shared_ptr<CXmlResourceExtraData>( QSE_NEW( CXmlResourceExtraData ) );
		pExtraData->ParseXml( rawBuffer );

		handle->SetExtra( shared_ptr<CXmlResourceExtraData>( pExtraData ) );

		return true;
	}


	void CXmlResourceExtraData::ParseXml(char* rawBuffer)
	{
		m_XmlDocument.Parse( rawBuffer );
	}
}