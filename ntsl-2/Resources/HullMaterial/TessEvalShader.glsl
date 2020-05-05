#version 410 core
 
layout(triangles, equal_spacing, ccw) in;
 
//quad interpol
vec4 interpolate(in vec4 v0, in vec4 v1, in vec4 v2)
{
    vec4 a = mix(v0, v1, gl_TessCoord.x);
    vec4 b = mix(v1, v2, gl_TessCoord.x);
    return mix(a, b, gl_TessCoord.y);
}
 
void main()
{ 
    gl_Position = interpolate(
    gl_in[0].gl_Position, 
    gl_in[1].gl_Position, 
    gl_in[2].gl_Position);
}