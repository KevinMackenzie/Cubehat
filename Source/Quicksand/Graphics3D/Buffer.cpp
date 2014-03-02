#include "stdafx.h"
#include "Buffer.hpp"

namespace Quicksand
{
	CGLBuffer::CGLBuffer(GLBufferType type)
	{
		m_Type = type;
		m_BufferId = QSE_UNINITIALIZED_OPENGL;
	}

	CGLBuffer::~CGLBuffer(void)
	{
		if (m_BufferId != QSE_UNINITIALIZED_OPENGL)
		{
			Destroy();
		}
	}


	//allocate data size count and fill it with given data
	void CGLBuffer::Allocate(const void* pData, int count)
	{
		Bind();
		glBufferData(m_Type, count, pData, m_Usage);
		Release();
	}

	//allocates data size count into the openGL buffer
	void CGLBuffer::Allocate(int count)
	{
		Allocate(0, count);
	}

	//get the buffer id
	unsigned int CGLBuffer::GetBufferId(void) const
	{
		return m_BufferId;
	}

	//map the openGL buffer for writing
	void* CGLBuffer::Map(void)
	{
		Bind();
		void *data = glMapBuffer(m_Type, m_Access);
		Release();

		return data;
	}


	//map the openGL buffer for writing, this calles SetAccessType automatically
	void* CGLBuffer::Map(GLBufferAccess accessType)
	{
		SetAccessType(accessType);
		return Map();
	}

	//set the access type of this buffer when mapping
	void CGLBuffer::SetAccessType(GLBufferAccess accessType)
	{
		m_Access = accessType;
	}

	//unmap the buffer
	bool CGLBuffer::UnMap(void)
	{
		Bind();
		glUnmapBuffer(m_Type);
		Release();
	}


	void CGLBuffer::QuickMap(int offset, const void * pData, int count)
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

	void CGLBuffer::QuickMap(int offset, const void* pData, int count, GLBufferAccess accessType)
	{
		SetAccessType(accessType);
		QuickMap(offset, pData, accessType);
	}

	//bind the buffer for use
	bool CGLBuffer::Bind(void)
	{
		if (!IsCreated())
		{
			return false;
		}

		glBindBuffer(m_Type, m_BufferId);

		return true;
	}

	//create MUST be called before use
	bool CGLBuffer::Create(void)
	{
		glGenBuffers(1, &m_BufferId);

		return IsCreated();
	}

	//deletes the openGL buffer
	void CGLBuffer::Destroy(void)
	{
		glDeleteBuffers(1, &m_BufferId);
		//just in case
		Release();
	}

	//returns true if the buffer has been created
	bool CGLBuffer::IsCreated(void) const
	{
		return (m_BufferId != QSE_UNINITIALIZED_OPENGL);
	}

	//gets the data and stores 'count' amount of data into 'pData' at the offset 'offset'
	bool CGLBuffer::Read(int offset, void * pData, int count)
	{
		glGetBufferSubData(m_Type, offset, count, pData);

		return pData != 0;
	}

	//unbinds the buffer
	void CGLBuffer::Release(void)
	{
		glBindBuffer(m_Type, 0);
	}

	//sets the usage pattern of this buffer
	void CGLBuffer::SetUsagePattern(GLBufferUsagePattern value)
	{
		m_Usage = value;
	}

	//get the size in bytes of the buffer
	int CGLBuffer::Size() const
	{
		if (!IsCreated())
			return -1;

		GLint buffSize;

		glBindBuffer(m_Type, m_BufferId);
		glGetBufferParameteriv(m_Type, GL_BUFFER_SIZE, &buffSize);
		glBindBuffer(m_Type, 0);

		return buffSize;
	}

	//get the type of buffer
	GLBufferType CGLBuffer::Type() const
	{
		return m_Type;
	}

	//get the usage pattern
	GLBufferUsagePattern CGLBuffer::UsagePattern() const
	{
		return m_Usage;
	}

	//write 'pData' to the buffer at 'offset' with the size 'count'
	void CGLBuffer::Write(int offset, const void * pData, int count)
	{
		glBufferSubData(m_Type, offset, count, pData);
	}

	////////////////////////////////////
	//CGLVertexArrayObject
	////////////////////////////////////


	CGLVertexArrayObject::CGLVertexArrayObject(void)
	{
		m_VaoId = QSE_UNINITIALIZED_OPENGL;
	}


	//this creats the VAO, this should be the first thing to call
	bool CGLVertexArrayObject::Create(void)
	{
		glGenVertexArrays(1, &m_VaoId);

		if (!IsCreated())
			return false;
		else
			return true;
	}

	//this is used to check if we have initialized the Id
	bool CGLVertexArrayObject::IsCreated(void)
	{
		return m_VaoId != QSE_UNINITIALIZED_OPENGL;
	}

	//bind the VAO for whatever
	void CGLVertexArrayObject::Bind(void)
	{
		if (!IsCreated())
		{
			QSE_ASSERT("Attempt to bind uninitailized openGL VAO");
		}

		glBindVertexArray(m_VaoId);
	}

	//unbind the VAO
	void CGLVertexArrayObject::Release(void)
	{
		glBindVertexArray(0);
	}

	//enable a GL_ARRAY_BUFFER for the vao.
	void CGLVertexArrayObject::AddBuffer(shared_ptr<CGLBuffer> buffer, GLArrayBufferProperties arrayBufferProperties)
	{
		if (buffer != NULL)
		{

			if (buffer->IsCreated())
			{


				Bind();

				glEnableVertexAttribArray(arrayBufferProperties.m_Index);
				glBindBuffer(buffer->Type(), buffer->GetBufferId());
				glVertexAttribPointer(
					arrayBufferProperties.m_Index,
					arrayBufferProperties.m_Size,
					arrayBufferProperties.m_Type,
					arrayBufferProperties.m_Normalized,
					arrayBufferProperties.m_Stride,
					arrayBufferProperties.m_pOffset);



				Release();


				//add the buffer to the list to be freed at the end
				m_Buffers.push_back(buffer);
			}
			else
			{
				QSE_ASSERT("Attempt to enable a vertex attrubute array on an uninitialized buffer");
			}
		}
		else
		{
			QSE_ASSERT("Attempt to add enable a vertex attribute array on a null buffer");
		}
	}

	//this disables all of the buffers attached with the VBO
	void CGLVertexArrayObject::DisableBuffers(void)
	{
		Bind();
		for (auto i : m_Buffers)
		{
			glDisableVertexAttribArray(i->GetBufferId());
		}
		Release();
	}

	//this deletes the buffer
	void CGLVertexArrayObject::Destroy(void)
	{
		glDeleteVertexArrays(1, &m_VaoId);
	}

}