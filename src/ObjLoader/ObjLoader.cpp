#include "ObjLoader.hpp"
#define OBJ_LOADER_VERBOSE true

ObjLoader::ObjLoader(const std::string path) {
    std::ifstream file(path);

    if(!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::string line;
    while(std::getline(file, line)) {
        if(line.substr(0, 2) == "f ") {
            std::vector<std::string> face_vertices = Util::split(line.substr(2), ' ');

            struct ObjFace face = parseFace(face_vertices);
            faces.push_back(face);
        } else if(line.substr(0, 2) == "v ") {
            std::istringstream s(line.substr(2));
            glm::vec4 v;
            s >> v.x;
            s >> v.y;
            s >> v.z;
            v.w = 1.0f;

            vertices.push_back(v);
        } else if(line.substr(0, 3) == "vt ") {
            std::istringstream s(line.substr(3));
            glm::vec2 uv;
            s >> uv.x;
            s >> uv.y;

            uvs.push_back(uv);
        } else if(line.substr(0, 3) == "vn ") {
            std::istringstream s(line.substr(3));
            glm::vec3 normal;
            s >> normal.x;
            s >> normal.y;
            s >> normal.z;

            normals.push_back(normal);
        } else {
            #ifdef OBJ_LOADER_VERBOSE
            std::cout <<"[ObjLoader] Ignoring line: "<< line << std::endl;
            #endif
        }
    }
}

struct ObjFace ObjLoader::parseFace(const std::vector<std::string>& face_vertices) {
    struct ObjFace face;
    for(std::string vertexString : face_vertices) {
        std::vector<std::string> vertex_data = Util::split(vertexString, '/');

        struct ObjVertex obj_vertex;
        obj_vertex.vertexIndex = std::stoi(vertex_data[0]);
        obj_vertex.uvIndex = std::stoi(vertex_data[1]);
        obj_vertex.normalIndex = std::stoi(vertex_data[2]);

        face.vertices.push_back(obj_vertex);
    }

    return face;
}