#include "stdafx.h"
#include "Shader.hpp"


namespace Quicksand
{


	Shader::Shader(ShaderType shaderType)
	{
		m_Type = shaderType;
		m_TextCache = "";

		m_ShaderID = glCreateShader(shaderType);
	}

	Shader::~Shader(void)
	{
		glDeleteShader(m_ShaderID);
	}

	void Shader::FlushTextCache(void)
	{
		m_TextCache.clear();
	}

	bool Shader::Load(std::string fileName, bool append = false)
	{

	}

	bool Shader::LoadList(std::list<std::string> fileNames, bool append = false)
	{

	}

	bool Shader::Load(const char* shaderText, bool append = false)
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


	bool Shader::Compile(void)
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

		//return whether it was successful
		return m_CompileInfo.m_bSuccess;
	}

	//returns NULL if there is no text in the cache
	const char* Shader::GetTextCache(void) const
	{
		return m_TextCache.c_str();
	}

	//this allows a user to manually edit the text
	char* Shader::GetEditibleTextCache(void)
	{
		return const_cast<char*>(m_TextCache.c_str());
	}


	//this lets you get compile information from opeGL
	const ShaderCompileStruct& Shader::GetCompileInfo(void) const
	{
		return m_CompileInfo;
	}


	//attaches a shader
	void ShaderProgram::AttachShader(Shader shader)
	{
		glAttachShader(m_ProgramID, shader.GetShaderID());
	}

	//this builds the program with the contained shaders
	//seperable is whether to build using seperable mode
	void ShaderProgram::Build(bool bSeperable = false)
	{
		glLinkProgram(m_ProgramID);
	}
}