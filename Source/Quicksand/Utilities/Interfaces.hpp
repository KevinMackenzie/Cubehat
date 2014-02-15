#ifndef INTERFACES_HPP
#define INTERFACES_HPP

//all of the interface clases used in this engine

class Resource;
class IResourceFile;
class ResHandle;

//
// class IResourceLoader
//
class IResourceLoader
{
public:
	virtual std::string VGetPattern( ) = 0;
	virtual bool VUseRawFile( ) = 0;
	virtual bool VDiscardRawBufferAfterLoad( ) = 0;
	virtual bool VAddNullZero( ) { return false; }
	virtual unsigned int VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize ) = 0;
	virtual bool VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle ) = 0;
};

class IResourceFile
{
public:
	virtual bool VOpen( ) = 0;
	virtual int VGetRawResourceSize( const Resource &r ) = 0;
	virtual int VGetRawResource( const Resource &r, char *buffer ) = 0;
	virtual int VGetNumResources( ) const = 0;
	virtual std::string VGetResourceName( int num ) const = 0;
	virtual bool VIsUsingDevelopmentDirectories( void ) const = 0;
	virtual ~IResourceFile( ) { }
};





#endif INTERFACES_HPP