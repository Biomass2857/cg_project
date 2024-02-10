#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 vNormal;

uniform mat4 modelview;

uniform float u_time;

out vec3 normal;
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
    mat4 rotXMatrix = generateXRotationMatrix(u_time*10.0);
    mat4 rotYMatrix = generateYRotationMatrix(u_time*7.5);
    mat4 rotZMatrix = generateZRotationMatrix(u_time*12.5);
    mat4 totalRotation = rotYMatrix * rotXMatrix * rotZMatrix;
    normal = mat3(totalRotation) * vNormal;
    gl_Position = totalRotation * modelview * vec4(aPos, 1.0);
}