#ifndef __HEADER_CAMERA
#define __HEADER_CAMERA

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace dojo {

class Camera {
    public:
        Camera();
        ~Camera();

        void setPos(glm::vec3 _pos);
        void lookAt(glm::vec3 _target);
        
        glm::vec3 getPosition();

        glm::mat4 getCameraTransform();
        glm::mat4 getProjectionTransform();

    private:
        glm::mat4 m_CameraTransform;
        glm::mat4 m_ProjectionTransform;

        float m_FOV;
        glm::vec3 m_CameraUp;

};

}

#endif
