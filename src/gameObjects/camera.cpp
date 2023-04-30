#include "../../include/gameObjects/camera.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <iostream>


namespace dojo {

    Camera::Camera() {
        m_cameraTransform = glm::mat4(1.0f);
        m_cameraFront = glm::vec3(0, 0, -1);
        m_cameraUp = glm::vec3(0.f, 1.f, 0.f);

        setPos(glm::vec3(0, 0, 15.f));
        lookAt(m_cameraFront);
        
        m_FOV = 60;
        m_projectionTransform = glm::perspective(glm::radians(m_FOV), 16.f/9.f, 0.1f, 100.f);

    }

    Camera::~Camera() {

    }

    void Camera::setPos(glm::vec3 _pos) {
        m_cameraTransform[3][0] = -_pos.x;
        m_cameraTransform[3][1] = -_pos.y;
        m_cameraTransform[3][2] = -_pos.z;
    }

    void Camera::lookAt(glm::vec3 _target) {
        m_cameraFront = glm::normalize(_target);
        m_cameraTransform = glm::lookAt(getPosition(), getPosition() + _target, m_cameraUp);
    }

    void Camera::rotate(float _degrees, glm::vec3 _axis) {
        glm::vec3 oldPos = getPosition();
        setPos(glm::vec3(0));

        float rads = glm::radians(_degrees);
        float w = glm::cos(rads/2);
        glm::vec3 v = glm::sin(rads/2) * _axis;

        glm::quat q = glm::quat(w, v);
        glm::quat qc = glm::quat(w, -v);

        m_cameraFront = q * m_cameraFront * qc;

        lookAt(m_cameraFront);
        setPos(oldPos);
    }

    glm::vec3 Camera::getPosition() {
        glm::vec3 position;
        position.x = -m_cameraTransform[3][0];
        position.y = -m_cameraTransform[3][1];
        position.z = -m_cameraTransform[3][2];

        return position;
    }

    glm::mat4 Camera::getCameraTransform() {
        return m_cameraTransform;
    }

    glm::mat4 Camera::getProjectionTransform() {
        return m_projectionTransform;
    }

}
