#ifndef OBJECTLOADER_HPP
#define OBJECTLOADER_HPP
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

struct ObjectFace {
    std::string materialIdentifier;
    std::vector<struct ObjectVertex> vertices;
};

struct ObjectVertex {
    unsigned short vertexIndex;
    unsigned short uvIndex;
    unsigned short normalIndex;
};

class ObjectLoader {
    public:
        ObjectLoader(const std::string path);

        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> vertices;

        std::vector<struct ObjectFace> faces;

        friend class Object;
    private:
        unsigned int translateObjectFileIndexToVectorIndex(unsigned int index);
        std::vector<struct ObjMaterial> readMaterials(const std::string path);
        struct ObjectFace parseFace(const std::vector<std::string>& face_vertices);

        std::vector<struct ObjMaterial> materials;

        std::string path;
};

#endif