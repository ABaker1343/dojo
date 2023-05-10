#include "../../include/ecs/staticSpriteComponent.hpp"

namespace dojo {

StaticSpriteComponent::StaticSpriteComponent(const std::string& _path) : Component() {
    m_texture = new Texture(_path);
    flip = glm::ivec2(1);
    std::vector<float> vertices = std::vector<float> {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    m_renderInfo = RenderInfo::genNewRenderInfo(vertices);
}

StaticSpriteComponent::~StaticSpriteComponent() {
    delete m_texture;
}

}
