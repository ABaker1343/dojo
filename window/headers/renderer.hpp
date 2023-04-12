#include <iostream>
#include <string>
#include <exception>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.hpp"
#include "../../gameObjects/headers/renderable.hpp"
#include "../../headers/fileHandler.hpp"

namespace dojo{

class Renderer {
    public:
        Renderer(Window& _window, int _VPwidth, int _VPheight);
        ~Renderer();

        void draw(Renderable& _object);
        void setShader(const std::string& _shader);

    private:

        Window* m_Window;

        std::map<std::string, unsigned int> m_Shaders;

        void loadShaders();
        

};

}
