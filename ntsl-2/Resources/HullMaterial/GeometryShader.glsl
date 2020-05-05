#version 410 core

uniform mat4 cameraTransform;
uniform mat4 projectionMatrix;

uniform isampler2D edgeTableTex;
uniform isampler2D triTableTex;
uniform int testu;

layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 fcolor;

int edgeTableValue(int i)
{
	return texelFetch(edgeTableTex, ivec2(i, 0), 0).a;
}

int triTableValue(int i, int j)
{
	return texelFetch(triTableTex, ivec2(j, i), 0).a;
}

void main()
{
	vec4 pos = gl_in[0].gl_Position;

	fcolor = vec3(1.0, 0.0, 0.0);

	if (texelFetch(edgeTableTex, ivec2(2, 0), 0) == vec4(0)) {
		fcolor.b = 1.0;
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