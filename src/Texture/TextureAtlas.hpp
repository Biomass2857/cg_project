#ifndef TEXTUREATLAS_HPP
#define TEXTUREATLAS_HPP

#include <string>
#include <cstring>
#include <unordered_map>

#include "Texture.hpp"

#include "../stb/stb_image.h"

struct TextureConfiguration {
    std::string name;
    int startX;
    int startY;
    int width;
    int height;

    TextureConfiguration(
        const std::string& name,
        int startX,
        int startY,
        int width,
        int height
    ) : name(name), startX(startX), startY(startY), width(width), height(height) {}
};

class TextureAtlas {
    public:
        TextureAtlas(
            const std::string& path,
            const std::vector<TextureConfiguration>& configuration
        );

        Texture getTexture(const std::string& name) const;

        void free();
    private:
        const uint8_t* textureAtlasBytes;

        int channels;
        int width, height;
        std::unordered_map<std::string, Texture> textures;
};

#endif