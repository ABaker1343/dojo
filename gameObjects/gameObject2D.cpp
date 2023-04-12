#include "headers/gameObject2D.hpp"

namespace dojo {
    
    GameObject2D::GameObject2D(std::string& _texturePath) : GameObject() {
        setTexture(_texturePath);

        vertexData = new std::vector<float> {
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
        };

        m_VertexBufferSize = vertexData->size();
    }

    void GameObject2D::setTexture(std::string& _texturePath) {
        // set the texture of the object for rendering
    }

    void GameObject2D::createBuffers() {
        glGenBuffers(1, &m_VertexBuffer);
        glGenVertexArrays(1, &m_VertexArrayObject);
        glBindVertexArray(m_VertexArrayObject);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData->size(), vertexData->data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

    }

}
