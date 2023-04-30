#include "../../../include/gameObjects/objects2D/gameObject2D.hpp"

namespace dojo {
    
    GameObject2D::GameObject2D() : GameObject() {
        m_VertexData = std::vector<float> {
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
            1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        };

        m_VertexBufferSize = m_VertexData.size();

        createBuffers();

        m_WorldTransform = glm::mat4(1.0f);

        setPos(glm::vec3(0,0,0));
        setScale(glm::vec3(1.f));
        
        m_Flip = glm::ivec2(1,1);
    }

    void GameObject2D::createBuffers() {
        glGenBuffers(1, &m_VertexBuffer);
        glGenVertexArrays(1, &m_VertexArrayObject);
        glBindVertexArray(m_VertexArrayObject);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_VertexData.size(), m_VertexData.data(), GL_STATIC_DRAW);

        // vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

    }

    void GameObject2D::setPos(float _x, float _y, float _z) {
        m_WorldTransform[3][0] = _x;
        m_WorldTransform[3][1] = _y;
        m_WorldTransform[3][2] = _z;
    }
    void GameObject2D::setPos(glm::vec3 _pos) {
        m_WorldTransform[3][0] = _pos.x;
        m_WorldTransform[3][1] = _pos.y;
        m_WorldTransform[3][2] = _pos.z;
    }

    void GameObject2D::setPos(float _x, float _y) {
        m_WorldTransform[3][0] = _x;
        m_WorldTransform[3][1] = _y;
    }
    void GameObject2D::setPos(glm::vec2 _pos) {
        m_WorldTransform[3][0] = _pos.x;
        m_WorldTransform[3][1] = _pos.y;
    }

    void GameObject2D::setScale(float _x, float _y, float _z) {
        m_WorldTransform[0][0] = _x;
        m_WorldTransform[1][1] = _y;
        m_WorldTransform[2][2] = _z;
    }
    void GameObject2D::setScale(glm::vec3 _scale) {
        setScale(_scale.x, _scale.y, _scale.z);
    }

    void GameObject2D::setScale(float _x, float _y) {
        m_WorldTransform[0][0] = _x;
        m_WorldTransform[1][1] = _y;
    }
    void GameObject2D::setScale(glm::vec2 _scale) {
        setScale(_scale.x, _scale.y);
    }

    void GameObject2D::flipx() {
        m_Flip.x = m_Flip.x * -1;
    }
    
    void GameObject2D::flipy() {
        m_Flip.y = m_Flip.y * -1;
    }

    glm::vec3 GameObject2D::getPos() {
        glm::vec3 pos;
        pos.x = m_WorldTransform[3][0];
        pos.y = m_WorldTransform[3][1];
        pos.z = m_WorldTransform[3][2];
        return pos;
    }
    
    glm::vec3 GameObject2D::getScale() {
        glm::vec3 scale;
        scale.x = m_WorldTransform[0][0];
        scale.y = m_WorldTransform[1][1];
        scale.z = m_WorldTransform[2][2];
        return scale;
    }

    glm::ivec2 GameObject2D::getFlip() {
        return m_Flip;
    }

    GameObject2D::~GameObject2D() {

    }

}
