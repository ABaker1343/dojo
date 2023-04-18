#ifndef __HEADER_BOX_COLLIDER
#define __HEADER_BOX_COLLIDER

#include <glm/glm.hpp>

#include "collider.hpp"

namespace dojo {

class BoxCollider : public Collider {
    public:
        BoxCollider(glm::vec3 _pos, glm::vec3 _scale);
        ~BoxCollider() override;

        glm::vec3 getCenter();
        glm::vec3 getScale();

        void setCenter(glm::vec3 _center);
        void setScale(glm::vec3 _scale);

        bool isColliding(BoxCollider *_other);

    protected:
        std::vector<float> m_Vertices;
        glm::vec3 m_Center;
        glm::vec3 m_Scale;

};

}

#endif
