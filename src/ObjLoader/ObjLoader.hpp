#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

#include "../Util/Util.hpp"

struct ObjMaterial {
    std::string name;
    float Ns;
    float Ni;
    float d;
    glm::vec3 Tf;
    uint8_t illum;
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    // TODO: Map_Kd
};

struct ObjFace {
    std::string materialIdentifier;
    std::vector<struct ObjVertex> vertices;
};

struct ObjVertex {
    unsigned short vertexIndex;
    unsigned short uvIndex;
    unsigned short normalIndex;
};

class ObjLoader {
    public:
        ObjLoader(const std::string path);

        friend class Object;
    private:
        std::vector<struct ObjMaterial> readMaterials(const std::string path);
        struct ObjFace parseFace(const std::vector<std::string>& face_vertices);

        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> vertices;
        std::vector<struct ObjMaterial> materials;
        std::vector<struct ObjFace> faces;

        std::string path;
};

#endif