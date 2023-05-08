#ifndef __HEADER_SCENE
#define __HEADER_SCENE

#include "../window/renderer.hpp"
#include "../components/spriteComponent.hpp"

namespace dojo {

class Scene {
    public:
        Scene();
        ~Scene();

    private:
        Renderer* m_renderer;
        std::vector<SpriteComponent> m_spriteComponents;

};

}

#endif
