#ifndef __HEADER_GAME_OBJECT_2D
#define __HEADER_GAME_OBJECT_2D

#include "gameObject.hpp"
#include "renderable.hpp"
#include "texture.hpp"

#include <string>
#include <vector>
#include <iostream>

namespace dojo {

class GameObject2D : public GameObject, public Renderable {
    public:
        GameObject2D(const std::string& _texturePath);
        ~GameObject2D();

        void setTexture(const std::string& _texturePath);
        void setPos(glm::vec3 _pos) override;
        void setScale(glm::vec3 _scale) override;

        glm::vec3 getPos() override;
        glm::vec3 getScale() override;

    protected:
        std::vector<float> *m_VertexData;

        void createBuffers() override;
        

};

}

#endif
