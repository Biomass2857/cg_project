#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#elif __linux__
#include <GL/gl.h>
#elif _WIN32
#include <GL/gl.h>
#endif

#include <stdint.h>

// class Texture {
//     public:
//         Texture(const uint8_t* data, unsigned int width, unsigned int height, unsigned short channels);

//         void free();
//     private:
//         uint8_t *data;

//         unsigned short channels;
//         unsigned int width, height;
// };

#endif