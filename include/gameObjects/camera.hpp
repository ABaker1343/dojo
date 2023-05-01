#ifndef __HEADER_CAMERA
#define __HEADER_CAMERA

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace dojo {

class Camera {

    public:

        enum CameraDir {
            YAW, PITCH, ROLL
        };

    public:
        Camera();
        ~Camera();

        void setPos(glm::vec3 _pos);
        void lookAt(glm::vec3 _target);
        void rotate(float degrees, CameraDir _dir);
        
        glm::vec3 getPosition();

        void makeOrtho();
        void makePerspective();
        void setFOV(float _fov);

        glm::mat4 getCameraTransform();
        glm::mat4 getProjectionTransform();

    private:
        glm::mat4 m_cameraTransform;
        glm::mat4 m_projectionTransform;

        float m_FOV;
        float m_aspectRatio;
        glm::vec3 m_cameraUp;
        glm::vec3 m_worldUp;
        glm::vec3 m_cameraFront;
        glm::vec3 m_cameraRight;
        glm::vec3 m_cameraPos;
        float m_pitch;
        float m_yaw;
        float m_roll;
        glm::vec3 m_center;

        void updateVectors();
};

}

#endif
