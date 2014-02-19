#ifndef SHADER_HPP
#define SHADER_HPP

//this definesa bunch of classes to deal with openGL shaders

namespace Quicksand
{
	//set these to the appropriate openGL
	//enum type so we can directly enter them 
	//into the openGL functions
	enum ShaderType
	{
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
	};

	typedef ShaderCompileStruct_t ShaderCompileStruct;
	typedef ShaderCompileStruct_t ShaderProgramBuildStruct;

	class Shader
	{
	protected:
		GLuint m_ShaderID;
		ShaderType m_Typel;
		const char* m_TextCache;

		ShaderCompileStruct m_CompileInfo;

	public:
		Shader(void);
		~Shader(void);

		//fileName is the name of the file that is should load,
		//replace is whether it should append the current shader information, 
		//or replace it.  Once the information is compiled though, the text cache 
		//is flushed
		bool Load(std::string fileName, bool append = false);
		bool LoadList(std::list<std::string> fileNames, bool append = false);
		bool Load(const char* shaderText, bool append = false);

		//compile the shader with the text in the text cache
		bool Compile(void);

		//flush the cache of text that exists before it compiles
		void FlushTextCache(void);

		//returns NULL if there is no text in the cache
		const char* GetTextCache(void) const;

		//this allows a user to manually edit the text
		char* GetEditibleTextCache(void);

		//this lets you get compile information from opeGL
		const ShaderCompileStruct& GetCompileInfo(void) const;

	};

	class ShaderProgramBase
	{
	protected:
		GLuint m_ProgramID;

		ShaderProgramBuildStruct m_BuildInfo;
	protected:

		//you must impliment this, we do not want any direct instances of this class
		virtual void Overide(void)const = 0;

	public:

		const GLuint GetProgramID(void) const { return m_ProgramID; }

		//use the program ID for the current draw calls
		void UseProgram(void) const;


		const ShaderProgramBuildStruct& GetProgramInfo(void) const;
	};

#define OVERIDEPROGRAM virtual void Overide(void)const{}

	class ShaderProgram : public ShaderProgramBase
	{
		std::map<ShaderType, Shader> m_Shaders;

	protected:

		OVERIDEPROGRAM

	public:
		ShaderProgram(void){}
		~ShaderProgram(void){}

		//attaches a shader
		void AttachShader(Shader shader);

		//this builds the program with the contained shaders
		//seperable is whether to build using seperable mode
		void Build(bool bSeperable = false);
	};

	//a useful typedef
	typedef std::list<ShaderType> ShaderTypeList;

	class ShaderProgramSeperable : public ShaderProgramBase
	{
		std::map<ShaderTypeList, ShaderProgram*> m_SeperablePrograms;

	protected:

		OVERIDEPROGRAM

	public:
		ShaderProgramSeperable(void){}
		~ShaderProgramSeperable(void){}

		//attach a program to this seperable program.  The list of shader types is the different
		//shader types it contains
		void AttachProgram(ShaderProgramBase* pProgram, std::list<ShaderType> programShaderType);

		//link the shader programs
		void Link(void);

	};


}


#endif