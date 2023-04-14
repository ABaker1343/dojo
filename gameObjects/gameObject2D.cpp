#include "headers/gameObject2D.hpp"

namespace dojo {
    
    GameObject2D::GameObject2D(const std::string& _texturePath) : GameObject() {
        m_Texture = nullptr;
        setTexture(_texturePath);

        m_VertexData = new std::vector<float> {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.0f,  0.0f, 1.0f,
            0.5f, 0.5f, 0.0f,   1.0f, 1.0f,
            0.5f, 0.5f, 0.0f,   1.0f, 1.0f,
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        };

        m_VertexBufferSize = m_VertexData->size();

        createBuffers();

        m_WorldTransform = glm::mat4(1.0f);
        setPos(glm::vec3(0,0,0));
        setScale(glm::vec3(1.f));
    }

    void GameObject2D::setTexture(const std::string& _texturePath) {
        // set the texture of the object for rendering
        if (!(m_Texture == nullptr)) {
            delete m_Texture;
        }
        m_Texture = new Texture(_texturePath);
    }

    void GameObject2D::createBuffers() {
        glGenBuffers(1, &m_VertexBuffer);
        glGenVertexArrays(1, &m_VertexArrayObject);
        glBindVertexArray(m_VertexArrayObject);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_VertexData->size(), m_VertexData->data(), GL_STATIC_DRAW);

        // vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

    }

    void GameObject2D::setPos(glm::vec3 _pos) {
        m_WorldPos = _pos;
        m_WorldTransform[3][0] = m_WorldPos.x;
        m_WorldTransform[3][1] = m_WorldPos.y;
        m_WorldTransform[3][2] = m_WorldPos.z;
    }

    void GameObject2D::setScale(glm::vec3 _scale) {
        m_Scale = _scale;
        m_WorldTransform[0][0] = m_Scale.x;
        m_WorldTransform[1][1] = m_Scale.y;
        m_WorldTransform[2][2] = m_Scale.z;
    }

    glm::vec3 GameObject2D::getPos() {
        return m_WorldPos;
    }
    
    glm::vec3 GameObject2D::getScale() {
        return m_Scale;
    }

    GameObject2D::~GameObject2D() {
        delete m_Texture;
    }

}
