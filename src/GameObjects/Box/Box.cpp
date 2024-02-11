#include "Box.hpp"

Box::Box(float len, Texture texture) : Object({
    // front face
    -len, -len, len, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    len, -len, len, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -len, len, len, -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    len, len, len, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f,

    // right face
    len, -len, len, 0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
    len, len, len, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    len, -len, -len, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    len, len, -len, 0.5f, 0.5f, -0.5f, 1.0f, 0.0f,

    // back face
    -len, -len, -len, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    len, -len, -len, 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    -len, len, -len, -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    len, len, -len, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f,

    // left face
    -len, -len, len, -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
    -len, -len, -len, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    -len, len, len, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -len, len, -len, -0.5f, 0.5f, -0.5f, 1.0f, 0.0f,

    // top face
    -len, len, len, -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    len, len, len, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -len, len, -len, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
    len, len, -len, 0.5f, 0.5f, -0.5f, 1.0f, 0.0f,

    // bottom face
    -len, -len, len, -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
    len, -len, len, 0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
    -len, -len, -len, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    len, -len, -len, 0.5f, -0.5f, -0.5f, 1.0f, 0.0f
}, {
    // front face
    0, 1, 2,
    1, 2, 3,

    // right face
    4, 5, 6,
    5, 6, 7,

    // back face
    8, 9, 10,
    9, 10, 11,

    // left face
    12, 13, 14,
    13, 14, 15,

    // top face
    16, 17, 18,
    17, 18, 19,

    // bottom face
    20, 21, 22,
    21, 22, 23
}, { VertexFeature::Position, VertexFeature::Normal, VertexFeature::UV }) {
    setTexture(texture);
    translate(glm::vec3(0.5f, 0.5f, 0.5f));
}