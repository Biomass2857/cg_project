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
#include "../ObjLoader/ObjLoader.hpp"

class Object {
    public:
        Object(
            const std::vector<float> vertices,
            const std::vector<unsigned int> indices
        );

        Object(const std::string& obj_file_path);

        void render();
        void free();
    private:
        void init(
            const std::vector<float> vertices,
            const std::vector<unsigned int> indices
        );

        unsigned int indexCount;
        unsigned int VAO, VBO, IBO;
};

#endif