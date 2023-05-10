#include "../../include/dojo.hpp"

void updateCamera(dojo::Window* _window, dojo::Camera* _camera);
void updateLight(dojo::Window* _window, dojo::Entity* _light);

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
    std::cout << transform3d->getPos().x << std::endl;
    dojo::DirectionalLightComponent* emmitLight = new dojo::DirectionalLightComponent();
    dojo::ViewerTransformComponent* lightTransform = new dojo::ViewerTransformComponent();
    lightTransform->setPos(glm::vec3(-25, 15, 0))->setScale(glm::vec3(0.2));
    dojo::StaticMeshComponent* lightmesh = new dojo::StaticMeshComponent("../PS1MemoryCard_OBJ/MemoryCard.obj");
    entityLight->addComponent(emmitLight, dojo::Component::LightBit);
    entityLight->addComponent(lightTransform, dojo::Component::TransformBit);
    entityLight->addComponent(lightmesh, dojo::Component::StaticMeshBit);

    assert(entity2d->hasComponents(dojo::Component::TransformBit | dojo::Component::StaticSpriteBit));

    while(!window->shouldClose()) {
        window->pollEvents();
        updateCamera(window, camera);
        updateLight(window, entityLight);
        renderer->clear(emmitLight);
        renderer->clear();

        // draw to shadow maps
        renderer->draw(emmitLight, lightTransform, mesh, transform3d);

        // draw components using the shadow map
        renderer->draw(camera, emmitLight, lightTransform, mesh, transform3d);
        renderer->draw(camera, lightmesh, lightTransform);

        // these components are not drawn with shadow maps
        renderer->draw(camera, sprite, transform);

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
    if (_window->KEYS[GLFW_KEY_L]) {
        return;
    }

    if (_window->KEYS[GLFW_KEY_S]) {
        _camera->move(-1, dojo::Camera::FORWARD);
    } else if (_window->KEYS[GLFW_KEY_W]) {
        _camera->move(1, dojo::Camera::FORWARD);
    }

    if (_window->KEYS[GLFW_KEY_D]) {
        _camera->move(1, dojo::Camera::RIGHT);
    } else if (_window->KEYS[GLFW_KEY_A]) {
        _camera->move(-1, dojo::Camera::RIGHT);
    }

    if (_window->KEYS[GLFW_KEY_SPACE]) {
        _camera->move(1, dojo::Camera::UP);
    } else if (_window->KEYS[GLFW_KEY_Z]) {
        _camera->move(-1, dojo::Camera::UP);
    }
}

void updateLight(dojo::Window* _window, dojo::Entity* _light) {
    if (!_window->KEYS[GLFW_KEY_L]) {
        return;
    }

    dojo::TransformComponent* transform = (dojo::TransformComponent*)(_light->getComponent(dojo::Component::TransformBit));

    if (_window->KEYS[GLFW_KEY_W]) {
        transform->setPos(transform->getPos() + glm::vec3(0, 0, -1));
    } else if (_window->KEYS[GLFW_KEY_S]) {
        transform->setPos(transform->getPos() + glm::vec3(0, 0, 1));
    }

    if (_window->KEYS[GLFW_KEY_A]) {
        transform->setPos(transform->getPos() + glm::vec3(-1, 0, 0));
    } else if (_window->KEYS[GLFW_KEY_D]) {
        transform->setPos(transform->getPos() + glm::vec3(1, 0, 0));
    }

    if (_window->KEYS[GLFW_KEY_SPACE]) {
        transform->setPos(transform->getPos() + glm::vec3(0, 1, 0));
    } else if (_window->KEYS[GLFW_KEY_Z]) {
        transform->setPos(transform->getPos() + glm::vec3(0, -1, 0));
    }
}
