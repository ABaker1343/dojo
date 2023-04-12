#include "headers/renderable.hpp"

namespace dojo {

Renderable::Renderable() {
    // create nice
}

Renderable::~Renderable() {

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

}
