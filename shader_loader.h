#pragma once
#include <sstream>

class ShaderLoader {
public:
	std::stringstream getCode(const char * path);
	GLuint compileShader(const char* code, int shaderType);
};