#ifndef BUFFER_HPP
#define BUFFER_HPP

namespace Quicksand
{

	//some enumerations to encapsulate the OpenGL
	//enums into something better organized
	enum GLBufferAccess
	{
		GLBufferReadOnly = 0x88D8,
		GLBufferWriteOnly = 0x88B9,
		GLBufferReadWrite = 0x88BA
	};

	enum GLBufferType
	{
		GLBufferVertex = 0x8892,
		GLBufferIndex  = 0x8893,
		GLBufferPixelPack = 0x88EB,
		GLBufferPixelUnpack = 0x88EC
	};

	enum GLBufferUsagePattern
	{
		GLBufferUsageStreamDraw = 0x88E0,
		GLBufferUsageStreamRead = 0x88E1,
		GLBufferUsageStreamCopy = 0x88E2,
		GLBufferUsageStaticDraw = 0x88E4,
		GLBufferUsageStaticRead = 0x88E5,
		GLBufferUsageStaticCopy = 0x88E6,
		GLBufferUsageDynamicDraw = 0x88E8,
		GLBufferUsageDynamicRead = 0x88E9,
		GLBufferUsageDynamicCopy = 0x88EA
	};

		//this is useful to detect if the openGL object is uninitialized
#define QSE_UNINITIALIZED_OPENGL 0xFFFFFFFF

	//these are all helper classes to encapsulate openGL buffers
	class GLBuffer
	{
		unsigned int m_BufferId;

		GLBufferAccess m_Access;
		GLBufferType   m_Type;
		GLBufferUsagePattern  m_Usage;

	public:

		GLBuffer(GLBufferType type = GLBufferVertex);
		~GLBuffer(void);



		//allocate data size count and fill it with given data
		void Allocate(const void* pData, int count);

		//allocates data size count into the openGL buffer
		void Allocate(int count);

		//get the buffer id
		unsigned int GetBufferId(void) const;

		//map the openGL buffer for writing call SetAccessType first
		void* Map(void);

		//map the openGL buffer for writing, this calles SetAccessType automatically
		void* Map(GLBufferAccess accessType);

		//this uses a mapping method but handles it all alone.  This assumes SetAccessType has been called
		void QuickMap(int offset, const void* pData, int count);

		//this uses a mapping method but handles it all alone.  This does not assume SetAcessType has been called
		void QuickMap(int offset, const void* pData, int count, GLBufferAccess accessType);

		//set the access type of this buffer when mapping
		void SetAccessType(GLBufferAccess accessType);

		//unmap the buffer
		bool UnMap(void);

		//bind the buffer for use
		bool Bind(void);

		//create MUST be called before use
		bool Create(void);

		//deletes the openGL buffer
		void Destroy(void);

		//returns true if the buffer has been created
		bool IsCreated(void) const;

		//gets the data and stores 'count' amount of data into 'pData' at the offset 'offset'
		bool Read(int offset, void * pData, int count);

		//unbinds the buffer
		void Release(void);

		//sets the usage pattern of this buffer
		void SetUsagePattern(GLBufferUsagePattern value);

		//get the size in bytes of the buffer
		int Size() const;

		//get the type of buffer
		GLBufferType Type() const;

		//get the usage pattern
		GLBufferUsagePattern UsagePattern() const;

		//write 'pData' to the buffer at 'offset' with the size 'count'
		void Write(int offset, const void * pData, int count);

		//releases the buffer of type and binds the null buffer
		static void Release(GLBufferType type)
		{
			glBindBuffer(type, 0);
		}
	};



	class GLVertexArrayObject
	{

	};

	typedef GLVertexArrayObject GLVAO;




}



#endif