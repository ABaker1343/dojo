#include "../../include/ecs/directionalLightComponent.hpp"

namespace dojo {

    DirectionalLightComponent::DirectionalLightComponent(glm::vec3 _color) {
        m_shadowMap = new Texture(glm::ivec2(1280, 720), Texture::Use::DEPTH_MAP);
        
        m_color = _color;
    }

    DirectionalLightComponent::~DirectionalLightComponent() {
        delete m_shadowMap;
    }

    glm::vec3 DirectionalLightComponent::getColor() {
        return m_color;
    }
    Texture* DirectionalLightComponent::getShadowMap() {
        return m_shadowMap;
    }


}
