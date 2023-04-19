#ifndef __HEADER_WINDOW
#define __HEADER_WINDOW

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <stdexcept>
#include <vector>
#include <functional>
#include <iostream>
#include <algorithm>

namespace dojo {

class Window;

//typedef void(*keyCallback) (int _key, int _scancode, int _action, int _mode);
typedef std::function<void(int, int, int, int)> keyCallback;

class Window {
    public:
        Window(int _width, int _height, const std::string& _title);
        ~Window();

        void flipBuffers();
        void pollEvents();

        void useDefaultKeyCallback(bool _use);
        void setCustomKeyCallback(keyCallback);

        glm::vec2 getDimensions();

        static bool KEYS[GLFW_KEY_LAST + 1];

    private:
        GLFWwindow* m_Window;

        static bool m_usingDefaultKeyCallback;

        static void windowKeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
        static void windowResizeCallback(GLFWwindow* _window, int _width, int _height);
        static keyCallback m_defaultKeyCallback;
        static std::vector<keyCallback> m_keyCallbacks;

        void initWindow(int _width, int _height, const std::string& _title);
        void setWindowCallbacks();
        
};

}


#endif
