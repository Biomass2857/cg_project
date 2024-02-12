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
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Util/Util.hpp"
#include "../ObjLoader/ObjLoader.hpp"
#include "../ShaderProgram/ShaderProgram.hpp"
#include "../Texture/Texture.hpp"
#include "../Camera/Camera.hpp"

enum class VertexFeature { Position = 0, Normal = 1, Color = 2, UV = 3 };

class Object {
    public:
        Object(
            const std::vector<float> vertices,
            const std::vector<unsigned int> indices,
            const std::vector<VertexFeature> features = { VertexFeature::Position }
        );

        Object(
            const std::string& obj_file_path,
            const std::vector<VertexFeature> features = { VertexFeature::Position, VertexFeature::Normal, VertexFeature::UV },
            float scaleOnLoad = 1.0f
        );

        void setTexture(const Texture& texture);
        void setShader(const ShaderProgram& shader);

        void rotate(float angle, glm::vec3 axis);

        void setTranslation(glm::vec3 translation);
        void translate(glm::vec3 translation);
        void scale(float factor);

        void render(Camera& camera);
        void free();
    private:
        void init(
            const std::vector<float> vertices,
            const std::vector<unsigned int> indices,
            const std::vector<VertexFeature> features
        );

        glm::mat4 transformation;
        bool textureEnabled;
        const Texture* texture;
        const ShaderProgram* shader;

        // TODO: remove this member later again, right now its just for hacky purposes
        float currentScale;

        unsigned int indexCount;
        unsigned int VAO, VBO, IBO;
};

#endif