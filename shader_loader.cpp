#include <fstream>
#include <vector>
#include <GL/glew.h>

#include "shader_loader.h"

std::stringstream ShaderLoader::getCode(const char* path) {
    // Read the Shader code from the file
    std::string shaderCode;
    std::ifstream inputStream(path, std::ios::in);
    std::stringstream sstr;
    if (inputStream.is_open()) {
        sstr << inputStream.rdbuf();
        shaderCode = sstr.str();
        inputStream.close();
    } else {
        printf("Can't open %s. Are you in the right directory?\n", path);
        getchar();
    }
    return sstr;
}

GLuint ShaderLoader::compileShader(const char* code, int shaderType) {
    // Compile shader
    GLuint shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &code, NULL);
    glCompileShader(shaderId);
    // Check shader
    GLint result = GL_FALSE;
    int infoLogLength;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> error(infoLogLength + 1);
        glGetShaderInfoLog(shaderId, infoLogLength, NULL, &error[0]);
        printf("%s\n", &error[0]);
    }
    return shaderId;
}