#include "../../../include/gameObjects/objects2D/gameObject2DStatic.hpp"

namespace dojo {
    GameObject2DStatic::GameObject2DStatic(const std::string& _texturePath) : GameObject2D() {
        m_Texture = nullptr;
        setTexture(_texturePath);
    }

    GameObject2DStatic::~GameObject2DStatic() {
        delete m_Texture;
    }
    
    Texture* GameObject2DStatic::getTexture() {
        return m_Texture;
    }

    void GameObject2DStatic::setTexture(const std::string& _texturePath) {
        // set the texture of the object for rendering
        if (!(m_Texture == nullptr)) {
            delete m_Texture;
        }
        m_Texture = new Texture(_texturePath);
    }
}
