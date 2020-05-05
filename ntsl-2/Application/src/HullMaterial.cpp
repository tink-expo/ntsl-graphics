#include "HullMaterial.hpp"

void HullMaterial::CreateMaterial()
{
    _program->AttachShader("Resources/HullMaterial/VertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("Resources/HullMaterial/GeometryShader.glsl", GL_GEOMETRY_SHADER);
    _program->AttachShader("Resources/HullMaterial/FragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
}