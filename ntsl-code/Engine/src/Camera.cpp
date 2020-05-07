#include <Camera.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp> 

namespace Engine
{
    void pm(glm::mat4 m) {
        // for (int i = 0; i < 4; ++i) {
        //     for (int j = 0; j < 4; ++j) {
        //         std::cout << m[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }
        std::cout << glm::to_string(m) << std::endl;
        std::cout << std::endl;
    }
    
    Camera::Camera()
    {
        _position = glm::vec3(0.0f);
        _orientation = glm::mat4(1.0f);
    }

    Camera::Camera(glm::vec3 position, glm::mat4 orientation)
    {
        _position = position;
        _orientation = orientation;
    }

    glm::mat4 Camera::GetCameraTransform()
    {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), _position);
        return _orientation * translationMatrix;
    }

    void Camera::SetProjection(float aspectRatio, float fov, float zNear, float zFar) 
    { 
        _projection.aspectRatio = aspectRatio;
        _projection.fov = fov;
        _projection.zNear = zNear;
        _projection.zFar = zFar;
    }

    ProjectionData Camera::GetProjection()
    {
        return _projection;
    }
} // Engine