#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

#include "../Util/Util.hpp"

// struct ObjMaterial {

// }

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

    private:
        struct ObjFace parseFace(const std::vector<std::string>& face_vertices);

        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> vertices;
        // std::vector<struct ObjMaterial> materials;
        std::vector<struct ObjFace> faces;
};

#endif