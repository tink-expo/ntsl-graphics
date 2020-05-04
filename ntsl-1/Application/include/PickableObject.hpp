#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <RenderObject.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include <PickingMaterial.hpp>

class PickableObject : public Engine::RenderObject
{
private:
    PickingMaterial* _pickingMat;

public:
    PickableObject(Engine::Mesh* mesh, Engine::Material* material);
    void RenderPicking(Engine::Camera* cam);
    void SetPickingMat(PickingMaterial* mat) { _pickingMat = mat; };
};