#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec4 vertexColor;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos.xyz, 1.0);
    vertexColor = vec4(abs(aNormal), 1.0);
}