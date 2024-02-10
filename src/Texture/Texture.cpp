#include "Texture.hpp"

Texture::Texture(const uint8_t* data, unsigned int width, unsigned int height, unsigned short channels) {
    this->data = data;
    this->width = width;
    this->height = height;
    this->channels = channels;
}

void Texture::free() {
    delete [] data;
}