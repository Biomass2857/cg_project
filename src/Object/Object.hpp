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

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Util/Util.hpp"
#include "../ObjLoader/ObjLoader.hpp"
#include "../ShaderProgram/ShaderProgram.hpp"

class Object {
    public:
        Object(
            const std::vector<float> vertices,
            const std::vector<unsigned int> indices
        );

        Object(const std::string& obj_file_path);

        void setShader(const ShaderProgram& shader);

        void scale(float factor);

        void render();
        void free();
    private:
        void init(
            const std::vector<float> vertices,
            const std::vector<unsigned int> indices
        );

        glm::mat4 transformation;
        const ShaderProgram* shader;

        unsigned int indexCount;
        unsigned int VAO, VBO, IBO;
};

#endif