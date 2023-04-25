#include <chrono>

#include "walker.hpp"
#include "menu.hpp"

class Game {
    public:
        Game();
        ~Game();

        Menu* m_menu;
        Walker* m_walker;

        void run();

    private:
        dojo::Window* m_window;
        dojo::Renderer* m_renderer;
        dojo::Camera* m_camera;
        dojo::GameObject2DStatic* m_background;
        dojo::GameObject2DStatic* m_background2;
        bool m_running;

        void runMainLoop();
};
