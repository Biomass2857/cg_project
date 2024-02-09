#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform mat4 rotZMatrix;
uniform mat4 rotXMatrix;
uniform mat4 rotYMatrix;
out vec3 color;
uniform float u_time;
float theta;
float phi;
float gamma;

void main()
{

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
    gl_Position = rotYMatrix * rotXMatrix * rotZMatrix * vec4(aPos, 1.0);
}