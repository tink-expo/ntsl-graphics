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

void Geometry::GenerateSphere(Engine::Mesh* mesh, float radius)
{
    const int thetaCount = 120;
    const int phiCount = 60;
    
    mesh->AddAttribute(4);
    mesh->AddAttribute(4);

    vector<glm::vec4> vertices;
    vector<glm::vec4> normals;

    float x, y, z, xy;
    float nx, ny, nz, normalizer = 1.0f / radius;

    float thetaStep = 2 * (float)PI / thetaCount;
    float phiStep = (float)PI / phiCount;
    float theta, phi;

    for (int i=0; i<=phiCount; ++i) {
        phi = (float)PI / 2 - i * phiStep;
        xy = radius * cosf(phi);
        z = radius * sinf(phi);

        for (int j=0; j<=thetaCount; ++j) {
            theta = j * thetaStep;
            x = xy * cosf(theta);
            y = xy * sinf(theta);
            vertices.push_back(glm::vec4(x, y, z, 1.0f));

            nx = x * normalizer;
            ny = y * normalizer;
            nz = z * normalizer;
            normals.push_back(glm::vec4(nx, ny, nz, 1.0f));
        }
    }

    int numElements = 0;
    int t1, t2;
    for (int i=0; i<phiCount; ++i) {
        t1 = i * (thetaCount + 1);
        t2 = t1 + thetaCount + 1;

        for (int j=0; j<thetaCount; ++j, ++t1, ++t2) {
            if (i > 0) {
                mesh->AddVertexData(vertices[t1]);
                mesh->AddVertexData(normals[t1]);

                mesh->AddVertexData(vertices[t2]);
                mesh->AddVertexData(normals[t2]);

                mesh->AddVertexData(vertices[t1+1]);
                mesh->AddVertexData(normals[t1+1]);

                numElements += 3;
            }

            if (i < phiCount-1) {
                mesh->AddVertexData(vertices[t1+1]);
                mesh->AddVertexData(normals[t1+1]);

                mesh->AddVertexData(vertices[t2]);
                mesh->AddVertexData(normals[t2]);

                mesh->AddVertexData(vertices[t2+1]);
                mesh->AddVertexData(normals[t2+1]);

                numElements += 3;
            }
        }
    }

    mesh->SetNumElements(numElements);
    mesh->CreateMesh();

    return;
}

/* cone center at (0.0f, 0.0f, 0.0f), pointing toward +z */
void Geometry::GenerateCone(Engine::Mesh* mesh, float radius, float height)
{
    const int thetaCount = 120;

    mesh->AddAttribute(4);
    mesh->AddAttribute(4);

    glm::vec4 topVertex = glm::vec4(0.0f, 0.0f, height, 1.0f);
    vector<glm::vec4> baseVertices;

    float x, y;
    float thetaStep = 2 * (float)PI / thetaCount;
    float theta;

    for (int i=0; i<=thetaCount; ++i) {
        theta = i * thetaStep;
        x = radius * cosf(theta);
        y = radius * sinf(theta);
        baseVertices.push_back(glm::vec4(x, y, 0, 1.0f));
    }

    int numElements = 0;
    for (int i=0; i<thetaCount; ++i) {
        int nextI = i < thetaCount-1 ? i+1 : 0;
        glm::vec4 n = glm::vec4(ComputeNormal(baseVertices[i], baseVertices[nextI], topVertex), 0.0f);

        mesh->AddVertexData(baseVertices[i]);
        mesh->AddVertexData(n);
        mesh->AddVertexData(baseVertices[nextI]);
        mesh->AddVertexData(n);
        mesh->AddVertexData(topVertex);
        mesh->AddVertexData(n);

        numElements += 3;
    }

    mesh->SetNumElements(numElements);
    mesh->CreateMesh();

    return;
}

