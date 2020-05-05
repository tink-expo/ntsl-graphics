#version 410 core

uniform mat4 cameraTransform;
uniform mat4 projectionMatrix;

layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 fcolor;

void main()
{
	vec4 pos = gl_in[0].gl_Position;

	fcolor = vec3(1.0, 0.0, 0.0);
	if (pos.y > 0 && pos.z > 0) {
		fcolor.y = 1.0;
	}
	if (pos.y < 0 && pos.z < 0) {
		fcolor.z = 1.0;
	}
	if (pos.y > 0 && pos.z < 0) {
		fcolor.x = 0;
		fcolor.y = 1;
	}

	mat4 MVM = inverse(cameraTransform);

	vec4 offs[3] = vec4[](
		vec4(0.0, 0.0, 0.0, 0.0),
		vec4(0.2, 0.0, 0.0, 0.0),
		vec4(0.0, 0.1, 0.0, 0.0)
	);

	for (int i = 0; i < offs.length(); ++i) {
		vec4 w_pos = MVM * (pos + offs[i]);
		gl_Position = projectionMatrix * w_pos;
		EmitVertex();
	}
	EndPrimitive();
}