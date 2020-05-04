#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <RenderObject.hpp>
#include <Mesh.hpp>
#include <Material.hpp>

/*
 * TODO: Inherit RenderObject class to implement animation
 */
class Snowflake: public Engine::RenderObject
{
private:
public:
    Snowflake(Engine::Mesh* mesh, Engine::Material* material);
    bool stacked = false;
    glm::vec3 initialPosition;
};