#ifndef __HEADER_VIEWER_COMPONENT
#define __HEADER_VIEWER_COMPONENT

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "transformComponent.hpp"

namespace dojo {

class ViewerTransformComponent : public TransformComponent {
    public:
        ViewerTransformComponent(glm::vec3 _target = glm::vec3(0));
        ~ViewerTransformComponent();

        void setFOV(float _fov);
        void setTarget(glm::vec3 _target);

        glm::mat4 getViewTransform();
        glm::mat4 getProjectionTransform();
        glm::mat4 getWorldTransform();

    private:

        float m_fov;
        float m_aspectRatio;
        float m_near;
        float m_far;
        glm::mat4 m_viewTransform;
        glm::mat4 m_projectionTransform;
        glm::mat4 m_rotation;
        glm::mat4 m_scale;
        glm::vec3 m_target;
        glm::vec3 m_up;

        void updateViewTransform();

};

}

#endif
