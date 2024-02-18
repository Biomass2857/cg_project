#include "TextureAtlas.hpp"

TextureAtlas::TextureAtlas(
    const std::string& path,
    const std::vector<TextureConfiguration>& configuration
) {
    stbi_set_flip_vertically_on_load(true);
    int width = 0, height = 0, channels = 0;
    const uint8_t* textureAtlasBytes = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if(width <= 0 || height <= 0 || channels <= 0) {
        std::cerr <<"Failed to load texture atlas: " << path << std::endl;
    }

    for(const TextureConfiguration& config : configuration) {
        uint8_t* data = new uint8_t[config.width * config.height * channels];
        for(int y = 0; y < config.height; y++) {
            int atlasStartY = config.startY + y;
            int dataStart = (atlasStartY * width + config.startX) * channels;
            int dataLen = config.width * channels;
            int dataOffset = y * config.width * channels;

            std::memcpy(data + dataOffset, textureAtlasBytes + dataStart, dataLen);
        }

        Texture texture = Texture(data, config.width, config.height, channels);
        textures.insert(std::pair<std::string, Texture>(config.name, texture));
    }

    stbi_image_free((void*) textureAtlasBytes);
}

Texture TextureAtlas::getTexture(const std::string& name) const {
    return textures.at(name);
}

void TextureAtlas::free() {
    for(auto& pair : textures) {
        pair.second.free();
    }
}