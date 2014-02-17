#ifndef XMLRESOURCE_HPP
#define XMLRESOURCE_HPP

#include "ResourceCache.hpp"
#include "../3rdParty/tinyxml-2.6.2/tinyxml.h"

namespace Quicksand
{


	class XmlResourceExtraData : public IResourceExtraData
	{
		//the data
		TiXmlDocument m_XmlDocument;

	public:
		virtual std::string VToString( ) { return "XmlResourceExtraData"; }
		void ParseXml( char* pRawBuffer );
		TiXmlElement* GetRoot( void ) { return m_XmlDocument.RootElement( ); }
	};

	class XmlResourceLoader : public IResourceLoader
	{
	public:
		virtual bool VUseRawFile() { return false; }
		virtual bool VDiscardRawBufferAfterLoad() { return true; }
		virtual unsigned int VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize ) { return rawSize; }
		virtual bool VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle );
		virtual std::string VGetPattern() { return "*.xml"; }


		//this is purly for convenience
		static TiXmlElement* LoadAndReturnRootXmlElement( const char* resourceString );
	};

}


#endif