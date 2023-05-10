#include "../../include/ecs/viewerTransformComponent.hpp"

namespace dojo {

    ViewerTransformComponent::ViewerTransformComponent(glm::vec3 _target) : TransformComponent() {
        m_viewTransform = glm::identity<glm::mat4>();
        m_fov = 60;
        m_target = _target;
        m_aspectRatio = 16.f/9.f;
        m_up = glm::vec3(0, 1, 0);
        m_projectionTransform = glm::perspective(m_fov, m_aspectRatio, 0.15f, 60.f);
        updateViewTransform();

    }

    void ViewerTransformComponent::setFOV(float _fov) {
        m_fov = _fov;
    }

    void ViewerTransformComponent::setTarget(glm::vec3 _target) {
        m_target = _target;
        updateViewTransform();
    }

    glm::mat4 ViewerTransformComponent::getViewTransform() {
        updateViewTransform();
        return m_viewTransform;
    }

    glm::mat4 ViewerTransformComponent::getProjectionTransform() {
        return m_projectionTransform;
    }

    void ViewerTransformComponent::updateViewTransform() {
        m_viewTransform = glm::lookAt(getPos(), m_target, m_up);
    }
}
