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
        
        m_FOV = 90;
        m_aspectRatio = 16.f/9.f;
        m_projectionTransform = glm::perspective(glm::radians(m_FOV), m_aspectRatio, 0.1f, 100.f);

    }

    Camera::~Camera() {

    }

    void Camera::setPos(glm::vec3 _pos) {
        m_cameraPos = _pos;
        updateVectors();
    }

    void Camera::makeOrtho() {
        float verticalFOV = m_FOV * m_aspectRatio;
        m_projectionTransform = glm::ortho(-0.5 * m_FOV, 0.5 * m_FOV, -0.5 * verticalFOV, 0.5 * verticalFOV, 0.1, 100.0);
    }

    void Camera::makePerspective() {
        m_projectionTransform = glm::perspective(glm::radians(m_FOV), m_aspectRatio, 0.1f, 100.f);
    }

    void Camera::setFOV(float _fov) {
        m_FOV = _fov;
    }

    void Camera::lookAt(glm::vec3 _target) {

    }

    void Camera::rotate(float _degrees, CameraAxis _axis) {
        switch(_axis) {
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

    void Camera::move(float _amount, CameraDir _dir) {
        glm::vec3 movement;
        switch(_dir) {
            case FORWARD:
                movement = m_cameraFront * _amount;
                break;
            case RIGHT:
                movement = m_cameraRight * _amount;
                break;
            case UP:
                movement = m_cameraUp * _amount;
                break;
        }
        setPos(getPosition() + movement);
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
