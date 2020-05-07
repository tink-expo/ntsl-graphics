#pragma once

#include <Material.hpp>

class HullMaterial : public Engine::Material
{
private:
public:
    void CreateMaterial();
    void UpdateIforce(const glm::vec3& i_force);
};