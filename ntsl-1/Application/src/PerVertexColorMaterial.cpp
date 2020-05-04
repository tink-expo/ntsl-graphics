#include <PerVertexColorMaterial.hpp>

void PerVertexColorMaterial::CreateMaterial()
{
    _program->AttachShader("Resources/PerVertexColorMaterial/VertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("Resources/PerVertexColorMaterial/FragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
}