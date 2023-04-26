#ifndef __HEADER_WINDOW
#define __HEADER_WINDOW

#include "../../headers/glad/glad.h"
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

typedef std::function<void(int, int, int, int)> KeyCallbackFunction;
typedef std::function<void(int, int, int)> MouseCallbackFunction;
typedef std::function<void(double, double)> CursorPosCallbackFunction;
typedef std::function<void(int, int)> ResizeCallbackFunction;

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

        glm::vec2 getDimensions();
        glm::vec2 getMousePos();

        static bool KEYS[GLFW_KEY_LAST + 1];
        static bool MOUSE_BUTTONS[GLFW_MOUSE_BUTTON_LAST + 1];

        static void bindRenderer(Renderer* _renderer);

    private:
        GLFWwindow* m_Window;
        static std::vector<Renderer*> m_ActiveRenderers;
        
        static glm::vec2 m_MousePos;

        static bool m_usingDefaultKeyCallback;
        static bool m_usingDefaultMouseCallback;
        static bool m_usingDefaultCursorCallback;
        static bool m_usingDefaultResizeCallback;

        static unsigned int genNewCallbackID();
        static void windowKeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
        static void windowResizeCallback(GLFWwindow* _window, int _width, int _height);
        static void windowMouseCallback(GLFWwindow* _window, int _button, int _action, int _mods);
        static void windowCursorPosCallback(GLFWwindow *_window, double _xpos, double _ypos);
        static KeyCallback m_defaultKeyCallback;
        static std::vector<KeyCallback> m_keyCallbacks;
        static std::vector<MouseCallback> m_mouseCallbacks;
        static std::vector<CursorPosCallback> m_cursorPosCallbacks;
        static std::vector<ResizeCallback> m_resizeCallbacks;

        void initWindow(int _width, int _height, const std::string& _title);
        void setWindowCallbacks();
        
};

}


#endif
