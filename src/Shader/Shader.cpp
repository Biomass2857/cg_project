#include "Shader.hpp"

Shader::Shader(const std::string path, ShaderType type) {
    std::string fileExtension = getTypeFileExtension(type);
    std::string source = Util::readFileToString(path + fileExtension);

    const char* const sourcePtr = source.c_str();

    ID = glCreateShader(getShaderID(type));
    glShaderSource(ID, 1, &sourcePtr, nullptr);
    glCompileShader(ID);

    checkCompiled();
}

void checkShaderCompiled(GLint shader) {
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<GLchar> infoLog(length);
        glGetShaderInfoLog(shader, length, &length, &infoLog[0]);
        std::cerr <<"Shader compilation failed: "<< &infoLog[0] << std::endl;
        glDeleteShader(shader);
        exit(-1);
    }
}

GLint Shader::getShaderID(ShaderType type) {
    switch(type) {
        case ShaderType::VERTEX:
            return GL_VERTEX_SHADER;
        case ShaderType::FRAGMENT:
            return GL_FRAGMENT_SHADER;
    }
}

std::string Shader::getTypeFileExtension(ShaderType type) {
    switch(type) {
        case ShaderType::VERTEX:
            return ".vert";
        case ShaderType::FRAGMENT:
            return ".frag";
    }

    return "";
}

void Shader::checkCompiled() {
    GLint compiled;
    glGetShaderiv(ID, GL_COMPILE_STATUS, &compiled);
    if(!compiled) {
        GLint length;
        glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
        std::vector<GLchar> infoLog(length);
        glGetShaderInfoLog(ID, length, &length, &infoLog[0]);
        std::cerr <<"Shader compilation failed: "<< &infoLog[0] << std::endl;
        glDeleteShader(ID);
        exit(-1);
    }
}

void Shader::free() {
    glDeleteShader(ID);
}