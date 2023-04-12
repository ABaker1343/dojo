#ifndef __HEADER_GAME_OBJECT_2D
#define __HEADER_GAME_OBJECT_2D

#include "gameObject.hpp"
#include "renderable.hpp"

#include <string>
#include <vector>

namespace dojo {

class GameObject2D : public GameObject, public Renderable {
    public:
        GameObject2D(const std::string& _texturePath);
        ~GameObject2D();

        void setTexture(const std::string& _texturePath);

    protected:
        unsigned int m_Texture;
        std::vector<float> *vertexData;

        void createBuffers();
        

};

}

#endif
