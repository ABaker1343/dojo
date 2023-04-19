#include "../headers/dojo.hpp"
#include <iostream>
#include <chrono>
#include <thread>

void printvec(glm::vec3 _vec) {
    std::cout << _vec.x << " " << _vec.y << " " << _vec.z << std::endl;
}

void HandleInputs(dojo::Window* _window, dojo::GameObject2DAnimated* _object) {

    static auto lastUpdateTime = std::chrono::steady_clock::now();
    static auto lastAnimationTime = lastUpdateTime;
    
    auto thisUpdateTime = std::chrono::steady_clock::now();
    auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(thisUpdateTime - lastUpdateTime).count();
    auto animationTimePassed = std::chrono::duration_cast<std::chrono::milliseconds>(thisUpdateTime - lastAnimationTime).count();
    
    float movementSpeed = 1 * (timePassed / 1000.f); // seconds
    float animationSpeed = 1000.f / 5.f; // frames per seconds

    if (_window->KEYS[GLFW_KEY_D]) {
        if (_object->getFlip().x != dojo::GameObject2D::FACE_RIGHT) {
            _object->flipx();
        }
        if (animationTimePassed > animationSpeed) {
            lastAnimationTime = std::chrono::steady_clock::now();
            if (_object->currentAnimation() == "Walking") {
                if(!_object->nextFrame()) {
                    _object->resetAnimation();
                }
            } else {
                _object->setAnimation("Walking");
                _object->resetAnimation();
            }
        }

        _object->setPos(_object->getPos() + glm::vec3(movementSpeed, 0, 0));
    }

    else if (_window->KEYS[GLFW_KEY_A]) {
        if (_object->getFlip().x != dojo::GameObject2D::FACE_LEFT) {
            _object->flipx();
        }
        if (animationTimePassed > animationSpeed) {
            lastAnimationTime = std::chrono::steady_clock::now();
            if (_object->currentAnimation() == "Walking") {
                if(!_object->nextFrame()) {
                    _object->resetAnimation();
                }
            } else {
                _object->setAnimation("Walking");
                _object->resetAnimation();
            }
        }

        _object->setPos(_object->getPos() + glm::vec3(-movementSpeed, 0, 0));
    }

    else {
        if (_object->currentAnimation() != "Idle"){
            _object->setAnimation("Idle");
            _object->resetAnimation();
        } else {
            if  (animationTimePassed > animationSpeed) {
                lastAnimationTime = std::chrono::steady_clock::now();
                if (!_object->nextFrame()) {
                    _object->resetAnimation();
                }
            }
        }
    }

    lastUpdateTime = std::chrono::steady_clock::now();
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

    dojo::GameObject2DAnimated *obj1 = new dojo::GameObject2DAnimated("Enchantress/Idle.png", 1, 5, "Idle");
    obj1->addAnimation("Walking", "Enchantress/Walk.png", 1, 8);
    obj1->setScale(glm::vec3(2));
    dojo::BoxCollider *box1 = new dojo::BoxCollider(obj1->getPos(), obj1->getScale());

    dojo::GameObject2DStatic *obj2 = new dojo::GameObject2DStatic("stick_man.png");
    obj2->setScale(glm::vec3(0.75));
    dojo::BoxCollider *box2 = new dojo::BoxCollider(obj2->getPos(), obj2->getScale());

    //dojo::GameObject2DStatic *obj = new dojo::GameObject2DStatic("stick_man.jpg");
    //renderer->setShader("2DStatic");

    std::cout << "created game object" << std::endl;

    bool running = true;

    unsigned int frames = 0;

    auto start = std::chrono::steady_clock::now();

    float frametimeMilliseconds = 1000.f/60.f;

    while(running) {

        auto frameStart = std::chrono::steady_clock::now();
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

        HandleInputs(w, obj1);
        obj2->flipx();

        frames ++;
        auto frameEnd = std::chrono::steady_clock::now();

        auto frameElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();
        
        if (frameElapsed < frametimeMilliseconds) {
            int sleepTime = frametimeMilliseconds - frameElapsed;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        }
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
