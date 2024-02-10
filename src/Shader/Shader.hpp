#ifndef SHADER_HPP
#define SHADER_HPP
#include <GL/glew.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "../Util/Util.hpp"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#elif __linux__
#include <GL/gl.h>
#elif _WIN32
#include <GL/gl.h>
#endif

enum ShaderType {
    VERTEX,
    FRAGMENT
};

class Shader {
    public:
        Shader(const std::string path, ShaderType type);

        void free();

        friend class ShaderProgram;
    private:
        GLint getShaderID(ShaderType type);
        std::string getTypeFileExtension(ShaderType type);
        void checkCompiled();

        unsigned int ID;
};

#endif