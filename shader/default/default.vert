#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 vNormal;

uniform mat4 modelview;

uniform float u_time;

out vec3 normal;
out vec3 color;

uniform mat4 camMatrix;

void main() {
    color = aColor;
    gl_Position = camMatrix * modelview * vec4(aPos, 1.0);
}