#include "stdafx.h"

namespace ShaderUtil
{
	void printShaderLog(GLuint shader);

	GLuint loadShader(std::string path, GLenum shaderType);

	bool createProgram(GLuint& programID, GLuint vertex, GLuint fragment);

	GLuint createVertFragProg(std::string vertPath, std::string fragPath);
}