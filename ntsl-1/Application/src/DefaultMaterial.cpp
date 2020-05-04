#include <DefaultMaterial.hpp>

void DefaultMaterial::CreateMaterial()
{
    _program->AttachShader("Resources/DefaultMaterial/VertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("Resources/DefaultMaterial/FragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
}

void DefaultMaterial::UpdateUresolution(float res_w, float res_h)
{
    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "u_resolution");
    glUniform2f(location, res_w, res_h);
}

void DefaultMaterial::UpdateUtime(float time)
{
    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "u_time");
    glUniform1f(location, time);
}