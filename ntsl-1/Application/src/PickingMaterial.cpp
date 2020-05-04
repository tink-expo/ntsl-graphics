#include <PickingMaterial.hpp>

void PickingMaterial::CreateMaterial()
{
    _program->AttachShader("Resources/PickingMaterial/PickingVertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("Resources/PickingMaterial/PickingFragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
}