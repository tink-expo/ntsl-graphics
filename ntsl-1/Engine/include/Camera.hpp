#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    struct ProjectionData
    {
        float aspectRatio = 4.0f / 3.0f;
        float fov = 60.0f;
        float zNear = 0.01f;
        float zFar = 1000.0f;
    };

    class Camera
    {
    private:
        glm::vec3 _position;
        glm::mat4 _orientation;
        ProjectionData _projection;
    public:
        Camera();
        Camera(glm::vec3 position, glm::mat4 orientation);

        void SetPosition(glm::vec3 position) { _position = position; }
        glm::vec3 GetPosition() { return _position; }

        void SetOrientation(glm::mat4 orientation) { _orientation = orientation; }
        glm::mat4 GetOrientation() { return _orientation; }

        glm::mat4 GetCameraTransform();

        void SetProjection(float aspectRatio, float fov, float zNear, float zFar);
        ProjectionData GetProjection();
        glm::mat4 GetProjectionMatrix() { return glm::perspective(glm::radians(_projection.fov), _projection.aspectRatio, _projection.zNear, _projection.zFar); }
    };
} // Engine