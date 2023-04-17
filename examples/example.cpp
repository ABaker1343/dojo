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

    dojo::Camera* cam = new dojo::Camera();
    cam->setPos(glm::vec3(0.f, 0.f, 15.f));
    cam->lookAt(glm::vec3(0.f));

    std::cout << "created renderer" << std::endl;

    dojo::GameObject2DAnimated *obj = new dojo::GameObject2DAnimated("stick_man.jpg", 1, 4);
    renderer->setShader("2DAnimated");
    //dojo::GameObject2DStatic *obj = new dojo::GameObject2DStatic("stick_man.jpg");
    //renderer->setShader("2DStatic");

    std::cout << "created game object" << std::endl;

    bool running = true;
    bool goingRight = true;

    unsigned int frames = 0;

    auto start = std::chrono::steady_clock::now();

    while(running) {
        renderer->clear();
        renderer->draw(cam, obj);
        w->flipBuffers();
        w->pollEvents();
        if (w->KEYS[GLFW_KEY_ESCAPE]) {
            running = false;
        }
        if (goingRight){
            //obj->setPos(obj->getPos() + glm::vec3(0.01, 0, 0));
            auto currpos = obj->getPos();
            obj->setPos(currpos.x + 0.01, currpos.y);
            //cam->setPos(cam->getPosition() + glm::vec3(0, 0, 0.1));
        }
        else {
            //obj->setPos(obj->getPos() + glm::vec3(-0.01, 0, 0));
            auto currpos = obj->getPos();
            obj->setPos(currpos.x + -0.01, currpos.y);
            //cam->setPos(cam->getPosition() + glm::vec3(0, 0, -0.1));
        }

        if (obj->getPos().x > 0.7) {
            goingRight = false;
        } else if (obj->getPos().x < -0.7) {
            goingRight = true;
        }

        if (frames % 10 == 0){
            if (!obj->nextFrame()) {
                obj->resetAnimation();
                obj->flipx();
            }
        }
        if (frames % 100 == 0) {
            obj->flipy();
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
