#include "Util.hpp"

std::string Util::readFileToString(const std::string& filename) {
    std::ifstream file;
    std::stringstream ss;

    file.open(filename);
    if(file.is_open()) {
        ss << file.rdbuf();
        file.close();
    } else {
        std::cerr <<"Unable to open file: "<< filename << std::endl;
    }
    
    return ss.str();
}

void Util::printGLError() {
    std::cerr <<"OpenGL error: 0x"<< std::hex << glGetError() << std::endl;
}