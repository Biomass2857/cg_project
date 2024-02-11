#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(std::vector<Shader> shaders) {
    ID = glCreateProgram();
    for(auto it = shaders.begin(); it != shaders.end(); it++) {
        add(*it);
    }

    glLinkProgram(ID);

    checkLinked();
}

void ShaderProgram::add(Shader shader) {
    glAttachShader(ID, shader.ID);
}

void ShaderProgram::use() const {
    glUseProgram(ID);
}

void ShaderProgram::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderProgram::setMatrix4(const std::string &name, const glm::mat4 matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setTexture(const std::string &name, const Texture& texture) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), 0/*texture.slot*/);
}

void ShaderProgram::checkLinked() const {
    GLint linkStatus;
    glGetProgramiv(ID, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        GLint logLength;
        glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(logLength);
        glGetProgramInfoLog(ID, logLength, nullptr, log.data());
        std::cerr << "Linking failed: " << log.data() << std::endl;
        exit(1);
    }
}

void ShaderProgram::free() {
    glDeleteProgram(ID);
}