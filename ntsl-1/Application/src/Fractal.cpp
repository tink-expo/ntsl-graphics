#include <Fractal.hpp>
#include <cmath>
#include <cstdio>

Fractal::Fractal()
{

}

Fractal::~Fractal()
{

}

glm::vec4 Fractal::divideVectors(const glm::vec4 &v1, const glm::vec4 &v2, int a, int b)
{
    glm::vec4 ret = glm::vec4(0.0f);
    for (int i=0; i<4; ++i) {
        ret[i] = (v1[i] * b + v2[i] * a) / (a + b);
    }
    return ret;
}

void Fractal::makeHorn(Engine::Mesh* mesh, const glm::vec4 &leftVec, const glm::vec4 &rightVec, int level, int& numElements)
{
    if (level > MAX_LEVEL) return;
    glm::vec4 nextLeftVec = divideVectors(leftVec, rightVec, 1, 2);
    glm::vec4 nextRightVec = divideVectors(leftVec, rightVec, 2, 1);
    static glm::mat4 rot60zMat = glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec4 nextTopVec = rot60zMat * (nextRightVec - nextLeftVec) + nextLeftVec;

    /* triangle vertices */
    glm::vec4 vertices[6] = {
        nextLeftVec, nextRightVec, nextTopVec,
        nextRightVec, nextLeftVec, nextTopVec
    };
    for (int i=3; i<6; ++i) {
        vertices[i][2] *= (-1);
    }

    /* normals */
    static glm::vec4 frontNormal = glm::vec4(ComputeNormal(vertices[0], vertices[1], vertices[2]), 0.0f);
    static glm::vec4 backNormal = glm::vec4(ComputeNormal(vertices[3], vertices[4], vertices[5]), 0.0f);
    glm::vec4 leftNormal = glm::vec4(ComputeNormal(vertices[0], vertices[2], vertices[5]), 0.0f);
    glm::vec4 rightNormal = glm::vec4(ComputeNormal(vertices[1], vertices[5], vertices[2]), 0.0f);
    static glm::vec4 belowNormal = glm::vec4(ComputeNormal(vertices[0], vertices[4], vertices[1]), 0.0f);
    glm::vec4 normals[] = {frontNormal, backNormal, leftNormal, rightNormal, belowNormal};
    
    const int NUM_TRIANGLES = 8;
    int vIndices[NUM_TRIANGLES][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {0, 2, 5},
        {4, 0, 5},
        {1, 5, 2},
        {1, 3, 5},
        {0, 4, 1},
        {1, 4, 3}
    };
    int nIdx[NUM_TRIANGLES] = {0, 1, 2, 2, 3, 3, 4, 4};

    for (int i=0; i<8; ++i) {
        for (auto& idx : vIndices[i]) {
            mesh->AddVertexData(vertices[idx]);
            mesh->AddVertexData(normals[i]);
        }
    }
    numElements += NUM_TRIANGLES * 3; // record increased numElements (for setNumElemnts in mesh)

    makeHorn(mesh, nextLeftVec, nextTopVec, level + 1, numElements);
    makeHorn(mesh, nextTopVec, nextRightVec, level + 1, numElements);
    if (level == 0) {
        // if initial, recursive call to below edge (reversed direction)
        makeHorn(mesh, nextRightVec, nextLeftVec, level + 1, numElements);
    } else {
        // else, recursive call to newly created left, right side edge
        makeHorn(mesh, leftVec, nextLeftVec, level + 1, numElements);
        makeHorn(mesh, nextRightVec, rightVec, level + 1, numElements);
    }
    return;
}


void Fractal::GenerateSnowflake(Engine::Mesh* mesh)
{
    mesh->AddAttribute(4);
    mesh->AddAttribute(4);

    float xAbs = 3.0f / 5;
    float yAbs = (float)(1 / std::sqrt((double)3)) / 5;
    glm::vec4 bottom1 = glm::vec4(-xAbs, -yAbs, 0.02f, 1.0f);
    glm::vec4 bottom2 = glm::vec4(xAbs, -yAbs, 0.02f, 1.0f);

    int numElements = 0;
    makeHorn(mesh, bottom1, bottom2, 0, numElements);
    // initial makeHorn function call position is set like above
    // so that the created snowflake mesh center position is x = 0.0f, y = 0.0f

    mesh->SetNumElements(numElements);
    mesh->CreateMesh();
    return;
}