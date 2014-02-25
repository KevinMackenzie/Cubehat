#include "stdafx.h"
#include "Buffer.hpp"

namespace Quicksand
{
	GLBuffer::GLBuffer(GLBufferType type)
	{
		m_Type = type;
		m_BufferId = QSE_UNINITIALIZED_OPENGL;
	}

	GLBuffer::~GLBuffer(void)
	{
		if (m_BufferId != QSE_UNINITIALIZED_OPENGL)
		{
			Destroy();
		}
	}


	//allocate data size count and fill it with given data
	void GLBuffer::Allocate(const void* pData, int count)
	{
		Bind();
		glBufferData(m_Type, count, pData, m_Usage);
		Release();
	}

	//allocates data size count into the openGL buffer
	void GLBuffer::Allocate(int count)
	{
		Allocate(0, count);
	}

	//get the buffer id
	unsigned int GLBuffer::GetBufferId(void) const
	{
		return m_BufferId;
	}

	//map the openGL buffer for writing
	void* GLBuffer::Map(void)
	{
		Bind();
		void *data = glMapBuffer(m_Type, m_Access);
		Release();

		return data;
	}


	//map the openGL buffer for writing, this calles SetAccessType automatically
	void* GLBuffer::Map(GLBufferAccess accessType)
	{
		SetAccessType(accessType);
		return Map();
	}

	//set the access type of this buffer when mapping
	void GLBuffer::SetAccessType(GLBufferAccess accessType)
	{
		m_Access = accessType;
	}

	//unmap the buffer
	bool GLBuffer::UnMap(void)
	{
		Bind();
		glUnmapBuffer(m_Type);
		Release();
	}


	void GLBuffer::QuickMap(int offset, const void * pData, int count)
	{
		Bind();
		char *data = (char*)glMapBuffer(m_Type, m_Access);
		char *inData = (char*)pData;

		if (offset + count > sizeof(data) / sizeof(char))
		{
			QSE_ASSERT("Attempt to map a buffer pasts its bounds");
		}

		if (count > sizeof(inData) / sizeof(char))
		{
			QSE_ASSERT("Attempt to map data into a buffer that is smaller that its alleged size");
		}


		for (int i = 0; i < count; i++)
		{
			data[i + offset] =  inData[i];
		}

		Release();
	}

	void GLBuffer::QuickMap(int offset, const void* pData, int count, GLBufferAccess accessType)
	{
		SetAccessType(accessType);
		QuickMap(offset, pData, accessType);
	}

	//bind the buffer for use
	bool GLBuffer::Bind(void)
	{
		if (!IsCreated())
		{
			return false;
		}

		glBindBuffer(m_Type, m_BufferId);

		return true;
	}

	//create MUST be called before use
	bool GLBuffer::Create(void)
	{
		glGenBuffers(1, &m_BufferId);

		return IsCreated();
	}

	//deletes the openGL buffer
	void GLBuffer::Destroy(void)
	{
		glDeleteBuffers(1, &m_BufferId);
		//just in case
		Release();
	}

	//returns true if the buffer has been created
	bool GLBuffer::IsCreated(void) const
	{
		return (m_BufferId != QSE_UNINITIALIZED_OPENGL);
	}

	//gets the data and stores 'count' amount of data into 'pData' at the offset 'offset'
	bool GLBuffer::Read(int offset, void * pData, int count)
	{
		glGetBufferSubData(m_Type, offset, count, pData);

		return pData != 0;
	}

	//unbinds the buffer
	void GLBuffer::Release(void)
	{
		glBindBuffer(m_Type, 0);
	}

	//sets the usage pattern of this buffer
	void GLBuffer::SetUsagePattern(GLBufferUsagePattern value)
	{
		m_Usage = value;
	}

	//get the size in bytes of the buffer
	int GLBuffer::Size() const
	{
		if (!IsCreated())
			return -1;

		GLint buffSize;

		Bind();
		glGetBufferParameteriv(m_Type, GL_BUFFER_SIZE, &buffSize);
		Release();

		return buffSize;
	}

	//get the type of buffer
	GLBufferType GLBuffer::Type() const
	{
		return m_Type;
	}

	//get the usage pattern
	GLBufferUsagePattern GLBuffer::UsagePattern() const
	{
		return m_Usage;
	}

	//write 'pData' to the buffer at 'offset' with the size 'count'
	void GLBuffer::Write(int offset, const void * pData, int count)
	{
		glBufferSubData(m_Type, offset, count, pData);
	}

}