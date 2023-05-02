#include "../../include/gameObjects/renderable.hpp"

namespace dojo {

Renderable::Renderable() {
    // empty constructor
}

void Renderable::createBuffers() {

}

unsigned int Renderable::getVertexBuffer() {
    return m_vertexBuffer;
}

unsigned int Renderable::getVertexBufferSize() {
    return m_vertexBufferSize;
}

unsigned int Renderable::getVertexArray() {
    return m_vertexArrayObject;
}

glm::mat4 Renderable::getWorldTransform() {
    return m_worldTransform;
}

}
