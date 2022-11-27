#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec4 vertexColor;

uniform mat4 transform;

void main()
{
    gl_Position = vec4(aPos.xyz, 1.0) * transform;
    vertexColor = vec4(aUV, 0.0, 1.0);
}