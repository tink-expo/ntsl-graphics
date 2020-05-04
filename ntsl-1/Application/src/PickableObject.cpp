#include <PickableObject.hpp>

PickableObject::PickableObject(Engine::Mesh* mesh, Engine::Material* material) : Engine::RenderObject(mesh, material)
{

}

void PickableObject::RenderPicking(Engine::Camera* cam)
{
    if (_index >= 0)
    {
        GLuint programId = _pickingMat->GetProgram()->GetProgramId();
        glUseProgram(programId);

        // Send camera uniform data to glsl program
        glm::mat4 worldTransform = GetWorldTransform();

        glUniformMatrix4fv(glGetUniformLocation(programId, "worldTransform"), 1, GL_FALSE, (float*)&worldTransform);

        glm::mat4 cameraTransform = cam->GetCameraTransform();
        glUniformMatrix4fv(glGetUniformLocation(programId, "cameraTransform"), 1, GL_FALSE, (float*)&cameraTransform);

        glm::mat4 projectionMatrix = cam->GetProjectionMatrix();
        glUniformMatrix4fv(glGetUniformLocation(programId, "projectionMatrix"), 1, GL_FALSE, (float*)&projectionMatrix);

        GLuint objectIDLoc = glGetUniformLocation(programId, "objectID");

        float r = ((_index >> 16) & 0xFF) / 255.0f;
        float g = ((_index >> 8) & 0xFF) / 255.0f;
        float b = (_index & 0xFF) / 255.0f;

        glUniform3f(objectIDLoc, r, g, b);

        GLenum drawMode = _mesh->GetDrawMode();
        int numElements = _mesh->GetNumElements();

        if (drawMode == GL_FALSE)
        {
            std::cout << "Cannot render this object with undefined behaviour\n";
            throw std::exception();
        }

        if (numElements == 0)
        {
            std::cout << "Cannot render this object with zero elements\n";
            throw std::exception();
        }

        glBindVertexArray(_mesh->GetVao());

        if (_mesh->GetHasIndex())
        {
            glBindBuffer(GL_ARRAY_BUFFER, _mesh->GetVbo());
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->GetIbo());
            glDrawElements(drawMode, numElements, GL_UNSIGNED_INT, 0);
        }
        else
        {

            glBindBuffer(GL_ARRAY_BUFFER, _mesh->GetVbo());
            glDrawArrays(drawMode, 0, numElements);
        }

        glBindVertexArray(0);
    }

}