#include "Object.hpp"

Object::Object(
    const std::vector<float> vertices,
    const std::vector<unsigned int> indices
) {
    init(vertices, indices);
}

Object::Object(const std::string& obj_file_path) {
    ObjLoader loader(obj_file_path);

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for(struct ObjFace face : loader.faces) {
        for(struct ObjVertex vertex : face.vertices) {
            vertices.push_back(loader.vertices[vertex.vertexIndex].x);
            vertices.push_back(loader.vertices[vertex.vertexIndex].y);
            vertices.push_back(loader.vertices[vertex.vertexIndex].z);

            // use normals as color for now
            vertices.push_back(loader.normals[vertex.normalIndex].x);
            vertices.push_back(loader.normals[vertex.normalIndex].y);
            vertices.push_back(loader.normals[vertex.normalIndex].z);

            vertices.push_back(loader.normals[vertex.normalIndex].x);
            vertices.push_back(loader.normals[vertex.normalIndex].y);
            vertices.push_back(loader.normals[vertex.normalIndex].z);

            // Add later
            // vertices.push_back(loader.uvs[vertex.uvIndex].x);
            // vertices.push_back(loader.uvs[vertex.uvIndex].y);

            indices.push_back(indices.size());
        }
    }

    init(vertices, indices);
}

void Object::init(
    const std::vector<float> vertices,
    const std::vector<unsigned int> indices
) {
    this->transformation = glm::mat4(1.0f);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);

    // colors
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));

    // normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));

    indexCount = indices.size();
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void Object::setShader(const ShaderProgram& shader) {
    this->shader = &shader;
}

void Object::scale(float factor) {
    transformation = glm::scale(transformation, glm::vec3(factor));
}

void Object::render() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    shader->setMatrix4("modelview", transformation);
    shader->use();

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