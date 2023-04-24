#ifndef __HEADER_MENU_ITEM
#define __HEADER_MENU_ITEM

#include <glm/glm.hpp>
#include <vector>

#include "../../headers/texture.hpp"
#include "../../../gameObjects/headers/renderable.hpp"

namespace dojo {

class MenuItem : public Renderable {
    public:
        MenuItem(glm::vec2 _pos, glm::vec2 _scale, const std::string& _displayText);
        MenuItem(glm::vec2 _pos, glm::vec2 _scale, Texture* _texture);
        ~MenuItem();

        Texture* getTexture();
        unsigned int getVertexArray();

    private:
        
        static unsigned int m_staticVertexArray;
        static unsigned int m_staticVertexBuffer;

        Texture* m_texture;
        
        void createBuffers();

};

}

#endif
