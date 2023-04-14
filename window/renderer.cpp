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

void Renderer::draw(Camera* _camera, Renderable* _object) {
    // bind texture
    
    glBindTexture(GL_TEXTURE_2D, _object->getTexture()->getHandle());
    
    //bind uniforms
    unsigned int worldTransformLoc = glGetUniformLocation(m_CurrentShader, "in_worldTransform");
    glUniformMatrix4fv(worldTransformLoc, 1, GL_FALSE, glm::value_ptr(_object->getWorldTransform()));

    unsigned int cameraTransformLoc = glGetUniformLocation(m_CurrentShader, "in_cameraTransform");
    glUniformMatrix4fv(cameraTransformLoc, 1, GL_FALSE, glm::value_ptr(_camera->getCameraTransform()));

    unsigned int projectionTransformLoc = glGetUniformLocation(m_CurrentShader, "in_projectionTransform");
    glUniformMatrix4fv(projectionTransformLoc, 1, GL_FALSE, glm::value_ptr(_camera->getProjectionTransform()));
    
    glBindVertexArray(_object->getVertexArray());

    glDrawArrays(GL_TRIANGLES, 0, _object->getVertexBufferSize() / 3);
}

void Renderer::setShader(const std::string& _shader) {
    m_CurrentShader = m_Shaders[_shader];
    glUseProgram(m_CurrentShader);
}

void Renderer::setShaderPath(const std::string& _shaderPath) {
    m_ShaderPath = _shaderPath;
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
