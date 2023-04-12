#include "headers/window.hpp"

namespace dojo {

bool Window::KEYS[GLFW_KEY_LAST + 1];

Window::Window(int _width, int _height, const std::string& _title){

    initWindow(_width, _height, _title);

    // initialize keys
    for (int i = 0; i < GLFW_KEY_LAST + 1; i++) {
        KEYS[i] = false;
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

}

void Window::setWindowCallbacks() {
    glfwSetKeyCallback(m_Window, windowKeyCallback);
    glfwSetWindowSizeCallback(m_Window, windowResizeCallback);
    glfwSetFramebufferSizeCallback(m_Window, windowResizeCallback);
}

void Window::windowKeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods) {
    // when a key is pressed add it to the array
    if (_action == GLFW_PRESS) {
        KEYS[_key] = true;
    }
    if (_action == GLFW_RELEASE) {
        KEYS[_key] = false;
    }
}

void Window::flipBuffers() {
    glfwSwapBuffers(m_Window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

glm::vec2 Window::getDimensions() {
    int width, height;
    glfwGetWindowSize(m_Window, &width, &height);
    return glm::vec2(width, height);
}

void Window::windowResizeCallback(GLFWwindow* _window, int _width, int _height) {

}

Window::~Window() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

}
