#version 410 core
 
layout(vertices = 3) out;
 
void main(void)
{
    gl_TessLevelOuter[0] = 2.0;
    gl_TessLevelOuter[1] = 2.0;
    gl_TessLevelOuter[2] = 2.0;

    gl_TessLevelInner[0] = 2.0;
    gl_TessLevelInner[1] = 2.0;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}