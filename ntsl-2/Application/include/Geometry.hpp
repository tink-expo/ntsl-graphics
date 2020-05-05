#pragma once

#include <vector>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Mesh.hpp>

class Geometry
{
private:
protected:
	Geometry *parent;
	std::vector<Geometry *> children;
public:
	Geometry();
	~Geometry();
	glm::vec3 ComputeNormal(glm::vec4 a_a, glm::vec4 a_b, glm::vec4 a_c);
	void GenerateSphere(Engine::Mesh* mesh, float radius);
	void GenerateCone(Engine::Mesh* mesh, float radius, float height);
	void GenerateWing(Engine::Mesh* mesh, float radius, float height);
	void GenerateGradientRectangle(Engine::Mesh* mesh);
	void GenerateAbigRectangle(Engine::Mesh* mesh, float x, float y);
	void GenerateGrid(Engine::Mesh* mesh);

	void AddChild(Geometry *geometry);
};

