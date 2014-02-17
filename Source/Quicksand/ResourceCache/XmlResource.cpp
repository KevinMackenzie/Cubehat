#include "stdafx.h"
#include "XmlResource.hpp"


namespace Quicksand
{
	TiXmlElement* XmlResourceLoader::LoadAndReturnRootXmlElement( const char* resourceString )
	{
		Resource resource( resourceString );
		shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle( &resource );  // this actually loads the XML file from the zip file
		shared_ptr<XmlResourceExtraData> pExtraData = static_pointer_cast<XmlResourceExtraData>(pResourceHandle->GetExtra( ));
		return pExtraData->GetRoot( );
	}


	bool XmlResourceLoader::VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle )
	{
		if (rawSize < 0)
			return false;

		shared_ptr<XmlResourceExtraData> pExtraData = shared_ptr<XmlResourceExtraData>( QSE_NEW( XmlResourceExtraData ) );
		pExtraData->ParseXml( rawBuffer );

		handle->SetExtra( shared_ptr<XmlResourceExtraData>( pExtraData ) );

		return true;
	}


	void XmlResourceExtraData::ParseXml(char* rawBuffer)
	{
		m_XmlDocument.Parse( rawBuffer );
	}
}