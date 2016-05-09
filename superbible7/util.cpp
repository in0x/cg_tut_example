#include "util.h"

namespace ShaderUtil
{
	void printShaderLog(GLuint shader)
	{
		if (glIsShader(shader))
		{
			int infoLogLength = 0;
			int maxLength = infoLogLength;

			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			char* infoLog = new char[maxLength];

			glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);

			if (infoLogLength > 0)
			{
				std::cout << infoLog << std::endl;
			}

			delete[] infoLog;
		}
		else
		{
			std::cout << "Name " << shader << " is not a shader\n" << std::endl;
		}
	}

	GLuint loadShader(std::string path, GLenum shaderType)
	{
		GLuint shaderID;

		std::string shaderString;
		std::ifstream fileStream(path);

		if (fileStream)
		{
			shaderString.assign(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());

			shaderID = glCreateShader(shaderType);

			const char* shaderSource = shaderString.c_str();
			glShaderSource(shaderID, 1, (const char**)&shaderSource, NULL);

			glCompileShader(shaderID);

			GLint shaderCompiled = GL_FALSE;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);

			if (shaderCompiled != GL_TRUE)
			{
				std::cout << "Unable to compile shader " << shaderID << " Source:\n" << shaderSource << std::endl;
				printShaderLog(shaderID);
				glDeleteShader(shaderID);
				shaderID = 0;
			}
		}
		else
		{
			std::cout << "Unable to open file " << path.c_str() << std::endl;
		}

		return shaderID;
	}

	bool createProgram(GLuint& programID, GLuint vertex, GLuint fragment)
	{
		programID = glCreateProgram();

		glAttachShader(programID, vertex);
		glAttachShader(programID, fragment);

		glLinkProgram(programID);

		GLint programSuccess = GL_TRUE;
		glGetProgramiv(programID, GL_LINK_STATUS, &programSuccess);

		if (programSuccess != GL_TRUE)
		{
			std::cout << "Error while linking shader program \n" << "GL Error Code: " << glGetError() << std::endl;
			return false;
		}

		return true;
	}

	GLuint createVertFragProg(std::string vertPath, std::string fragPath)
	{
		GLuint vertShaderID = loadShader(vertPath, GL_VERTEX_SHADER);
		GLuint fragShaderID = loadShader(fragPath, GL_FRAGMENT_SHADER);

		GLuint programID;
		createProgram(programID, vertShaderID, fragShaderID);

		glDeleteShader(vertShaderID);
		glDeleteShader(fragShaderID);

		return programID;
	}
}
