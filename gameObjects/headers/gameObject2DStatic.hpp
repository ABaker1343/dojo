#ifndef __HEADER_GAME_OBJECT_2D_STATIC
#define __HEADER_GAME_OBJECT_2D_STATIC

#include "gameObject2D.hpp"
#include "renderable.hpp"
#include "texture.hpp"

#include <string>
#include <vector>
#include <iostream>

namespace dojo {

class GameObject2DStatic : public GameObject2D {
    public:
        GameObject2DStatic(const std::string& _texturePath);
        ~GameObject2DStatic();

        void setTexture(const std::string& _texturePath);
        Texture* getTexture();

    protected:
        Texture* m_Texture;
        

};

}

#endif