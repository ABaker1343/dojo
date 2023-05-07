#ifndef __HEADER_PLACEMENT
#define __HEADER_PLACEMENT

#include <cstddef>
#include <dojo/dojo.hpp>
#include <dojo/gameObjects/objects2D/gameObject2DAnimated.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <memory>
#include <chrono>

#include "objectWrapper.hpp"
#include "cameraWrapper.hpp"
#include "objectCreator.hpp"

class Placement {

    public:
        Placement(int argc, const char* argv[]);
        ~Placement();
        void run();

    private:
        std::unique_ptr<dojo::Window> m_window;
        std::unique_ptr<dojo::Renderer> m_renderer;

        std::unique_ptr<CameraWrapper> m_cameraWrap;
        std::unique_ptr<dojo::MenuItem> m_createButton;
        std::unique_ptr<ObjectCreator> m_creator;

        std::vector<ObjectWrapper*> m_objects;
        size_t m_selectedIndex;
        ObjectWrapper* m_selectedObject;
        float m_menuStackHeight;
        glm::vec4 m_selectedColor;
        glm::vec4 m_defaultColor;

        std::chrono::time_point<std::chrono::steady_clock> m_lastUpdateTime;
        std::chrono::milliseconds m_msSinceUpdate;
    

        void update();
        void render();

        void createObject();

        unsigned int bindKeyCallback();
        unsigned int bindButtonCallback();

};

#endif
