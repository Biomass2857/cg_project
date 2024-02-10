#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#elif __linux__
#include <GL/gl.h>
#elif _WIN32
#include <GL/gl.h>
#endif

#include <vector>
#include "../Util/Util.hpp"

class Object {
    public:
        Object(
            const std::vector<float> vertices,
            const std::vector<unsigned short> indices
        );

        void render();
        void free();
    private:
        unsigned int indexCount;
        unsigned int VAO, VBO, IBO;
};

#endif