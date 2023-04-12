#include "gameObject.hpp"
#include "renderable.hpp"

#include <string>

namespace dojo {

class GameObject2D : GameObject, Renderable {
    public:
        GameObject2D(std::string& _texturePath);
        ~GameObject2D();

    protected:
        unsigned int m_Texture;

};

}
