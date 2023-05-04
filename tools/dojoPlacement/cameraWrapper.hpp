#ifndef __HEADER_CAMERA_WRAPPER
#define __HEADER_CAMERA_WRAPPER

#include <dojo/dojo.hpp>
#include <memory>

class CameraWrapper {
    public:
        std::unique_ptr<dojo::Camera> m_camera;
        std::unique_ptr<dojo::MenuItem> m_info;
        
        CameraWrapper(dojo::Renderer* _renderer);
        void move(float _amount, dojo::Camera::CameraDir _dir, dojo::Renderer* _renderer);

    private:
        void updateInfo(dojo::Renderer* _renderer);
};

#endif
