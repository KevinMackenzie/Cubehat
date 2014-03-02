#ifndef QSE_SHADER_HPP
#define QSE_SHADER_HPP

//this definesa bunch of classes to deal with openGL shaders

namespace Quicksand
{
	//set these to the appropriate openGL
	//enum type so we can directly enter them 
	//into the openGL functions
	enum ShaderType
	{
		QSE_UNDEFINED           = NULL,
		QSE_COMPUTE_SHADER		= GL_COMPUTE_SHADER,
		QSE_VERTEX_SHADER		= GL_VERTEX_SHADER,
		QSE_TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
		QSE_TESS_EVAL_SHADER	= GL_TESS_EVALUATION_SHADER,
		QSE_GEOMETRY_SHADER		= GL_GEOMETRY_SHADER,
		QSE_FRAGMENT_SHADER		= GL_FRAGMENT_SHADER
	};


	//a set of structures for getting compile, build and link information
	struct ShaderCompileStruct_t
	{
		const char* m_InfoLog;
		bool m_bSuccess;

		ShaderCompileStruct_t(void) : m_bSuccess(false), m_InfoLog(""){}

		void Flush(void)
		{
			delete[] m_InfoLog;
		}
	};

	typedef ShaderCompileStruct_t ShaderCompileStruct;
	typedef ShaderCompileStruct_t ShaderProgramBuildStruct;

	class CShader
	{
		CShader(void){}
	protected:
		GLuint m_ShaderID;
		ShaderType m_Type;
		std::string m_TextCache;

		ShaderCompileStruct m_CompileInfo;
	public:
		CShader(ShaderType shaderType);
		virtual ~CShader(void);

		//fileName is the name of the file that is should load,
		//replace is whether it should append the current shader information, 
		//or replace it.  Once the information is compiled though, the text cache 
		//is flushed
		bool Load(std::string fileName, bool append = false);
		bool LoadList(std::list<std::string> fileNames, bool append = false);
		bool Load(const char* shaderText, bool append = false);

		//compile the shader with the text in the text cache
		bool Compile(bool flushOnSuccess = true);

		//flush the cache of text that exists before it compiles
		void FlushTextCache(void);

		//returns NULL if there is no text in the cache
		const char* GetTextCache(void) const;

		//this allows a user to manually edit the text
		char* GetEditibleTextCache(void);

		//this lets you get compile information from opeGL
		const ShaderCompileStruct& GetCompileInfo(void) const;

		ShaderType GetShaderType(void) const { return m_Type; }

		GLuint GetShaderID(void) const { return m_ShaderID; }

	};

	class CShaderProgramBase
	{
	protected:
		GLuint m_ProgramID;

		//this tells us about the link info for programs
		ShaderProgramBuildStruct m_BuildInfo;
	public:
		CShaderProgramBase(void){ m_ProgramID = glCreateProgram(); }
		virtual ~CShaderProgramBase(void){ glDeleteProgram(m_ProgramID); }

		const GLuint GetProgramID(void) const { return m_ProgramID; }

		//use the program ID for the current draw calls
		virtual void UseProgram(void) const = 0;

		void FlushProgramInfo(void) { m_BuildInfo.Flush(); }
		const ShaderProgramBuildStruct& GetProgramInfo(void) const { return m_BuildInfo; }

		ShaderProgramBuildStruct GetAndFlush(void) { FlushProgramInfo(); return GetProgramInfo(); }
	};

	class CShaderProgram : public CShaderProgramBase
	{
	public:
		CShaderProgram(void){}
		~CShaderProgram(void){}

		//use the program for the draw calls
		virtual void UseProgram(void) const { glUseProgram(m_ProgramID); }

		//attaches a shader
		void AttachShader(CShader shader);

		//this builds the program with the contained shaders
		//a seperable mode will be implimented when needed
		bool Build(void);
	};

	//TODO: impliment program pipeline objects and seperable shader programs
	/*
	class CProgramPipelineObject : public CShaderProgramBase
	{
		std::vector<CShaderProgram*> m_SeperablePrograms;

	public:
		ProgramPipelineObject(void){}
		~ProgramPipelineObject(void){}

		//attach a program to this seperable program.  The list of shader types is the different
		//shader types it contains
		void AttachProgram(CShaderProgramBase* pProgram, std::list<ShaderType> programShaderType);

		//link the shader programs
		void Link(void);

	};*/


}


#endif