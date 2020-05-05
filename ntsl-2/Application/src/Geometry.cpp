#include <Geometry.hpp>
#include <stdlib.h>
#include <vector>
#include <cstdio>
#include <cmath>
using namespace std;
#define PI 3.14159265358979323846

Geometry::Geometry()
{
}


Geometry::~Geometry()
{
}


glm::vec3 Geometry::ComputeNormal(glm::vec4 a_a, glm::vec4 a_b, glm::vec4 a_c)
{
    glm::vec3 normal = glm::normalize(glm::cross(a_b.xyz() - a_a.xyz(), a_c.xyz() - a_a.xyz()));
    return normal;
}

void Geometry::GenerateGrid(Engine::Mesh* mesh, 
        const glm::vec3& region_small, const glm::vec3& region_big, float grid_size)
{
    mesh->AddAttribute(4);

    int num_elems = 0;

    for (float pos_x = region_small.x; pos_x <= region_big.x; pos_x += grid_size) {
        for (float pos_y = region_small.y; pos_y <= region_big.y; pos_y += grid_size) {
            for (float pos_z = region_small.z; pos_z <= region_big.z; pos_z += grid_size) {
                mesh->AddVertexData(glm::vec4(pos_x, pos_y, pos_z, 1.0));

                ++num_elems;
            }
        }
    }

    mesh->SetNumElements(num_elems);
	mesh->CreateMesh();
    std::cout << num_elems << std::endl;

    return;
}