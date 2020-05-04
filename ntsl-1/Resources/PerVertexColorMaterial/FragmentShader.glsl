#version 330 core

in vec4 frag_color;
out vec4 output_color;

void main()
{
    float gamma = 2.2;
    output_color.rgb = pow(frag_color.rgb, vec3(1.0/gamma));
    output_color.a = frag_color.a;
}