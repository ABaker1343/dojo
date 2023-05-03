#include "../../include/window/window.hpp"
#include <GLFW/glfw3.h>

namespace dojo {

bool Window::KEYS[GLFW_KEY_LAST + 1];
bool Window::MOUSE_BUTTONS[GLFW_MOUSE_BUTTON_LAST + 1];
glm::vec2 Window::m_MousePos = glm::vec2();

std::vector<KeyCallback> Window::m_keyCallbacks = std::vector<KeyCallback>();
std::vector<MouseCallback> Window::m_mouseCallbacks = std::vector<MouseCallback>();
std::vector<CursorPosCallback> Window::m_cursorPosCallbacks = std::vector<CursorPosCallback>();
std::vector<ResizeCallback> Window::m_resizeCallbacks = std::vector<ResizeCallback>();

bool Window::m_usingDefaultKeyCallback = true;
bool Window::m_usingDefaultMouseCallback = true;
bool Window::m_usingDefaultCursorCallback = true;
bool Window::m_usingDefaultResizeCallback = true;

std::vector<Renderer*> Window::m_ActiveRenderers = std::vector<Renderer*>();

Window::Window(int _width, int _height, const std::string& _title){

    initWindow(_width, _height, _title);

    // initialize keys
    for (int i = 0; i <= GLFW_KEY_LAST; i++) {
        KEYS[i] = false;
    }
    for (unsigned int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i++) {
        MOUSE_BUTTONS[i] = false;
    }

    // set window callback
    setWindowCallbacks();

}

void Window::initWindow(int _width, int _height, const std::string& _title) {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // create a glfw window
    m_Window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);

    if (m_Window == NULL){
        glfwTerminate();
        throw std::runtime_error("failed to create glfw window");
    }
    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("failed to initialize glad");
    }

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

}

void Window::setWindowCallbacks() {
    glfwSetKeyCallback(m_Window, windowKeyCallback);
    glfwSetWindowSizeCallback(m_Window, windowResizeCallback);
    glfwSetMouseButtonCallback(m_Window, windowMouseCallback);
    glfwSetCursorPosCallback(m_Window, windowCursorPosCallback);
    glfwSetFramebufferSizeCallback(m_Window, windowResizeCallback);
}

unsigned int Window::setCustomKeyCallback(KeyCallbackFunction _callback) {
    KeyCallback newCallback = KeyCallback {
        genNewCallbackID(),
        _callback
    };
    m_keyCallbacks.push_back(newCallback);
    return newCallback.ID;
}
unsigned int Window::setCustomMouseCallback(MouseCallbackFunction _callback) {
    MouseCallback newCallback = {
        genNewCallbackID(),
        _callback
    };
    m_mouseCallbacks.push_back(newCallback);
    return newCallback.ID;
}
unsigned int Window::setCustomCursorPosCallback(CursorPosCallbackFunction _callback) {
    CursorPosCallback newCallback = {
        genNewCallbackID(),
        _callback
    };
    m_cursorPosCallbacks.push_back(newCallback);
    return newCallback.ID;
}
unsigned int Window::setCustomResizeCallback(ResizeCallbackFunction _callback) {
    ResizeCallback newCallback = {
        genNewCallbackID(),
        _callback
    };
    m_resizeCallbacks.push_back(newCallback);
    return newCallback.ID;
}

void Window::removeKeyCallback(unsigned int _callbackID) {
    std::vector<KeyCallback>::iterator it;
    for (it = m_keyCallbacks.begin(); it != m_keyCallbacks.end(); it++) {
        if (it->ID == _callbackID) {
            m_keyCallbacks.erase(it);
            break;
        }
    }
}
void Window::removeMouseCallback(unsigned int _callbackID) {
    std::vector<MouseCallback>::iterator it;
    for (it = m_mouseCallbacks.begin(); it != m_mouseCallbacks.end(); it++) {
        if (it->ID == _callbackID) {
            m_mouseCallbacks.erase(it);
            break;
        }
    }
}
void Window::removeCursorPosCallback(unsigned int _callbackID) {
    std::vector<CursorPosCallback>::iterator it;
    for (it = m_cursorPosCallbacks.begin(); it != m_cursorPosCallbacks.end(); it++) {
        if (it->ID == _callbackID) {
            m_cursorPosCallbacks.erase(it);
            break;
        }
    }
}
void Window::removeResizeCallback(unsigned int _callbackID) {
    std::vector<ResizeCallback>::iterator it;
    for (it = m_resizeCallbacks.begin(); it != m_resizeCallbacks.end(); it++) {
        if (it->ID == _callbackID) {
            m_resizeCallbacks.erase(it);
            break;
        }
    }
}

unsigned int Window::genNewCallbackID() {
    static unsigned int lastID;
    return lastID++;
}

void Window::useDefaultKeyCallback(bool _use) {
    m_usingDefaultKeyCallback = _use;
}
void Window::useDefaultMouseCallback(bool _use) {
    m_usingDefaultMouseCallback = _use;
}
void Window::useDefaultCursorPosCallback(bool _use) {
    m_usingDefaultCursorCallback = _use;
}
void Window::useDefaultResizeCallback(bool _use) {
    m_usingDefaultResizeCallback = _use;
}

void Window::windowKeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods) {
    // when a key is pressed add it to the array
    
    if (m_usingDefaultKeyCallback) {
        switch (_action) {
            case GLFW_PRESS:
                KEYS[_key] = true;
                break;
            case GLFW_RELEASE:
                KEYS[_key] = false;
                break;
        }
    }
    

    for (unsigned int i = 0; i < m_keyCallbacks.size(); i++){
        m_keyCallbacks[i].function(_key, _scancode, _action, _mods);
    }
}

void Window::windowMouseCallback(GLFWwindow *_window, int _button, int _action, int _mods) {
    if (m_usingDefaultMouseCallback) {
        switch (_action) {
            case GLFW_PRESS:
                MOUSE_BUTTONS[_button] = true;
                break;
            case GLFW_RELEASE:
                MOUSE_BUTTONS[_button] = false;
        }
    }

    for (unsigned int i = 0; i < m_mouseCallbacks.size(); i++) {
        m_mouseCallbacks[i].function(_button, _action, _mods);
    }
}

void Window::windowCursorPosCallback(GLFWwindow *_window, double _xpos, double _ypos) {
    if (m_usingDefaultCursorCallback) {
        m_MousePos.x = _xpos;
        m_MousePos.y = _ypos;
    }

    for (unsigned int i = 0; i < m_cursorPosCallbacks.size(); i++) {
        m_cursorPosCallbacks[i].function(_xpos, _ypos);
    }
}

void Window::windowResizeCallback(GLFWwindow* _window, int _width, int _height) {
    if (m_usingDefaultResizeCallback) {

    }

    for (unsigned int i = 0; i < m_resizeCallbacks.size(); i++) {
        m_resizeCallbacks[i].function(_width, _height);
    }
}

void Window::flipBuffers() {
    glfwSwapBuffers(m_Window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

glm::vec2 Window::getMousePos() {
    return m_MousePos;
}

glm::vec2 Window::getDimensions() {
    int width, height;
    glfwGetWindowSize(m_Window, &width, &height);
    return glm::vec2(width, height);
}

void Window::bindRenderer(Renderer* _renderer) {
    m_ActiveRenderers.push_back(_renderer);
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(m_Window);
}

Window::~Window() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

}
