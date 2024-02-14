#include "Object.hpp"

unsigned short getVertexFeatureSize(VertexFeature feature) {
    switch(feature) {
        case VertexFeature::Position:
            return 3;
        case VertexFeature::Normal:
            return 3;
        case VertexFeature::Color:
            return 3;
        case VertexFeature::UV:
            return 2;
    }

    return 0;
}

Object::Object(
    const std::vector<float> vertices,
    const std::vector<unsigned int> indices,
    const std::vector<VertexFeature> features
) {
    init(vertices, indices, features);
}

Object::Object(
    const std::string& objFilePath,
    const std::vector<VertexFeature> features,
    float scaleOnLoad
) {
    ObjLoader loader(objFilePath);

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    bool positionEnabled = std::find(features.begin(), features.end(), VertexFeature::Position) != features.end();
    bool normalEnabled = std::find(features.begin(), features.end(), VertexFeature::Normal) != features.end();
    bool colorEnabled = std::find(features.begin(), features.end(), VertexFeature::Color) != features.end();
    bool uvEnabled = std::find(features.begin(), features.end(), VertexFeature::UV) != features.end();

    for(struct ObjFace face : loader.faces) {
        for(struct ObjVertex vertex : face.vertices) {
            if(positionEnabled) {
                vertices.push_back(scaleOnLoad * loader.vertices[vertex.vertexIndex].x);
                vertices.push_back(scaleOnLoad * loader.vertices[vertex.vertexIndex].y);
                vertices.push_back(scaleOnLoad * loader.vertices[vertex.vertexIndex].z);
            }

            if(normalEnabled) {
                vertices.push_back(loader.normals[vertex.normalIndex].x);
                vertices.push_back(loader.normals[vertex.normalIndex].y);
                vertices.push_back(loader.normals[vertex.normalIndex].z);
            }

            // TODO: use correct color
            if(colorEnabled) {
                vertices.push_back(loader.normals[vertex.normalIndex].x);
                vertices.push_back(loader.normals[vertex.normalIndex].y);
                vertices.push_back(loader.normals[vertex.normalIndex].z);
            }

            if(uvEnabled) {
                vertices.push_back(loader.uvs[vertex.uvIndex].x);
                vertices.push_back(loader.uvs[vertex.uvIndex].y);
            }

            indices.push_back(indices.size());
        }
    }

    init(vertices, indices, features);
}

void Object::init(
    const std::vector<float> vertices,
    const std::vector<unsigned int> indices,
    const std::vector<VertexFeature> features
) {
    this->rotation = glm::mat4(1.0f);
    this->scaling = glm::mat4(1.0f);
    this->translation = glm::mat4(1.0f);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    unsigned short stride = 0;
    for(VertexFeature feature : features) {
        stride += getVertexFeatureSize(feature);
    }

    unsigned short initialOffset = 0;
    for(VertexFeature feature : features) {
        unsigned short size = getVertexFeatureSize(feature);
        glEnableVertexAttribArray((int) feature);
        glVertexAttribPointer((int) feature, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(initialOffset * sizeof(float)));
        initialOffset += size;
    }

    indexCount = indices.size();
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    textureEnabled = std::find(features.begin(), features.end(), VertexFeature::UV) != features.end();
}

void Object::setTexture(const Texture& texture) {
    this->texture = &texture;
}

void Object::setShader(const ShaderProgram& shader) {
    this->shader = &shader;
}

void Object::setRotation(float angle, glm::vec3 axis) {
    rotation = glm::rotate(glm::mat4(1.0f), angle, axis);
}

void Object::rotate(float angle, glm::vec3 axis) {
    rotation = glm::rotate(rotation, angle, axis);
}

void Object::setTranslation(glm::vec3 translation) {
    this->translation = glm::translate(glm::mat4(1.0f), translation);
}

void Object::translate(glm::vec3 translation) {
    this->translation = glm::translate(this->translation, translation);
}

void Object::scale(float factor) {
    this->scaling = glm::scale(this->scaling, glm::vec3(factor));

    // bug source
    // normalize w component
    for(int i = 0; i < 3; i++) {
        scaling[i][3] /= factor;
    }
}

void Object::render(Camera& camera) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    if(textureEnabled) {
        texture->bind();
        shader->setTexture("tex0", *texture);
    }

    shader->use();

    shader->setMatrix4("camMatrix", camera.getMatrix());
    shader->setMatrix4("modelview", translation * rotation * scaling);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::free() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}