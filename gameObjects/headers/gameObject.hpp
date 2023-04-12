#include <glm/glm.hpp>


namespace dojo {

class GameObject {
    public:
        GameObject();
        GameObject(glm::vec3 _pos, glm::vec3 _scale);
        ~GameObject();

        void setPos();
        void scale();

    protected:
        glm::vec3 m_WorldPos;
        glm::vec3 m_Scale;
};

}
