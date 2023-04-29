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
        Placement(int argc, const char* argv[]);
        ~Placement();
        void run();

    private:
        std::unique_ptr<dojo::Window> m_window;
        std::unique_ptr<dojo::Renderer> m_renderer;
        std::unique_ptr<dojo::Camera> m_camera;

        std::vector<ObjectWrapper*> m_objects;
        size_t m_selectedIndex;
        ObjectWrapper* m_selectedObject;
    
        void update();

};
