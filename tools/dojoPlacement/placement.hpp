#include <cstddef>
#include <dojo/dojo.hpp>
#include <dojo/gameObjects/objects2D/gameObject2DAnimated.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <memory>

#include "objectWrapper.hpp"

class Placement {

    public:

        struct CameraWrapper {
            std::unique_ptr<dojo::Camera> m_camera;
            std::unique_ptr<dojo::MenuItem> m_info;
            
            CameraWrapper(dojo::Renderer* _renderer);
            void updateInfo(dojo::Renderer* _renderer);
        };

    public:
        Placement(int argc, const char* argv[]);
        ~Placement();
        void run();

    private:
        std::unique_ptr<dojo::Window> m_window;
        std::unique_ptr<dojo::Renderer> m_renderer;

        std::unique_ptr<CameraWrapper> m_cameraWrap;

        std::vector<ObjectWrapper*> m_objects;
        size_t m_selectedIndex;
        ObjectWrapper* m_selectedObject;
        glm::vec4 m_selectedColor;
        glm::vec4 m_defaultColor;
    
        void update();

        unsigned int bindKeyCallback();

};
