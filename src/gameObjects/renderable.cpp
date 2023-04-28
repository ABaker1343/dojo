#include "../../include/gameObjects/renderable.hpp"

namespace dojo {

Renderable::Renderable() {
    // empty constructor
}

void Renderable::createBuffers() {

}

unsigned int Renderable::getVertexBuffer() {
    return m_VertexBuffer;
}

unsigned int Renderable::getVertexBufferSize() {
    return m_VertexBufferSize;
}

unsigned int Renderable::getVertexArray() {
    return m_VertexArrayObject;
}

glm::mat4 Renderable::getWorldTransform() {
    return m_WorldTransform;
}

}
