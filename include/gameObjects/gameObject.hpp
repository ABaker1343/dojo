#ifndef __HEADER_GAME_OBJECT
#define __HEADER_GAME_OBJECT

#include <glm/glm.hpp>
#include <memory>


namespace dojo {

class GameObject {
    public:
        virtual ~GameObject() {};
        virtual std::unique_ptr<GameObject> clone() = 0;
        virtual void setPos(glm::vec3 _pos) = 0;
        virtual void setPos(float _x, float _y, float _z) = 0;
        virtual void setScale(glm::vec3 _scale) = 0;
        virtual void setScale(float _x, float _y, float _z) = 0;

        virtual glm::vec3 getPos() = 0;
        virtual glm::vec3 getScale() = 0;

    protected:

};

}

#endif
