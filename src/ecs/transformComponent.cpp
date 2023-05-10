#include "../../include/ecs/transformComponent.hpp"

namespace dojo {

    TransformComponent::TransformComponent() : Component() {
        m_rotation = glm::identity<glm::mat4>();
        m_scale = glm::identity<glm::mat4>();
        m_translation = glm::identity<glm::mat4>();

        updateTransform();
    }

    glm::vec3 TransformComponent::getPos() {
        glm::vec3 pos;
        pos.x = m_translation[3][0];
        pos.y = m_translation[3][1];
        pos.z = m_translation[3][2];
        //pos = pos / m_translation[3][3];
        return pos;
    }

    glm::vec3 TransformComponent::getScale() {
        glm::vec3 scale;
        scale.x = m_scale[0][0];
        scale.y = m_scale[1][1];
        scale.z = m_scale[2][2];
        return scale;
    }

    TransformComponent* TransformComponent::setPos(glm::vec3 _pos) {
        m_translation[3][0] = _pos.x;
        m_translation[3][1] = _pos.y;
        m_translation[3][2] = _pos.z;
        m_translation[3][3] = 1;
        updateTransform();
        return this;
    }

    TransformComponent* TransformComponent::setScale(glm::vec3 _scale) {
        m_scale[0][0] = _scale.x;
        m_scale[1][1] = _scale.y;
        m_scale[2][2] = _scale.z;
        m_scale[3][3] = 1;
        updateTransform();
        return this;
    }

    void TransformComponent::updateTransform() {
        m_worldTransform = m_scale * m_rotation * m_translation;
        m_worldTransform = m_translation * m_rotation * m_scale;
    }
}
