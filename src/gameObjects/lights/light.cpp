#include "../../../include/gameObjects/lights/light.hpp"
#include <glm/ext/matrix_clip_space.hpp>

namespace dojo {

    Light::Light() {
        m_lightTransform = glm::identity<glm::mat4>();
        m_fov = 60;
        m_target = glm::vec3(0);
        m_aspectRatio = 16.f/9.f;
        m_up = glm::vec3(0, 1, 0);
        m_projectionTransform = glm::perspective(m_fov, m_aspectRatio, 0.1f, 100.f);
        updateLightTransform();
        m_shadowMap = new Texture(glm::ivec2(1280, 720), Texture::Use::DEPTH_MAP);

        
    }

    Light::~Light() {
        delete m_shadowMap;
    }

    void Light::setPos(glm::vec3 _pos) {
        m_pos = _pos;
        updateLightTransform();
    }
    glm::vec3 Light::getPos() {
        return m_pos;
    }
    void Light::setFOV(float _fov) {
        m_fov = _fov;
    }
    void Light::setTarget(glm::vec3 _target) {
        m_target = _target;
        updateLightTransform();
    }
    Texture* Light::getShadowMap() {
        return m_shadowMap;
    }
    glm::mat4 Light::getLightTransform() {
        return m_lightTransform;
    }
    glm::mat4 Light::getProjectionTransform() {
        return m_projectionTransform;
    }

    void Light::updateLightTransform() {
        m_lightTransform = glm::lookAt(m_pos, m_target, m_up);
    }

}
