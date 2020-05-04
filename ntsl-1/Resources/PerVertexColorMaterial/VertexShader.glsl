#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 color;

uniform mat4 worldTransform;
uniform mat4 cameraTransform;
uniform mat4 projectionMatrix;

out vec4 frag_color;

void main()
{
    gl_Position = projectionMatrix * inverse(cameraTransform) * worldTransform * pos;
    frag_color = color;
}