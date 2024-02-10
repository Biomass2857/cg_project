#include "TextureAtlas.hpp"

TextureAtlas::TextureAtlas(
    const std::string& path,
    const std::vector<TextureConfiguration>& configuration
) {
    stbi_set_flip_vertically_on_load(true);
    textureAtlasBytes = stbi_load(path.c_str(), &width, &height, &channels, 0);

    for(const TextureConfiguration& config : configuration) {
        uint8_t* data = new uint8_t[config.width * config.height * channels];
        for(int y = 0; y < config.height; y++) {
            int atlasStartY = config.startY + y;
            int dataStart = (atlasStartY * width + config.startX) * channels;
            int dataLen = config.width * channels;

            std::memcpy(data, textureAtlasBytes + dataStart, dataLen);
        }

        textures.push_back(Texture(data, config.width, config.height, this->channels));
    }
}

void TextureAtlas::free() {
    for(Texture& texture : textures) {
        texture.free();
    }

    stbi_image_free((void*) textureAtlasBytes);
}