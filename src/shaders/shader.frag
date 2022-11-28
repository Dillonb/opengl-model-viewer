#version 450 core
out vec4 FragColor;

in vec2 uv;

layout (binding = 0) uniform sampler2D sampler;

void main()
{
    FragColor = vec4(texture(sampler, uv).rgb, 1.0);
    //FragColor = vec4(uv, 0.0, 1.0);
}