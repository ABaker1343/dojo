#ifndef __HEADER_SPRITE_COMPONENT
#define __HEADER_SPRITE_COMPONENT

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "component.hpp"
#include "../window/renderInfo.hpp"
#include "../gameObjects/texture.hpp"

namespace dojo {

class StaticSpriteComponent : public Component {
    public:
        StaticSpriteComponent(const std::string& _path);
        ~StaticSpriteComponent();

        Texture* m_texture;
        glm::ivec2 flip;
        RenderInfo m_renderInfo;
        
};

}

#endif
