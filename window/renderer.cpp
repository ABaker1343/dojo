#include "headers/renderer.hpp"
#include <GLFW/glfw3.h>

namespace dojo {

Renderer::Renderer(Window& _window, int _VPWidth, int _VPHeight) {

    m_Window = &_window;

    // glfw window hints

    glViewport(0, 0, _VPWidth, _VPHeight);
    loadShaders();
    setShader("default");
    
}

Renderer::~Renderer() {
    // remove all the programs
    std::map<std::string, unsigned int>::iterator it;
    for (it = m_Shaders.begin(); it != m_Shaders.end(); it++){
        glDeleteProgram(it->second);
    }
}

void Renderer::draw(Renderable& _object) {
    glBindVertexArray(_object.getVertexArray());
    glDrawArrays(GL_TRIANGLES, 0, _object.getVertexBufferSize());
}

void Renderer::setShader(const std::string& _shader) {
    unsigned int newShader = m_Shaders[_shader];
    glUseProgram(newShader);
}

void Renderer::loadShaders() {
    // load the shaders to use
    std::vector<std::string> shaderNames = {"default"};

    for (const std::string& shader : shaderNames) {
        // load the shader
        std::string vertSource = FileHandler::loadShaderCode(shader + ".vert");
        std::string fragSource = FileHandler::loadShaderCode(shader + ".frag");

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* vertSourceC = vertSource.c_str();
        const char* fragSourceC = fragSource.c_str();

        glShaderSource(vertexShader, 1, &vertSourceC, NULL);
        glShaderSource(fragmentShader, 1, &fragSourceC, NULL);

        int success;
        char info[512];

        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, info);
            std::cout << "info" << std::endl;
            throw std::runtime_error("failed to compile vertex shader: " + shader);
        }

        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, info);
            std::cout << "info" << std::endl;
            throw std::runtime_error("failed to compile fragment shader: " + shader);
        }

        unsigned int program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, info);
            std::cout << "info" << std::endl;
            throw std::runtime_error("failed to link shader program: " + shader);
        }

        m_Shaders[shader] = program;

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
}

}
