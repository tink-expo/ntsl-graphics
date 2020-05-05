#version 410 core

uniform mat4 cameraTransform;
uniform mat4 projectionMatrix;

uniform isampler2D edgeTableTex;
uniform isampler2D triTableTex;
uniform float gridSize;
uniform vec3 cubeDiffs;

layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 fcolor;

float offsetSphereFunction(vec3 pos)
{
	float offset = gridSize * sqrt(3.0f);
	float radius = 1.0f;
	return length(pos) - (offset + radius);
}

int getCubeIndex(vec3 pos)
{
	int index = 0;
	vec3 pos_base = pos - vec3(gridSize / 2.0f);
	for (int i = 0; i < 8; ++i) {
		if (offsetSphereFunction(pos_base + cubeDiffs[i]) < 0.0f) {
			index = index | (1 << i);
		}
	}
	return index;
}

int edgeTableValue(int i)
{
	return texelFetch(edgeTableTex, ivec2(i, 0), 0).r;
}

int triTableValue(int i, int j)
{
	return texelFetch(triTableTex, ivec2(j, i), 0).r;
}

void main()
{
	vec4 pos = gl_in[0].gl_Position;

	int cube_index = getCubeIndex(pos);
	if (edgeTableValue(cube_index) == 0) {
		return;
	}

	vec3 vertices[12];

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