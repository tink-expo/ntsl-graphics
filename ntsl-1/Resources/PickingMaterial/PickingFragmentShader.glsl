#version 330 core


out vec3 output_color;
uniform vec3 objectID;

void main()
{

	output_color= objectID;
	
}