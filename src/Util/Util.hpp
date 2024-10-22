#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#elif __linux__
#include <GL/gl.h>
#elif _WIN32
#include <GL/gl.h>
#endif

namespace Util {
    std::string readFileToString(const std::string& filename);

    std::vector<std::string> split(const std::string& s, char delimiter);

    template<typename T>
    void printVector(const std::vector<T>& vec);

    void printGLError();
}

#endif