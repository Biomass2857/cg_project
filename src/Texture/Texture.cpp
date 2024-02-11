#include "Texture.hpp"

GLint nextSlot() {
    static GLint __used_slots = 0;
    std::cout << "GL_TEXTURE" << __used_slots <<" used"<< std::endl;
    return __used_slots++;
}

Texture::Texture(const uint8_t* data, unsigned int width, unsigned int height, unsigned short channels) {
    this->data = data;
    this->width = width;
    this->height = height;
    this->channels = channels;
    this->slot = nextSlot();

    init();
}

void Texture::init() {
    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 /*+ slot*/);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::free() {
    glDeleteTextures(1, &ID);
    delete [] data;
}