#include "../../include/gameObjects/camera.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <iostream>


namespace dojo {

    Camera::Camera() {
        m_cameraTransform = glm::mat4(1.0f);

        m_worldUp = glm::vec3(0, 1, 0);

        m_yaw = -90.f;
        m_pitch = 0;
        m_roll = 0;

        setPos(glm::vec3(0, 0, 15));

        updateVectors();

        setPos(glm::vec3(0, 0, 15.f));
        
        m_FOV = 60;
        m_projectionTransform = glm::perspective(glm::radians(m_FOV), 16.f/9.f, 0.1f, 100.f);

    }

    Camera::~Camera() {

    }

    void Camera::setPos(glm::vec3 _pos) {
        m_cameraPos = _pos;
        updateVectors();
    }

    void Camera::lookAt(glm::vec3 _target) {

    }

    void Camera::rotate(float _degrees, CameraDir _dir) {
        switch(_dir) {
            case YAW:
                m_yaw += _degrees;
                break;
            case PITCH:
                m_pitch += _degrees;
                break;
            case ROLL:
                m_roll += _degrees;
                break;
        }
        updateVectors();
    }

    glm::vec3 Camera::getPosition() {
        return m_cameraPos;
    }

    void Camera::updateVectors() {
        m_cameraFront.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_cameraFront.y = sin(glm::radians(m_pitch));
        m_cameraFront.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_cameraFront = glm::normalize(m_cameraFront);

        m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_worldUp));
        //m_cameraUp = glm::normalize(glm::cross(m_cameraFront, m_cameraRight));
        m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));

        m_cameraTransform = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
    }

    glm::mat4 Camera::getCameraTransform() {
        return m_cameraTransform;
    }

    glm::mat4 Camera::getProjectionTransform() {
        return m_projectionTransform;
    }

}
