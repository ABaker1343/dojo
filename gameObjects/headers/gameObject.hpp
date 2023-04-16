#ifndef __HEADER_GAME_OBJECT
#define __HEADER_GAME_OBJECT

#include <glm/glm.hpp>


namespace dojo {

class GameObject {
    public:
        virtual ~GameObject() {};
        virtual void setPos(glm::vec3 _pos) = 0;
        virtual void setScale(glm::vec3 _scale) = 0;

        virtual glm::vec3 getPos() = 0;
        virtual glm::vec3 getScale() = 0;

    protected:

};

}

#endif
