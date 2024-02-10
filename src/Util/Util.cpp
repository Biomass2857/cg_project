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

template<typename T>
void Util::printVector(const std::vector<T>& vec) {
    for(auto& v : vec) {
        std::cout << v <<" ";
    }
    std::cout << std::endl;
}

template void Util::printVector<float>(const std::vector<float>& vec);
template void Util::printVector<unsigned short>(const std::vector<unsigned short>& vec);

void Util::printGLError() {
    std::cerr <<"OpenGL error: 0x"<< std::hex << glGetError() << std::endl;
}