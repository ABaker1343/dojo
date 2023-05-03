#include "placement.hpp"

Placement::CameraWrapper::CameraWrapper(dojo::Renderer* _renderer) {
    m_camera = std::make_unique<dojo::Camera>();
    m_camera->setPos(glm::vec3(0, 0, 5));
    m_camera->lookAt(glm::vec3(0));

    dojo::Texture* infotexture = new dojo::Texture(glm::vec2(500, 200));
    m_info = std::make_unique<dojo::MenuItem>(glm::vec2(0.75, 0), glm::vec2(0.25, 0.05), infotexture);

    updateInfo(_renderer);
}

void Placement::CameraWrapper::updateInfo(dojo::Renderer* _renderer) {
    std::stringstream stream;
    stream << "CameraPos: " << m_camera->getPosition();
    _renderer->textToTexture(m_info->getTexture(), stream.str(), glm::vec4(0.7, 0.7, 1, 1));
}

Placement::Placement(int argc, const char* argv[]) {
    m_window = std::make_unique<dojo::Window>(1280, 720, "dojo plcement");
    m_renderer = std::make_unique<dojo::Renderer>(m_window.get());
    m_renderer->setShaderPath("shaders/");
    m_renderer->loadShaders();
    m_cameraWrap = std::make_unique<CameraWrapper>(m_renderer.get());

    auto rendererRawPointer = m_renderer.get();
    m_window->setCustomResizeCallback([rendererRawPointer](int _x, int _y){
                rendererRawPointer->resize();
            });

    m_objects = std::vector<ObjectWrapper*>();

    for (int i = 1; i < argc; i++) {
        const std::string path(argv[i]);
        ObjectWrapper* newObject = new ObjectWrapper(path, glm::vec2(0.75, 1-0.05*i), m_renderer.get());
        newObject->setPos(glm::vec3(0));
        newObject->setScale(glm::vec3(1));

        m_objects.push_back(newObject);
    }

    m_defaultColor = glm::vec4(1);
    m_selectedColor = glm::vec4(1, 0.5, 0.5, 1.0);

}

Placement::~Placement() {
    for (size_t i = 0; i < m_objects.size(); i++) {
        delete m_objects[i];
    }
}
    
void Placement::run() {
    m_selectedObject = m_objects[0];
    m_selectedObject->setColor(m_selectedColor);

    unsigned int keyCallback = bindKeyCallback();

    while(!m_window->shouldClose()) {
        m_window->pollEvents();
        update();
        m_renderer->clear();
        for (size_t i = 0; i < m_objects.size(); i++) {
            m_renderer->draw(m_cameraWrap->m_camera.get(), m_objects[i]->getObject());
        }
        for (size_t i = 0; i < m_objects.size(); i++) {
            m_renderer->draw(m_objects[i]->getInfoItem());
        }
        m_renderer->draw(m_cameraWrap->m_info.get());
        m_window->flipBuffers();
    }

    m_window->removeKeyCallback(keyCallback);

}

void Placement::update() {
    if (m_window->KEYS[GLFW_KEY_W]) {
        m_selectedObject->setPos(m_selectedObject->getPos() + glm::vec3(0, 0.1, 0));
    }
    if (m_window->KEYS[GLFW_KEY_S]) {
        m_selectedObject->setPos(m_selectedObject->getPos() + glm::vec3(0, -0.1, 0));
    }
    if (m_window->KEYS[GLFW_KEY_A]) {
        m_selectedObject->setPos(m_selectedObject->getPos() + glm::vec3(-0.1, 0, 0));
    }
    if (m_window->KEYS[GLFW_KEY_D]) {
        m_selectedObject->setPos(m_selectedObject->getPos() + glm::vec3(+0.1, 0, 0));
    }
    if (m_window->KEYS[GLFW_KEY_UP]) {
        m_cameraWrap->m_camera->setPos(m_cameraWrap->m_camera->getPosition() + glm::vec3(0, 0, -0.1));
        m_cameraWrap->updateInfo(m_renderer.get());
    }
    if (m_window->KEYS[GLFW_KEY_DOWN]) {
        m_cameraWrap->m_camera->setPos(m_cameraWrap->m_camera->getPosition() + glm::vec3(0, 0, 0.1));
        m_cameraWrap->updateInfo(m_renderer.get());
    }
    if (m_window->KEYS[GLFW_KEY_LEFT]) {
        m_cameraWrap->m_camera->setPos(m_cameraWrap->m_camera->getPosition() + glm::vec3(-0.1, 0, 0));
        m_cameraWrap->updateInfo(m_renderer.get());
    }
    if (m_window->KEYS[GLFW_KEY_RIGHT]) {
        m_cameraWrap->m_camera->setPos(m_cameraWrap->m_camera->getPosition() + glm::vec3(0.1, 0, 0));
        m_cameraWrap->updateInfo(m_renderer.get());
    }


}

unsigned int Placement::bindKeyCallback() {
    dojo::KeyCallbackFunction callback = [this](int _key, int _scancode, int _action, int _mods) {
        switch(_action) {
            case GLFW_PRESS: {
                switch(_key) {
                    case GLFW_KEY_N: {
                        m_selectedObject->setColor(m_defaultColor);
                        m_selectedIndex++;
                        if (m_selectedIndex >= m_objects.size()) {
                            m_selectedIndex = 0;
                        }
                        m_selectedObject = m_objects[m_selectedIndex];
                        m_selectedObject->setColor(m_selectedColor);
                    }
                }
                }
            }
            };

    
    unsigned int callbackHandle = m_window->setCustomKeyCallback(callback);

    return callbackHandle;
}
