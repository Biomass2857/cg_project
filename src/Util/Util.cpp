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

std::vector<std::string> Util::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while(std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

template<typename T>
void Util::printVector(const std::vector<T>& vec) {
    for(auto& v : vec) {
        std::cout << v <<" ";
    }
    std::cout << std::endl;
}

template void Util::printVector<int>(const std::vector<int>& vec);
template void Util::printVector<std::string>(const std::vector<std::string>& vec);
template void Util::printVector<float>(const std::vector<float>& vec);
template void Util::printVector<unsigned short>(const std::vector<unsigned short>& vec);

void Util::printGLError() {
    std::cerr <<"OpenGL error: 0x"<< std::hex << glGetError() << std::endl;
}