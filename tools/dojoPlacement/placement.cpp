#include "placement.hpp"

Placement::Placement(int argc, const char* argv[]) {
    m_window = std::make_unique<dojo::Window>(1280, 720, "dojo plcement");
    m_renderer = std::make_unique<dojo::Renderer>(m_window.get());
    m_renderer->setShaderPath("shaders/");
    m_renderer->loadShaders();
    m_cameraWrap = std::make_unique<CameraWrapper>(m_renderer.get());

    m_window->setCustomResizeCallback([this](int _x, int _y){
                m_renderer->resize();
            });

    m_objects = std::vector<ObjectWrapper*>();

    m_menuStackHeight = 0.05;
    for (int i = 1; i < argc; i++) {
        const std::string path(argv[i]);
        ObjectWrapper* newObject = new ObjectWrapper(path, glm::vec2(0.75, 1-m_menuStackHeight), m_renderer.get());
        m_menuStackHeight += 0.05;
        newObject->setPos(glm::vec3(0));
        newObject->setScale(glm::vec3(1));

        m_objects.push_back(newObject);
    }

    m_createButton = std::make_unique<dojo::MenuItem>(glm::vec2(0.05), glm::vec2(0.1), "New", m_renderer.get());

    m_defaultColor = glm::vec4(1);
    m_selectedColor = glm::vec4(1, 0.5, 0.5, 1.0);

    m_creator = std::make_unique<ObjectCreator>(m_renderer.get());

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
    unsigned int buttonCallback = bindButtonCallback();

    while(!m_window->shouldClose()) {
        m_window->pollEvents();
        update();
        render();
        m_window->flipBuffers();
    }

    m_window->removeKeyCallback(keyCallback);
    m_window->removeMouseCallback(buttonCallback);

}

void Placement::update() {
    m_msSinceUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_lastUpdateTime);

    float movementSpeed = m_msSinceUpdate.count() * 0.005;

    if (m_window->KEYS[GLFW_KEY_UP]) {
        m_selectedObject->setPos(m_selectedObject->getPos() + glm::vec3(0, movementSpeed, 0));
    }
    if (m_window->KEYS[GLFW_KEY_DOWN]) {
        m_selectedObject->setPos(m_selectedObject->getPos() + glm::vec3(0, -movementSpeed, 0));
    }
    if (m_window->KEYS[GLFW_KEY_LEFT]) {
        m_selectedObject->setPos(m_selectedObject->getPos() + glm::vec3(-movementSpeed, 0, 0));
    }
    if (m_window->KEYS[GLFW_KEY_RIGHT]) {
        m_selectedObject->setPos(m_selectedObject->getPos() + glm::vec3(movementSpeed, 0, 0));
    }
    if (m_window->KEYS[GLFW_KEY_W]) {
        m_cameraWrap->move(movementSpeed, dojo::Camera::FORWARD, m_renderer.get());
    }
    if (m_window->KEYS[GLFW_KEY_S]) {
        m_cameraWrap->move(-movementSpeed, dojo::Camera::FORWARD, m_renderer.get());
    }
    if (m_window->KEYS[GLFW_KEY_A]) {
        m_cameraWrap->move(-movementSpeed, dojo::Camera::RIGHT, m_renderer.get());
    }
    if (m_window->KEYS[GLFW_KEY_D]) {
        m_cameraWrap->move(movementSpeed, dojo::Camera::RIGHT, m_renderer.get());
    }

    m_lastUpdateTime = std::chrono::steady_clock::now();


}

void Placement::render() {
    m_renderer->clear();
    for (size_t i = 0; i < m_objects.size(); i++) {
        auto object = m_objects[i];
        switch(object->getType()) {
            case ObjectWrapper::OBJECT_3D:
                m_renderer->draw(m_cameraWrap->m_camera.get(), (dojo::GameObject3D*)m_objects[i]->getObject());
                break;
            case ObjectWrapper::OBJECT_2D_STATIC:
                m_renderer->draw(m_cameraWrap->m_camera.get(), (dojo::GameObject2DStatic*)m_objects[i]->getObject());
                break;
            case ObjectWrapper::OBJECT_2D_ANIMATED:
                m_renderer->draw(m_cameraWrap->m_camera.get(), (dojo::GameObject2DAnimated*)m_objects[i]->getObject());
                break;
        }
    }
    for (size_t i = 0; i < m_objects.size(); i++) {
        m_renderer->draw(m_objects[i]->getInfoItem());
    }
    m_renderer->draw(m_cameraWrap->m_info.get());
    m_renderer->draw(m_createButton.get());
}

void Placement::createObject() {
    std::pair<dojo::GameObject*, ObjectWrapper::Type> objData;
    objData = m_creator->createObject();
    std::cout << m_menuStackHeight << std::endl;
    std::cout << "assined" << std::endl;
    ObjectWrapper* wrappedObject = new ObjectWrapper(std::get<0>(objData), std::get<1>(objData),
            glm::vec2(0.75, 1 - m_menuStackHeight), m_renderer.get());
    std::cout << "wrapped object" << std::endl;
    m_objects.push_back(wrappedObject);
    std::cout << "pushed" << std::endl;
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

    
    return m_window->setCustomKeyCallback(callback);
}

unsigned int Placement::bindButtonCallback() {
    dojo::MouseCallbackFunction callback = [this](int _button, int _action, int _mods) {
        glm::vec2 mousePos = m_window->getMousePos();
        glm::vec2 windowSize = m_window->getDimensions();
        switch(_action) {
            case GLFW_PRESS:
                if (m_createButton->isMouseOver(windowSize, mousePos)) {
                    createObject();
                }
                break;
        }
    };
    
    return m_window->setCustomMouseCallback(callback);
}
