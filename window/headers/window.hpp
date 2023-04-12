#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <stdexcept>

namespace dojo {

class Window {
    public:
        Window(int _width, int _height, std::string& _title);
        ~Window();

        void flipBuffers();
        void pollEvents();

        static bool KEYS[GLFW_KEY_LAST + 1];

    private:
        GLFWwindow* m_Window;

        static void windowKeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
        static void windowResizeCallback(GLFWwindow* _window, int _width, int _height);

        void initWindow(int _width, int _height, std::string& _title);
        void setWindowCallbacks();
        
};

}
