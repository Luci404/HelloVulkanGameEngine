#version 450

layout (location = 0) in vec3 inColor;

layout (location = 0) out vec3 outColor;

layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

void main()
{
    outColor = inColor;
}