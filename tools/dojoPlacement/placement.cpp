#include <cstddef>
#include <dojo/dojo.hpp>
#include <vector>
#include <iostream>
#include <memory>

int main(int argc, const char* argv[]) {


    std::unique_ptr<dojo::Window> window = std::make_unique<dojo::Window>(1280, 720, "dojo plcement");
    std::unique_ptr<dojo::Renderer> renderer = std::make_unique<dojo::Renderer>(window.get());
    renderer->setShaderPath("shaders/");
    renderer->loadShaders();
    std::unique_ptr<dojo::Camera> camera = std::make_unique<dojo::Camera>();
    camera->setPos(glm::vec3(0,0,5));
    camera->lookAt(glm::vec3(0));

    std::vector<dojo::GameObject2DStatic> objects = std::vector<dojo::GameObject2DStatic>();

    for (int i = 1; i < argc; i++) {
        const std::string path(argv[i]);
        dojo::GameObject2DStatic newObject = dojo::GameObject2DStatic(path);
        newObject.setPos(glm::vec3(0));
        newObject.setScale(glm::vec3(1));
        objects.push_back(newObject);
    }


    renderer->setShader("2DStatic");
    while(!window->shouldClose()) {
        window->pollEvents();
        renderer->clear();
        for (size_t i = 0; i < objects.size(); i++) {
            renderer->draw(camera.get(), &(objects[i]));
        }
        window->flipBuffers();
    }


}
