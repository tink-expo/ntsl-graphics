#version 410 core

in vec3 fcolor;
out vec4 output_color;

void main() 
{
    output_color = vec4(fcolor, 1);
}