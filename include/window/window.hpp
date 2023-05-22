#ifndef __HEADER_WINDOW
#define __HEADER_WINDOW

#include "../glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <stdexcept>
#include <vector>
#include <functional>
#include <iostream>
#include <algorithm>
#include <optional>

namespace dojo {

class Window;
class Renderer;

typedef std::function<void(int _key, int _scancode, int _action, int _mods)> KeyCallbackFunction;
typedef std::function<void(int _button, int _action, int _mods)> MouseCallbackFunction;
typedef std::function<void(double _x, double _y)> CursorPosCallbackFunction;
typedef std::function<void(int _width, int _height)> ResizeCallbackFunction;

typedef struct {
    unsigned int ID;
    KeyCallbackFunction function;
} KeyCallback;
typedef struct {
    unsigned int ID;
    MouseCallbackFunction function;
} MouseCallback;
typedef struct {
    unsigned int ID;
    CursorPosCallbackFunction function;
} CursorPosCallback;
typedef struct {
    unsigned int ID;
    ResizeCallbackFunction function;
} ResizeCallback;

class Window {
    public:
        Window(int _width, int _height, const std::string& _title);
        ~Window();

        void flipBuffers();
        void pollEvents();

        glm::vec2 getDimensions();
        glm::vec2 getMousePos();

        bool shouldClose();

        void useDefaultKeyCallback(bool _use);
        unsigned int setCustomKeyCallback(KeyCallbackFunction _callback);
        void removeKeyCallback(unsigned int _callbackID);

        void useDefaultMouseCallback(bool _use);
        unsigned int setCustomMouseCallback(MouseCallbackFunction _callback);
        void removeMouseCallback(unsigned int _callbackID);

        void useDefaultCursorPosCallback(bool _use);
        unsigned int setCustomCursorPosCallback(CursorPosCallbackFunction _callback);
        void removeCursorPosCallback(unsigned int _callbackID);

        void useDefaultResizeCallback(bool _use);
        unsigned int setCustomResizeCallback(ResizeCallbackFunction _callback);
        void removeResizeCallback(unsigned int _callbackID);

        bool KEYS[GLFW_KEY_LAST + 1];
        bool MOUSE_BUTTONS[GLFW_MOUSE_BUTTON_LAST + 1];

        void bindRenderer(Renderer* _renderer);

    private:
        GLFWwindow* m_window;
        std::vector<Renderer*> m_activeRenderers;
        
        glm::vec2 m_mousePos;

        bool m_usingDefaultKeyCallback;
        bool m_usingDefaultMouseCallback;
        bool m_usingDefaultCursorCallback;
        bool m_usingDefaultResizeCallback;

        void bindDefaultKeyCallback();

        unsigned int genNewCallbackID();
        static void windowKeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
        static void windowResizeCallback(GLFWwindow* _window, int _width, int _height);
        static void windowMouseCallback(GLFWwindow* _window, int _button, int _action, int _mods);
        static void windowCursorPosCallback(GLFWwindow *_window, double _xpos, double _ypos);
        static KeyCallbackFunction m_defaultKeyCallback;
        std::vector<KeyCallback> m_keyCallbacks;
        std::vector<MouseCallback> m_mouseCallbacks;
        std::vector<CursorPosCallback> m_cursorPosCallbacks;
        std::vector<ResizeCallback> m_resizeCallbacks;

        void initWindow(int _width, int _height, const std::string& _title);
        void setWindowCallbacks();
        
};

}


#endif
