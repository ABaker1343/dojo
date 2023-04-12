#include <glm/glm.hpp>


namespace dojo {

class GameObject {
    public:
        GameObject();
        GameObject(glm::vec3 _pos, glm::vec3 _scale);
        ~GameObject();

        void setPos(glm::vec3 _pos);
        void setScale(glm::vec3 _scale);

    protected:
        glm::vec3 m_WorldPos;
        glm::vec3 m_Scale;
};

}
