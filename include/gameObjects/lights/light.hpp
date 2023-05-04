#ifndef __HEADER_LIGHT
#define __HEADER_LIGHT

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "../texture.hpp"

namespace dojo {

class Light {
    public:
        Light();
        ~Light();

        void setPos(glm::vec3 _pos);
        glm::vec3 getPos();
        void setFOV(float _fov);
        void setTarget(glm::vec3 _target);
        Texture* getShadowMap();
        glm::mat4 getLightTransform();
        glm::mat4 getProjectionTransform();

    private:
        Texture* m_shadowMap;
        float m_fov;
        float m_aspectRatio;
        glm::mat4 m_lightTransform;
        glm::mat4 m_projectionTransform;
        glm::vec3 m_pos;
        glm::vec3 m_target;
        glm::vec3 m_up;

        void updateLightTransform();

};

}

#endif
