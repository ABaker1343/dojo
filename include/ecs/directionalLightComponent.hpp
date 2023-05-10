#ifndef __HEADER_DIRECTIONAL_LIGHT_COMPONENT
#define __HEADER_DIRECTIONAL_LIGHT_COMPONENT

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext.hpp>

#include "../gameObjects/texture.hpp"
#include "component.hpp"

namespace dojo {

class DirectionalLightComponent : public Component {
    public:
        DirectionalLightComponent(glm::vec3 _color = glm::vec3(1));
        ~DirectionalLightComponent();

        glm::vec3 getColor();
        Texture* getShadowMap();

    private:
        Texture* m_shadowMap;
        glm::vec3 m_color;


};

}

#endif
