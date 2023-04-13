#ifndef __HEADER_GAME_OBJECT
#define __HEADER_GAME_OBJECT

#include <glm/glm.hpp>


namespace dojo {

class GameObject {
    public:
        GameObject();
        GameObject(glm::vec3 _pos, glm::vec3 _scale);
        virtual ~GameObject();

        virtual void setPos(glm::vec3 _pos);
        virtual void setScale(glm::vec3 _scale);

        virtual glm::vec3 getPos();
        virtual glm::vec3 getScale();

    protected:
        glm::vec3 m_WorldPos;
        glm::vec3 m_Scale;
};

}

#endif
