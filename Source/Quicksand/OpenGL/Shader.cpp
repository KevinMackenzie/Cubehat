#include "stdafx.h"
#include "Shader.hpp"

namespace Quicksand
{


	CShader::CShader(ShaderType shaderType)
	{
		m_Type = shaderType;
		m_TextCache = "";

		m_ShaderID = glCreateShader(shaderType);
	}

	CShader::~CShader(void)
	{
		glDeleteShader(m_ShaderID);
	}

	void CShader::FlushTextCache(void)
	{
		m_TextCache.clear();
	}

	bool CShader::Load(std::string fileName, bool append = false)
	{

	}

	bool CShader::LoadList(std::list<std::string> fileNames, bool append = false)
	{

	}

	bool CShader::Load(const char* shaderText, bool append = false)
	{
		if (append)
		{
			m_TextCache += const_cast<char*>(shaderText);
		}
		else
		{
			m_TextCache = const_cast<char*>(shaderText);
		}
	}


	bool CShader::Compile(bool flushOnSuccess)
	{
		const char* strTmp = m_TextCache.c_str();

		glShaderSource(m_ShaderID, 1, &strTmp, NULL);

		//now compile
		glCompileShader(m_ShaderID);

		//get the resaults
		
		GLint success = 0;
		glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &success);

		m_CompileInfo.m_bSuccess = success == GL_TRUE ? true : false;

		GLint size = 0;
		glGetShaderiv(m_ShaderID, GL_INFO_LOG_LENGTH, &size);

		glGetShaderInfoLog(m_ShaderID, size, &size, const_cast<char*>(m_CompileInfo.m_InfoLog));

		//if the compile was succesful, and we want to flush, flush the textcache
		if (flushOnSuccess && m_CompileInfo.m_bSuccess)
			FlushTextCache();

		//return whether it was successful
		return m_CompileInfo.m_bSuccess;
	}

	//returns NULL if there is no text in the cache
	const char* CShader::GetTextCache(void) const
	{
		return m_TextCache.c_str();
	}

	//this allows a user to manually edit the text
	char* CShader::GetEditibleTextCache(void)
	{
		return const_cast<char*>(m_TextCache.c_str());
	}


	//this lets you get compile information from opeGL
	const ShaderCompileStruct& CShader::GetCompileInfo(void) const
	{
		return m_CompileInfo;
	}


	//attaches a shader
	void CShaderProgram::AttachShader(CShader shader)
	{
		glAttachShader(m_ProgramID, shader.GetShaderID());
	}

	//this builds the program with the contained shaders
	bool CShaderProgram::Build()
	{
		glLinkProgram(m_ProgramID);

		//get the resaults

		GLint success = 0;
		glGetShaderiv(m_ProgramID, GL_COMPILE_STATUS, &success);

		m_BuildInfo.m_bSuccess = success == GL_TRUE ? true : false;

		GLint size = 0;
		glGetShaderiv(m_ProgramID, GL_INFO_LOG_LENGTH, &size);

		glGetShaderInfoLog(m_ProgramID, size, &size, const_cast<char*>(m_BuildInfo.m_InfoLog));


		//return whether it was successful
		return m_BuildInfo.m_bSuccess;
	}
}