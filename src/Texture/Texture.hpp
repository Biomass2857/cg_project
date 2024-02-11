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

        unsigned int getID() const;

        void bind() const;

        void free();

        friend class ShaderProgram;
    private:
        void init();
        const uint8_t *data;

        unsigned short channels;
        unsigned int width, height;

        unsigned int ID;
};

#endif