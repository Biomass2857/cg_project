#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float theta;
uniform float phi;
uniform float gamma;
out vec3 color;

mat4 generateXRotationMatrix(float angle) {
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, cosAngle, -sinAngle, 0.0,
        0.0, sinAngle, cosAngle, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 generateYRotationMatrix(float angle) {
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    return mat4(
        cosAngle, 0.0, sinAngle, 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sinAngle, 0.0, cosAngle, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 generateZRotationMatrix(float angle) {
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    return mat4(
        cosAngle, -sinAngle, 0.0, 0.0,
        sinAngle, cosAngle, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

void main() {
    color = aColor;
    mat4 rotXMatrix = generateXRotationMatrix(theta);
    mat4 rotYMatrix = generateYRotationMatrix(phi);
    mat4 rotZMatrix = generateZRotationMatrix(gamma);
    gl_Position = rotYMatrix * rotXMatrix * rotZMatrix * vec4(aPos, 1.0);
}