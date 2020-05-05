#version 410 core

layout(location = 0) in vec4 pos;

out vec4 vpos;

void main()
{
	gl_Position = pos;
	vpos = pos;
}