/* semi-circle cylinder - center (radius, 0.0f, +-height/2) */
void Geometry::GenerateWing(Engine::Mesh* mesh, float radius, float height)
{
    const int thetaCount = 120;

    mesh->AddAttribute(4);
    mesh->AddAttribute(4);

    glm::vec4 baseCenterVertex = glm::vec4(radius, -height / 2, 0.0f, 1.0f);
    vector<glm::vec4> baseVertices;

    float x, z;
    float thetaStep = (float)PI / thetaCount;
    float theta;

    for (int i=0; i<=thetaCount; ++i) {
        theta = i * thetaStep;
        x = radius + radius * cosf(theta);
        z = radius * sinf(theta);
        baseVertices.push_back(glm::vec4(x, 0.0f, z, 1.0f));
    }

    glm::vec4 upNormal = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    glm::vec4 downNormal = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
    glm::vec4 topCenterVertex = baseCenterVertex; topCenterVertex.y += height;

    int numElements = 0;
    for (int i=0; i<thetaCount; ++i) {
        int nextI = i < thetaCount-1 ? i+1 : 0;

        mesh->AddVertexData(baseVertices[i]);
        mesh->AddVertexData(downNormal);
        mesh->AddVertexData(baseVertices[nextI]);
        mesh->AddVertexData(downNormal);
        mesh->AddVertexData(baseCenterVertex);
        mesh->AddVertexData(downNormal);

        glm::vec4 topVertexI = baseVertices[i]; topVertexI.y += height;
        glm::vec4 topVertexNextI = baseVertices[nextI]; topVertexNextI.y += height;
        mesh->AddVertexData(topVertexNextI);
        mesh->AddVertexData(upNormal);
        mesh->AddVertexData(topVertexI);
        mesh->AddVertexData(upNormal);
        mesh->AddVertexData(topCenterVertex);
        mesh->AddVertexData(upNormal);

        glm::vec4 n = glm::vec4(ComputeNormal(baseVertices[nextI], baseVertices[i], topVertexI), 0.0f);
        mesh->AddVertexData(baseVertices[nextI]);
        mesh->AddVertexData(n);
        mesh->AddVertexData(baseVertices[i]);
        mesh->AddVertexData(n);
        mesh->AddVertexData(topVertexI);
        mesh->AddVertexData(n);

        mesh->AddVertexData(baseVertices[nextI]);
        mesh->AddVertexData(n);
        mesh->AddVertexData(topVertexI);
        mesh->AddVertexData(n);
        mesh->AddVertexData(topVertexNextI);
        mesh->AddVertexData(n);

        numElements += 12;
    }

    mesh->SetNumElements(numElements);
    mesh->CreateMesh();

    return;
}

void Geometry::GenerateGradientRectangle(Engine::Mesh* mesh)
{
    mesh->AddAttribute(4);
    mesh->AddAttribute(4);

	float x = 15.0f;
	float y = 10.0f;
	float z = 5.0f;
    glm::vec4 p1 = glm::vec4(x, y, -z, 1.0f);
    glm::vec4 p2 = glm::vec4(-x, y, -z, 1.0f);
    glm::vec4 p3 = glm::vec4(-x, -y, -z, 1.0f);
    glm::vec4 p4 = glm::vec4(x, -y, -z, 1.0f);

    glm::vec4 light_gray = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    glm::vec4 dark_blue = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);


	vector<glm::vec4> positions = { p1, p2, p3, p1, p3, p4 };
	vector<bool> colors = { true, true, false, true, false, false };

	for (int i = 0; i < 6; ++i) {
		mesh->AddVertexData(positions[i]);
		mesh->AddVertexData(colors[i] ? dark_blue : light_gray);
	}

	mesh->SetNumElements(6);
	mesh->CreateMesh();

    return;
}

void Geometry::GenerateAbigRectangle(Engine::Mesh* mesh, float x, float y)
{
    mesh->AddAttribute(2);
    
    glm::vec2 p1(x, y);
    glm::vec2 p2(-x, y);
    glm::vec2 p3(-x, -y);
    glm::vec2 p4(x, -y);


	vector<glm::vec2> positions = { p1, p2, p3, p1, p3, p4 };

	for (int i = 0; i < 6; ++i) {
		mesh->AddVertexData(positions[i]);
	}

	mesh->SetNumElements(6);
	mesh->CreateMesh();

    return;
}

void Geometry::GenerateGrid(Engine::Mesh* mesh)
{
    mesh->AddAttribute(4);
    
    glm::vec4 p1(0.5, 0.5, -0.5, 1.0);
    glm::vec4 p2(0.0, 0.5, -0.5, 1.0);
    glm::vec4 p3(0.0, 0.0, -0.5, 1.0);


	vector<glm::vec4> positions = { p1, p2, p3 };

	for (const auto& pos : positions) {
		mesh->AddVertexData(pos);
	}
    for (const auto& pos : positions) {
		mesh->AddVertexData(pos - glm::vec4(0.5, 0.5, 0.0, 0.0));
	}

	mesh->SetNumElements(6);
	mesh->CreateMesh();

    return;
}