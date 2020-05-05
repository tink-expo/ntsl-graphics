#version 410 core

uniform mat4 cameraTransform;
uniform mat4 projectionMatrix;

uniform isampler2D edgeTableTex;
uniform isampler2D triTableTex;
uniform float gridSize;
uniform vec3 cubeDiffs[8];

layout (points) in;
layout (triangle_strip, max_vertices = 15) out;

out vec3 fcolor;

mat3 rrt(vec3 r)
{
	return mat3(r.x * r, r.y * r, r.z * r);
}

vec3 SimpleBrush(vec3 pos, vec3 center, vec3 force)
{
	vec3 diff = pos - center;
	float factor = max(dot(diff, force), 0.0);
	return pos + factor * force;
}

float offsetSphereFunction(vec3 pos)
{
	// vec3 deformed_pos = kelvinletsBrush(pos, vec3(1, 1, 0));
	// vec3 deformed_pos = pos + vec3(1, 1, 0);
	float offset = gridSize * sqrt(3.0);
	float radius = 10.0;
	return length(pos) - (radius + offset);
}

int getCubeIndex(vec3 pos)
{
	int cube_index = 0;
	vec3 pos_base = pos - vec3(gridSize / 2.0f);
	for (int i = 0; i < 8; ++i) {
		if (offsetSphereFunction(pos_base + cubeDiffs[i]) < 0.0f) {
			cube_index = cube_index | (1 << i);
		}
	}
	return cube_index;
}

int edgeTableValue(int i)
{
	return texelFetch(edgeTableTex, ivec2(i, 0), 0).r;
}

int triTableValue(int i, int j)
{
	return texelFetch(triTableTex, ivec2(j, i), 0).r;
}

vec3 getInterpedVetex(vec3 pos, int diff_i1, int diff_i2)
{
	vec3 pos_base = pos - vec3(gridSize / 2.0f);
	vec3 p1 = pos_base + cubeDiffs[diff_i1];
	vec3 p2 = pos_base + cubeDiffs[diff_i2];
	float l1 = offsetSphereFunction(p1);
	float l2 = offsetSphereFunction(p2);
	return mix(p1, p2, abs(l1) / (abs(l1) + abs(l2)));
}

int cube_diff_indices[12 * 2] = int[](
	0, 1,
	1, 2,
	2, 3,
	3, 0,
	4, 5,
	5, 6,
	6, 7,
	7, 4,
	0, 4,
	1, 5,
	2, 6,
	3, 7
);

void main()
{
	mat4 MVM = inverse(cameraTransform);
	vec4 pos = gl_in[0].gl_Position;
	fcolor = vec3(1.0, 1.0, 0.0);

	int cube_index = getCubeIndex(pos.xyz);
	int edge = edgeTableValue(cube_index);
	if (edge == 0) {
		return;
	}

	vec3 vertices[12];
	for (int i = 0; i < 12; ++i) {
		if ((edge & (1 << i)) != 0) {
			vertices[i] = getInterpedVetex(
					pos.xyz,
					cube_diff_indices[i * 2], 
					cube_diff_indices[i * 2 + 1]);
		}
	}

	for (int i = 0; i < 16 && triTableValue(cube_index, i) != -1; 
			i += 3) {
		for (int j = 0; j < 3; ++j) {
			int tri = triTableValue(cube_index, i + j);
			vec3 hull_vpos = vertices[tri];
			hull_vpos = SimpleBrush(hull_vpos, vec3(0, 0, 0), vec3(1, 1, 0));
			gl_Position = projectionMatrix * (MVM * vec4(hull_vpos, 1));
			EmitVertex();
		}
		EndPrimitive();
	}
}