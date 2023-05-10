#include "../../include/dojo.hpp"

void updateCamera(dojo::Window* _window, dojo::Camera* _camera);

int main() {
    
    dojo::Window* window = new dojo::Window(1280, 720, "ecs demo");
    dojo::Renderer* renderer = new dojo::Renderer(window);
    renderer->setShaderPath("../../src/shaders/");
    renderer->loadShaders();
    dojo::Camera* camera = new dojo::Camera();
    camera->setPos(glm::vec3(0, 5, 10));

    dojo::Entity* entity2d = new dojo::Entity();
    dojo::StaticSpriteComponent* sprite = new dojo::StaticSpriteComponent("../stick_man.png");
    dojo::TransformComponent* transform = new dojo::TransformComponent();
    entity2d->addComponent(sprite, dojo::Component::StaticSpriteBit);
    entity2d->addComponent(transform, dojo::Component::TransformBit);

    dojo::Entity* entity3d = new dojo::Entity();
    dojo::StaticMeshComponent* mesh = new dojo::StaticMeshComponent("../../bench/Bench.obj");
    dojo::TransformComponent* transform3d = new dojo::TransformComponent();
    transform3d->setPos(glm::vec3(-10, 0, 0))
            ->setScale(glm::vec3(1));
    entity3d->addComponent(mesh, dojo::Component::StaticMeshBit);
    entity3d->addComponent(transform3d, dojo::Component::TransformBit);

    dojo::Entity* entityLight = new dojo::Entity();
    dojo::TransformComponent* transformLight = new dojo::TransformComponent();
    transformLight->setPos(glm::vec3(-10, 5, 0))
        ->setScale(glm::vec3(0.2));
    std::cout << transform3d->getPos().x << std::endl;
    dojo::DirectionalLightComponent* emmitLight = new dojo::DirectionalLightComponent(transform3d->getPos());
    emmitLight->setPos(transformLight->getPos());
    dojo::StaticMeshComponent* ligthmesh = new dojo::StaticMeshComponent("../PS1MemoryCard_OBJ/MemoryCard.obj");
    
    dojo::MenuItem* map = new dojo::MenuItem(glm::vec2(0.2), glm::vec2(0.2), emmitLight->getShadowMap());

    assert(entity2d->hasComponents(dojo::Component::TransformBit | dojo::Component::StaticSpriteBit));

    while(!window->shouldClose()) {
        window->pollEvents();
        updateCamera();
        renderer->clear(emmitLight);
        renderer->clear();

        renderer->draw(emmitLight, mesh, transform3d);

        renderer->draw(camera, emmitLight, transformLight, mesh, transform3d);
        renderer->draw(camera, ligthmesh, transformLight);

        renderer->draw(camera, sprite, transform);

        renderer->draw(map);

        window->flipBuffers();
    }

    delete entity2d;
    delete entity3d;
    delete camera;
    delete renderer;
    delete window;


    return 0;
}

void updateCamera(dojo::Window* _window, dojo::Camera* _camera) {
    if (_window->KEYS[GLFW_KEY_S]) {
        _camera->move(-1, dojo::Camera::FORWARD);
    }
}
