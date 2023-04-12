#include "headers/renderer.hpp"
#include <GLFW/glfw3.h>

namespace dojo {

Renderer::Renderer(Window& _window, int _VPWidth, int _VPHeight) {

    // glfw window hints

    glViewport(0, 0, _VPWidth, _VPHeight);
    

}

Renderer::~Renderer() {

}

void Renderer::draw(Renderable& _object) {
    glBindBuffer(GL_ARRAY_BUFFER, _object.getVertexBuffer());
    glDrawArrays(GL_TRIANGLES, 0, _object.getVertexBufferSize());
}

void Renderer::setShader(std::string& _shader) {
    unsigned int newShader = m_Shaders[_shader];
    glUseProgram(newShader);
}

}
