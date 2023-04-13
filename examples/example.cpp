#include "../headers/dojo.hpp"
#include <iostream>
#include <chrono>

int main() {
    std::string title = "new window";
    dojo::Window *w = new dojo::Window(1280, 720, title);

    std::cout << "created window" << std::endl;

    dojo::Renderer *renderer = new dojo::Renderer(w);
    renderer->setShaderPath("../shaders/");
    renderer->loadShaders();

    std::cout << "created renderer" << std::endl;

    dojo::GameObject2D *obj = new dojo::GameObject2D("stick_man.jpg");

    obj->setTexture("stick_man.png");

    std::cout << "created game object" << std::endl;

    bool running = true;
    bool goingRight = true;

    unsigned int frames = 0;

    auto start = std::chrono::steady_clock::now();

    while(running) {
        renderer->clear();
        renderer->draw(obj);
        w->flipBuffers();
        w->pollEvents();
        if (w->KEYS[GLFW_KEY_ESCAPE]) {
            running = false;
        }
        if (goingRight){
            obj->setPos(obj->getPos() + glm::vec3(0.01, 0, 0));
        }
        else {
            obj->setPos(obj->getPos() + glm::vec3(-0.01, 0, 0));
        }

        if (obj->getPos().x > 0.7) {
            goingRight = false;
        } else if (obj->getPos().x < -0.7) {
            goingRight = true;
        }
        frames ++;
    }

    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end-start);
    std::cout << "frames per second: " << frames / elapsed.count() << std::endl;

    delete obj;
    delete renderer;
    delete w;

}
