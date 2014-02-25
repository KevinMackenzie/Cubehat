#ifndef QSE_BUFFER_HPP
#define QSE_BUFFER_HPP

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
		GLuint m_BufferId;

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
		GLuint GetBufferId(void) const;

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


	//a structure for identifying openGL buffer data
	struct GLArrayBufferProperties
	{
		GLuint m_Index;//layout location
		GLint  m_Size;//size of each element (eg vec3 would be 3) default is 4
		GLenum m_Type;//e.g. GL_FLOAT GL_INT
		GLboolean m_Normalized;//usually GL_FALSE
		GLsizei m_Stride;//stride between data
		const GLvoid* m_pOffset;//array buffer offset

		GLArrayBufferProperties(GLuint index, GLint size = 4, GLenum type = GL_FLOAT, GLboolean normalized = GL_FALSE, GLsizei stride = 0, const GLvoid *pOffset = (GLvoid*)0)
		{
			m_Index = index;
			m_Size = size;
			m_Type = type;
			m_Normalized = normalized;
			m_Stride = stride;
			m_pOffset = pOffset;
		}
	};


	class GLVertexArrayObject
	{
		GLuint m_VaoId;

		std::list<shared_ptr<GLBuffer> > m_Buffers;

	public:

		GLVertexArrayObject(void);
		~GLVertexArrayObject(void);

		//this creats the VAO, this should be the first thing to call
		bool Create(void);

		//this is used to check if we have initialized the Id
		bool IsCreated(void);

		//bind the VAO for whatever
		void Bind(void);

		//unbind the VAO
		void Release(void);

		//enable a GL_ARRAY_BUFFER for the vao.
		void AddBuffer(shared_ptr<GLBuffer> buffer, GLArrayBufferProperties arrayBufferProperties);

		//this disables all of the buffers attached with the VBO
		void DisableBuffers(void);

		//this deletes the buffer
		void Destroy(void);

	};

	typedef GLVertexArrayObject GLVAO;




}



#endif