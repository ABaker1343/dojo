#ifndef __HEADER_BOX_COLLIDER
#define __HEADER_BOX_COLLIDER

#include <glm/glm.hpp>

#include "collider.hpp"
#include "../../headers/gameObject.hpp"
#include "../../headers/objects2D/gameObject2D.hpp"

namespace dojo {

class BoxCollider : public Collider {

    public:

        enum Side {
            LEFT, RIGHT, TOP, BOTTOM,
        };

    public:
        BoxCollider(glm::vec3 _pos, glm::vec3 _scale);
        ~BoxCollider() override;

        void snapToSide(GameObject* _object, Side _side);

        glm::vec3 getCenter();
        glm::vec3 getScale();

        void setCenter(glm::vec3 _center);
        void setScale(glm::vec3 _scale);

        bool isColliding(BoxCollider *_other);

    protected:
        std::vector<float> m_Vertices;
        glm::vec3 m_center;
        glm::vec3 m_scale;

};

}

#endif
