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
#include "../ShaderProgram/ShaderProgram.hpp"
#include "../Texture/Texture.hpp"
#include "../Camera/Camera.hpp"
#include "../VertexFeature/VertexFeature.hpp"
#include "../ObjectTemplate/ObjectTemplate.hpp"

class Object {
    public:
        Object(
            const std::vector<float> vertices,
            const std::vector<unsigned int> indices,
            const std::vector<VertexFeature> features = { VertexFeature::Position }
        );

        Object(const ObjectTemplate& objectTemplate);

        void setTexture(const Texture& texture);
        void setShader(const ShaderProgram& shader);

        void setRotation(float angle, glm::vec3 axis);
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

        glm::mat4 scaling;
        glm::mat4 rotation;
        glm::mat4 translation;

        bool textureEnabled;
        const Texture* texture;
        const ShaderProgram* shader;

        unsigned int indexCount;
        unsigned int VAO, VBO, IBO;
};

#endif