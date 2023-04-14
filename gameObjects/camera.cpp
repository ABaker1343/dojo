#include "headers/camera.hpp"

namespace dojo {

    Camera::Camera() {
        m_CameraTransform = glm::mat4(1.0f);
        setPos(glm::vec3(0.0f));
        lookAt(glm::vec3(0.f));
        
        m_CameraUp = glm::vec3(0.f, 1.f, 0.f);

        m_FOV = 60;
        m_ProjectionTransform = glm::perspective(glm::radians(m_FOV), 16.f/9.f, 0.1f, 100.f);

    }

    Camera::~Camera() {

    }

    void Camera::setPos(glm::vec3 _pos) {
        m_CameraTransform[3][0] = -_pos.x;
        m_CameraTransform[3][1] = -_pos.y;
        m_CameraTransform[3][2] = -_pos.z;
    }

    void Camera::lookAt(glm::vec3 _target) {
        m_CameraTransform = glm::lookAt(getPosition(), _target, m_CameraUp);
    }

    glm::vec3 Camera::getPosition() {
        glm::vec3 position;
        position.x = -m_CameraTransform[3][0];
        position.y = -m_CameraTransform[3][1];
        position.z = -m_CameraTransform[3][2];

        return position;
    }

    glm::mat4 Camera::getCameraTransform() {
        return m_CameraTransform;
    }

    glm::mat4 Camera::getProjectionTransform() {
        return m_ProjectionTransform;
    }

}
