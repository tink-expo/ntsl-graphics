#include "NtslMaterial.hpp"
#include <glm/gtc/type_ptr.hpp>

void NtslMaterial::CreateMaterial()
{
    _program->AttachShader("Resources/NtslMaterial/VertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("Resources/NtslMaterial/GeometryShader.glsl", GL_GEOMETRY_SHADER);
    _program->AttachShader("Resources/NtslMaterial/FragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
}

void NtslMaterial::UpdateIforce(const glm::vec3& i_force)
{
    GLuint programId = _program->GetProgramId();
    glUseProgram(programId);
    
    GLuint location;
	location = glGetUniformLocation(programId, "i_force");
	glUniform3fv(location, 1, glm::value_ptr(i_force));
}