#include "../headers/dojo.hpp"
#include <iostream>

int main() {
    std::string title = "new window";
    dojo::Window *w = new dojo::Window(1280, 720, title);

    std::cout << "created window" << std::endl;

    dojo::Renderer *renderer = new dojo::Renderer(w);
    renderer->setShaderPath("../shaders/");
    renderer->loadShaders();

    std::cout << "created renderer" << std::endl;

    dojo::GameObject2D *obj = new dojo::GameObject2D("somepath");

    std::cout << "created game object" << std::endl;

    bool running = true;

    while(running) {
        renderer->clear();
        renderer->draw(*obj);
        w->pollEvents();
        if (w->KEYS[GLFW_KEY_ESCAPE]) {
            running = false;
        }
        w->flipBuffers();
    }

}
