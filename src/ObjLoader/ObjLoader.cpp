#include "ObjLoader.hpp"
#define OBJ_LOADER_VERBOSE true

ObjLoader::ObjLoader(const std::string path) {
    this->path = path;

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
            glm::vec4 vertex;
            s >> vertex.x;
            s >> vertex.y;
            s >> vertex.z;
            vertex.w = 1.0f;

            vertices.push_back(vertex);
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
        } else if(line.substr(0, 6) == "mtllib ") {
            std::string materialPath = line.substr(7);
            std::vector<struct ObjMaterial> lib_materials = readMaterials(materialPath);

            materials.insert(materials.end(), lib_materials.begin(), lib_materials.end());
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
        try {
            obj_vertex.vertexIndex = translateObjectFileIndexToVectorIndex(std::stoi(vertex_data[0]));
        } catch(std::exception e) {
            #ifdef OBJ_LOADER_VERBOSE
            std::cerr << "["<< path <<"] error missing vertex index" << std::endl;
            #endif
        }

        try {
            obj_vertex.uvIndex = translateObjectFileIndexToVectorIndex(std::stoi(vertex_data[1]));
        } catch(std::exception e) {
            #ifdef OBJ_LOADER_VERBOSE
            std::cout << "["<< path <<"] warning uv coordinate missing" << std::endl;
            #endif
        }

        try {
            obj_vertex.normalIndex = translateObjectFileIndexToVectorIndex(std::stoi(vertex_data[2]));
        } catch(std::exception e) {
            #ifdef OBJ_LOADER_VERBOSE
            std::cout << "["<< path <<"] warning normal vector missing" << std::endl;
            #endif
        }

        face.vertices.push_back(obj_vertex);
    }

    return face;
}

unsigned int ObjLoader::translateObjectFileIndexToVectorIndex(unsigned int index) {
    return index - 1;
}

std::vector<struct ObjMaterial> ObjLoader::readMaterials(const std::string path) {
    std::vector<struct ObjMaterial> materials;
    std::ifstream file(path);

    if(!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::string line;
    while(std::getline(file, line)) {
        if(line.substr(0, 6) == "newmtl") {
            struct ObjMaterial material;
            material.name = line.substr(7);
            materials.push_back(material);
        } else if(line.substr(0, 3) == "Ns ") {
            materials.back().Ns = std::stof(line.substr(3));
        } else if(line.substr(0, 3) == "Ni ") {
            materials.back().Ni = std::stof(line.substr(3));
        } else if(line.substr(0, 2) == "d ") {
            materials.back().d = std::stof(line.substr(2));
        } else if(line.substr(0, 3) == "Tf ") {
            std::istringstream s(line.substr(3));
            glm::vec3 Tf;
            s >> Tf.x;
            s >> Tf.y;
            s >> Tf.z;

            materials.back().Tf = Tf;
        } else if(line.substr(0, 7) == "illum ") {
            materials.back().illum = std::stoi(line.substr(7));
        } else if(line.substr(0, 3) == "Ka ") {
            std::istringstream s(line.substr(3));
            glm::vec3 Ka;
            s >> Ka.x;
            s >> Ka.y;
            s >> Ka.z;

            materials.back().Ka = Ka;
        } else if(line.substr(0, 3) == "Kd ") {
            std::istringstream s(line.substr(3));
            glm::vec3 Kd;
            s >> Kd.x;
            s >> Kd.y;
            s >> Kd.z;

            materials.back().Kd = Kd;
        } else if(line.substr(0, 3) == "Ks ") {
            std::istringstream s(line.substr(3));
            glm::vec3 Ks;
            s >> Ks.x;
            s >> Ks.y;
            s >> Ks.z;

            materials.back().Ks = Ks;
        } else {
            #ifdef OBJ_LOADER_VERBOSE
            std::cout <<"[ObjLoader] Ignoring line: "<< line << std::endl;
            #endif
        }
    }
}