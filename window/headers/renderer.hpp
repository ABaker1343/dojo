#ifndef __HEADER_RENDERER
#define __HEADER_RENDERER

#include <iostream>
#include <string>
#include <exception>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.hpp"
#include "../../gameObjects/headers/camera.hpp"
#include "../../gameObjects/headers/renderable.hpp"
#include "../../headers/fileHandler.hpp"

namespace dojo{

class Renderer {
    public:
        Renderer(Window* _window, glm::vec2 _VPSize);
        Renderer(Window* _window);
        ~Renderer();

        void draw(Camera* _camera, Renderable* _object);
        void setShader(const std::string& _shader);
        void setShaderPath(const std::string& _shaderPath);
        void loadShaders();
        void clear();

    private:

        Window* m_Window;
        std::string m_ShaderPath;
        unsigned int m_CurrentShader;

        std::map<std::string, unsigned int> m_Shaders;

        

};

}

#endif
