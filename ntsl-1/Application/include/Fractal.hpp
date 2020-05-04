#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Geometry.hpp>
#include <Mesh.hpp>

/*
 * Implement Fractal class to generate Koch snowflakes mesh (generate snowflake mesh using triangle list).
 * Koch snowflake: https://en.wikipedia.org/wiki/Koch_snowflake
 */
class Fractal: public Geometry
{
private:
    glm::vec4 divideVectors(const glm::vec4 &v1, const glm::vec4 &v2, int a, int b);
    void makeHorn(Engine::Mesh* mesh, const glm::vec4 &leftVec, const glm::vec4 &rightVec, int level, int& numElements);
    const int MAX_LEVEL = 3;
public:
    Fractal();
    ~Fractal();
    void GenerateSnowflake(Engine::Mesh* mesh);
};