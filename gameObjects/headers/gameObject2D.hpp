#include "gameObject.hpp"
#include "renderable.hpp"

#include <string>
#include <vector>

namespace dojo {

class GameObject2D : GameObject, Renderable {
    public:
        GameObject2D(std::string& _texturePath);
        ~GameObject2D();

        void setTexture(std::string& _texturePath);

    protected:
        unsigned int m_Texture;
        std::vector<float> *vertexData;

        void createBuffers();
        

};

}
