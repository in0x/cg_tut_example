#include "stdafx.h"

class ShaderUtil
{
public: 
	static void printShaderLog(GLuint shader)
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
			std::cout << shader << " is not a shader" << std::endl;
		}
	}

	static GLuint createShader(std::string path, GLenum shaderType)
	{
		std::string shaderString;
		std::ifstream fileStream(path);
		GLuint shader;

		if (fileStream)
		{
			shaderString.assign(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());

			shaderString.erase(std::remove_if(shaderString.begin(), shaderString.end(), 
				[](const char c) {
					return !(c >= 0 && c < 128);
				}
			), shaderString.end());

			const char* shaderSource = shaderString.c_str();

			shader = glCreateShader(shaderType);

			glShaderSource(shader, 1, (const char**)&shaderSource, NULL);

			glCompileShader(shader);
		}

		else
		{
			throw std::exception(("Failed to load shader" + path).c_str());
		}

		printShaderLog(shader);

		return shader;
	}
};