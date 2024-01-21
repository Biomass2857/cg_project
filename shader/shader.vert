#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform mat4 rotZMatrix;
uniform mat4 rotXMatrix;
uniform mat4 rotYMatrix;
out vec3 color;
void main()
{
    color = aColor;
    gl_Position = rotYMatrix * rotXMatrix * rotZMatrix * vec4(aPos, 1.0);
}