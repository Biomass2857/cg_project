#include "Texture.hpp"

Texture::Texture(const uint8_t* data, unsigned int width, unsigned int height, unsigned short channels) {
    this->data = data;
    this->width = width;
    this->height = height;
    this->channels = channels;

    init();
}

void Texture::init() {
    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    std::cout <<"Data: "<< std::endl;
    for(int i = 0; i < 50; i++) {
        std::cout << std::hex << (unsigned short) data[i] << " ";
    }
    std::cout << std::endl;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::getID() const {
    return ID;
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::free() {
    glDeleteTextures(1, &ID);
    delete [] data;
}