#include "headers/renderer.hpp"
#include <GLFW/glfw3.h>

namespace dojo {

Renderer::Renderer(Window* _window, glm::vec2 _VPSize) {

    m_Window = _window;

    // glfw window hints

    glViewport(0, 0, _VPSize.x, _VPSize.y);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    setShaderPath("shaders/");
    
}

Renderer::Renderer(Window* _window) {
    glm::vec2 windowSize = _window->getDimensions();
    Renderer(_window, windowSize);
}

Renderer::~Renderer() {
    // remove all the programs
    std::map<std::string, unsigned int>::iterator it;
    for (it = m_Shaders.begin(); it != m_Shaders.end(); it++){
        glDeleteProgram(it->second);
    }
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(Renderable& _object) {
    glBindVertexArray(_object.getVertexArray());
    std::cout << _object.getVertexArray() << std::endl;
    glDrawArrays(GL_TRIANGLES, 0, _object.getVertexBufferSize() / 3);
}

void Renderer::setShader(const std::string& _shader) {
    unsigned int newShader = m_Shaders[_shader];
    glUseProgram(newShader);
}

void Renderer::setShaderPath(const std::string& _shaderPath) {
    m_ShaderPath = _shaderPath;
    std::cout << "eeee" << _shaderPath << " " << m_ShaderPath << std::endl;
}

void Renderer::loadShaders() {
    // load the shaders to use
    std::vector<std::string> shaderNames = {"default"};

    for (const std::string& shader : shaderNames) {
        // load the shader
        std::string vertSource = FileHandler::loadShaderCode("../shaders/" + shader + ".vert");
        std::string fragSource = FileHandler::loadShaderCode(m_ShaderPath + shader + ".frag");

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
            std::cout << info << std::endl;
            throw std::runtime_error("failed to compile vertex shader: " + shader);
        }

        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, info);
            std::cout << info << std::endl;
            throw std::runtime_error("failed to compile fragment shader: " + shader);
        }

        unsigned int program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, info);
            std::cout << info << std::endl;
            throw std::runtime_error("failed to link shader program: " + shader);
        }

        m_Shaders[shader] = program;

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    setShader("default");
}

}
