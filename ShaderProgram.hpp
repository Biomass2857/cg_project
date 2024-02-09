#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP
#include <vector>
#include <string>
#include "Shader.hpp"

class ShaderProgram {
    public:
        ShaderProgram(std::vector<Shader> shaders);

        void add(Shader shader);
        void use();
        void free();

        void setFloat(const std::string &name, float value) const;
    private:
        void checkLinked();

        unsigned int ID;
};

#endif