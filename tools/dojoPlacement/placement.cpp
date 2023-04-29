#include "placement.hpp"


Placement::Placement(int argc, const char* argv[]) {
    m_window = std::make_unique<dojo::Window>(1280, 720, "dojo plcement");
    m_renderer = std::make_unique<dojo::Renderer>(m_window.get());
    m_renderer->setShaderPath("shaders/");
    m_renderer->loadShaders();
    m_camera = std::make_unique<dojo::Camera>();
    m_camera->setPos(glm::vec3(0,0,5));
    m_camera->lookAt(glm::vec3(0));

    auto rendererRawPointer = m_renderer.get();
    m_window->setCustomResizeCallback([rendererRawPointer](int _x, int _y){
                rendererRawPointer->resize();
            });

    m_objects = std::vector<ObjectWrapper*>();

    for (int i = 1; i < argc; i++) {
        const std::string path(argv[i]);
        ObjectWrapper* newObject = new ObjectWrapper(path, glm::vec2(0.75, 1-0.05*1), m_renderer.get());
        newObject->setPos(glm::vec3(0));
        newObject->setScale(glm::vec3(1));

        m_objects.push_back(newObject);
    }

}

Placement::~Placement() {
    for (size_t i = 0; i < m_objects.size(); i++) {
        delete m_objects[i];
    }
}
    
void Placement::run() {
    m_selectedObject = m_objects[0];

    while(!m_window->shouldClose()) {
        m_window->pollEvents();
        update();
        m_renderer->clear();
        m_renderer->setShader("2DStatic");
        for (size_t i = 0; i < m_objects.size(); i++) {
            m_renderer->draw(m_camera.get(), m_objects[i]->getObject());
        }
        m_renderer->setShader("menuShader");
        for (size_t i = 0; i < m_objects.size(); i++) {
            m_renderer->draw(m_objects[i]->getInfoItem());
        }
        m_window->flipBuffers();
    }
}

void Placement::update() {
    if (m_window->KEYS[GLFW_KEY_N]) {
        if(m_selectedIndex >= m_objects.size()) {
            m_selectedObject = m_objects[++m_selectedIndex];
        } else {
            m_selectedObject = m_objects[0];
        }
    }
    if (m_window->KEYS[GLFW_KEY_W] || m_window->KEYS[GLFW_KEY_UP]) {
        m_selectedObject->setPos(m_selectedObject->getPos() + glm::vec3(0, 0.1, 0));
    }

}
