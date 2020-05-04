#include <Snowflake.hpp>
#include <cstdlib>
#include <cmath>
using namespace std;

Snowflake::Snowflake(Engine::Mesh* mesh, Engine::Material* material) : Engine::RenderObject(mesh, material)
{
    initialPosition = glm::vec3(0.0f, 4.5f, 0.0f);
    float random = (float)(rand() % 100) / 100; // [0.0f, 1.0f)
    float width = 15.0f;
    initialPosition.x = -width / 2 + random * width;
    // randomly set initial position of created snowflake
    initialPosition.y += abs(initialPosition.x) / 2;
    SetPosition(initialPosition);
}