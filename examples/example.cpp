#include "../headers/dojo.hpp"
#include <iostream>
#include <chrono>

void printvec(glm::vec3 _vec) {
    std::cout << _vec.x << " " << _vec.y << " " << _vec.z << std::endl;
}

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

    dojo::GameObject2DAnimated *obj1 = new dojo::GameObject2DAnimated("stick_man.jpg", 1, 4);
    dojo::BoxCollider *box1 = new dojo::BoxCollider(obj1->getPos(), obj1->getScale());

    dojo::GameObject2DStatic *obj2 = new dojo::GameObject2DStatic("stick_man.png");
    obj2->setScale(glm::vec3(0.75));
    dojo::BoxCollider *box2 = new dojo::BoxCollider(obj2->getPos(), obj2->getScale());

    //dojo::GameObject2DStatic *obj = new dojo::GameObject2DStatic("stick_man.jpg");
    //renderer->setShader("2DStatic");

    std::cout << "created game object" << std::endl;

    bool running = true;
    bool goingRight = true;
    float delta = 3.0;

    unsigned int frames = 0;

    auto start = std::chrono::steady_clock::now();


    while(running) {
        renderer->clear();
        renderer->setShader("2DAnimated");
        renderer->draw(cam, obj1);
        renderer->setShader("2DStatic");
        renderer->draw(cam, obj2);
        w->flipBuffers();
        w->pollEvents();
        if (w->KEYS[GLFW_KEY_ESCAPE]) {
            running = false;
        }
        if (goingRight){
            //obj->setPos(obj->getPos() + glm::vec3(0.01, 0, 0));
            auto currpos = obj1->getPos();
            obj1->setPos(currpos.x + 0.01, currpos.y);
            box1->setCenter(obj1->getPos());
            //cam->setPos(cam->getPosition() + glm::vec3(0, 0, 0.1));
        }
        else {
            //obj->setPos(obj->getPos() + glm::vec3(-0.01, 0, 0));
            auto currpos = obj1->getPos();
            obj1->setPos(currpos.x + -0.01, currpos.y);
            box1->setCenter(obj1->getPos());
            //cam->setPos(cam->getPosition() + glm::vec3(0, 0, -0.1));
        }

        if (obj1->getPos().x > delta) {
            goingRight = false;
        } else if (obj1->getPos().x < -delta) {
            goingRight = true;
        }

        if (frames % 10 == 0){
            if (!obj1->nextFrame()) {
                obj1->resetAnimation();
                obj1->flipx();
            }
        }
        if (frames % 100 == 0) {
            obj1->flipy();
        }

        if (box1->isColliding(box2)) {
            std::cout << "colliding " << frames << "\n";
        }

        frames ++;
    }

    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end-start);
    std::cout << "frames per second: " << frames / elapsed.count() << std::endl;

    std::cout << "deleting object 1 static" << std::endl;
    delete obj1;
    std::cout << "deleting object 2 static" << std::endl;
    delete obj2;
    std::cout << "deleting boxes" << std::endl;
    delete box1;
    delete box2;
    std::cout << "deleting renderer" << std::endl;
    delete renderer;
    std::cout << "deleting window" << std::endl;
    delete w;

}
