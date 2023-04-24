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
    
    float movementSpeed = 3 * (timePassed / 1000.f); // seconds
    float animationSpeed = 1000.f / 5.f; // frames per seconds

    if (_window->KEYS[GLFW_KEY_D]) {
        if (_object->getFlip().x != dojo::GameObject2D::FACE_RIGHT) {
            _object->flipx();
        }

        if (_object->currentAnimation() != "Walking") {
            _object->setAnimation("Walking");
            _object->resetAnimation();
        }
        else if (animationTimePassed > animationSpeed) {
            lastAnimationTime = std::chrono::steady_clock::now();
            if(!_object->nextFrame()) {
                _object->resetAnimation();
            }
        }

        _object->setPos(_object->getPos() + glm::vec3(movementSpeed, 0, 0));
    }

    else if (_window->KEYS[GLFW_KEY_A]) {
        if (_object->getFlip().x != dojo::GameObject2D::FACE_LEFT) {
            _object->flipx();
        }

        if (_object->currentAnimation() != "Walking") {
            _object->setAnimation("Walking");
            _object->resetAnimation();
        }
        else if (animationTimePassed > animationSpeed) {
            lastAnimationTime = std::chrono::steady_clock::now();
            if(!_object->nextFrame()) {
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

    /*w->setCustomKeyCallback([w] (int _key, int _scancode, int _action, int _mode) -> void {
            if (_action == GLFW_PRESS) {
                w->KEYS[_key] = true;
            }
            if (_action == GLFW_RELEASE) {
                w->KEYS[_key] = false;
            }
        });*/
    w->useDefaultKeyCallback(true);

    std::cout << "created window" << std::endl;

    dojo::Renderer *renderer = new dojo::Renderer(w);
    renderer->setShaderPath("../shaders/");
    renderer->loadShaders();

    w->setCustomResizeCallback([renderer](int _width, int _height) {
            renderer->resize();
            });

    dojo::Camera* cam = new dojo::Camera();
    cam->setPos(glm::vec3(0.f, 0.f, 15.f));
    cam->lookAt(glm::vec3(0.f));

    std::cout << "created renderer" << std::endl;

    dojo::GameObject2DStatic *background = new dojo::GameObject2DStatic("Final/Social/test_2.png");
    background->setScale(glm::vec3(40, 20, 1));
    background->setPos(glm::vec3(0, 0, -10));

    dojo::GameObject2DAnimated *obj1 = new dojo::GameObject2DAnimated("Enchantress/Idle.png", 1, 5, "Idle");
    obj1->addAnimation("Walking", "Enchantress/Walk.png", 1, 8);
    obj1->setScale(glm::vec3(3));
    obj1->setPos(glm::vec3(0, -5, 0));
    dojo::BoxCollider *box1 = new dojo::BoxCollider(obj1->getPos(), obj1->getScale());

    dojo::GameObject2DStatic *obj2 = new dojo::GameObject2DStatic("stick_man.png");
    obj2->setScale(glm::vec3(0.75));
    dojo::BoxCollider *box2 = new dojo::BoxCollider(obj2->getPos(), obj2->getScale());
    obj2->setPos(obj2->getPos() + glm::vec3(0, 0, -1));

    dojo::Texture* tex = new dojo::Texture(glm::ivec2(500));
    renderer->drawToTexture(tex, "string", 0, 0 ,1);
    //dojo::MenuItem* menuItem = new dojo::MenuItem(glm::vec2(25), glm::vec2(2), tex);
    dojo::MenuItem* menuItem = new dojo::MenuItem(glm::vec2(25), glm::vec2(2), obj1->getTexture());
    

    //dojo::GameObject2DStatic *obj = new dojo::GameObject2DStatic("stick_man.jpg");
    //renderer->setShader("2DStatic");
    
    std::cout << "created game object" << std::endl;

    bool running = true;

    unsigned int frames = 0;

    auto start = std::chrono::steady_clock::now();

    while(running) {
        renderer->clear();
        renderer->setShader("2DStatic");
        renderer->draw(cam, background);
        renderer->draw(cam, obj2);
        renderer->setShader("2DAnimated");
        renderer->draw(cam, obj1);
        renderer->setShader("menuShader");
        renderer->draw(menuItem);
        w->flipBuffers();
        w->pollEvents();
        if (w->KEYS[GLFW_KEY_ESCAPE]) {
            running = false;
        }

        HandleInputs(w, obj1);
        obj2->flipx();

        frames ++;
    }

    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end-start);
    std::cout << "frames per second: " << frames / elapsed.count() << std::endl;

    std::cout << "deleting object 1 static" << std::endl;
    delete obj1;
    std::cout << "deleting object 2 static" << std::endl;
    delete obj2;
    delete menuItem;
    std::cout << "deleting boxes" << std::endl;
    delete box1;
    delete box2;
    std::cout << "deleting renderer" << std::endl;
    delete renderer;
    std::cout << "deleting window" << std::endl;
    delete w;

}
