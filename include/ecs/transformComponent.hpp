#ifndef __HEADER_TRANSFORMATION_COMPONENT
#define __HEADER_TRANSFORMATION_COMPONENT

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "component.hpp"

namespace dojo {

class TransformComponent : public Component {
    public:
        TransformComponent();

        glm::mat4 m_worldTransform;
        glm::mat4 m_scale;
        glm::mat4 m_rotation;
        glm::mat4 m_translation;

        glm::vec3 getPos();
        glm::vec3 getScale();

        TransformComponent* setPos(glm::vec3 _pos);
        TransformComponent* setScale(glm::vec3 _scale);

        void updateTransform();

    private:

};

}

#endif
