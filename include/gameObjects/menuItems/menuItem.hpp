#ifndef __HEADER_MENU_ITEM
#define __HEADER_MENU_ITEM

#include <glm/glm.hpp>
#include <vector>
#include <iostream>

#include "../texture.hpp"
#include "../renderable.hpp"

namespace dojo {
    class Renderer;
}

namespace dojo {

class MenuItem : public Renderable {
    public:
        MenuItem(glm::vec2 _pos, glm::vec2 _scale, const std::string& _displayText, Renderer* _renderer,
                glm::vec4 _textColor = glm::vec4(0, 0, 0, 1), glm::vec4 _backgroundColor = glm::vec4(0.5, 0.5, 0.5, 1));

        MenuItem(glm::vec2 _pos, glm::vec2 _scale, Texture* _texture);
        MenuItem(glm::vec2 _pos, glm::vec2 _scale);
        ~MenuItem();

        bool isMouseOver(glm::vec2 _windowSize, glm::vec2 _mousePos);

        void setPos(glm::vec2 _pos);
        void setScale(glm::vec2 _scale);

        Texture* getTexture();
        unsigned int getVertexArray();
        glm::vec4 getLocation();

    private:
        
        static unsigned int m_staticVertexArray;
        static unsigned int m_staticVertexBuffer;

        glm::vec2 m_pos;
        glm::vec2 m_scale;

        Texture* m_texture;
        
        void createBuffers();

};

}

#include "../../window/renderer.hpp"

#endif
