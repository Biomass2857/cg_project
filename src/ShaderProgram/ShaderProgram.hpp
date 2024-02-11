#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Shader/Shader.hpp"
#include "../Texture/Texture.hpp"

class ShaderProgram {
    public:
        ShaderProgram(std::vector<Shader> shaders);

        void add(Shader shader);
        void use() const;
        void free();

        void setFloat(const std::string &name, float value) const;
        void setMatrix4(const std::string &name, const glm::mat4 matrix) const;
        void setTexture(const std::string &name, const Texture& texture) const;
    private:
        void checkLinked() const;

        unsigned int ID;
};

#endif