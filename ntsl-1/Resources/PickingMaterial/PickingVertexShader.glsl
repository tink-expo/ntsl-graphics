#version 330 core

layout(location = 0) in vec4 pos;



uniform mat4 worldTransform;
uniform mat4 cameraTransform;
uniform mat4 projectionMatrix;
uniform vec3 objectID;







void main()
{
	// Output position of the vertex, in clip space : MVP * position
	mat4 MVM = inverse(cameraTransform) * worldTransform;


	vec4 wPosition = MVM * vec4(pos);
	gl_Position = projectionMatrix * wPosition;
	


}