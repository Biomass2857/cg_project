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
#include "../Util/Util.hpp"

class Texture {
    public:
        Texture(const uint8_t* data, unsigned int width, unsigned int height, unsigned short channels);

        void bind() const;

        void free();

        friend class ShaderProgram;
    private:
        const uint8_t* data;
        void init();

        unsigned short channels;
        unsigned int width, height;

        unsigned int ID;

        GLint slot;
};

#endif