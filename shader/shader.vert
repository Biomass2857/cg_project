#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 vNormal;

uniform float theta;
uniform float phi;
uniform float gamma;

out vec3 normal;
out vec3 color;
uniform float u_time;
float theta;
float phi;
float gamma;


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

        theta   = u_time*0,5;
        phi     = u_time*2;
        gamma   = u_time*4;
        mat4 rotZMatrix = mat4(
            cos(theta), 0.0f, -sin(theta), 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            sin(theta), 0.0f, cos(theta), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        
        mat4 rotXMatrix = mat4(
            cos(phi), -sin(phi), 0.0f, 0.0f,
            sin(phi), cos(phi), 0.0f, 0.0f,
            0.0f, 0.0f, 1, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );

        mat4 rotYMatrix = mat4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cos(gamma), -sin(gamma), 0.0f,
            0.0f, sin(gamma), cos(gamma), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    color = aColor;
    mat4 rotXMatrix = generateXRotationMatrix(theta);
    mat4 rotYMatrix = generateYRotationMatrix(phi);
    mat4 rotZMatrix = generateZRotationMatrix(gamma);
    mat4 totalTransformation = rotYMatrix * rotXMatrix * rotZMatrix;
    normal = mat3(totalTransformation) * vNormal;
    gl_Position = totalTransformation * vec4(aPos, 1.0);
}