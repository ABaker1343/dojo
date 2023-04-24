#include "../headers/menuItems/menuItem.hpp"


namespace dojo {

unsigned int MenuItem::m_staticVertexArray = 0;
unsigned int MenuItem::m_staticVertexBuffer = 0;

MenuItem::MenuItem(glm::vec2 _pos, glm::vec2 _scale, const std::string& _displayText) {
    m_texture = new Texture(glm::ivec2(500, 500));
};

MenuItem::MenuItem(glm::vec2 _pos, glm::vec2 _scale, Texture* _texture) {
    createBuffers();
    m_texture = _texture;
}

MenuItem::~MenuItem() {
    delete m_texture;
}

void MenuItem::createBuffers() {
    if (m_staticVertexBuffer != 0) {
        return;
    }

    //create 2 simple 2d buffer
    std::vector<float> data = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
    };

    glGenVertexArrays(1, &m_staticVertexArray);
    glGenBuffers(1, &m_staticVertexBuffer);

    glBindVertexArray(m_staticVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, m_staticVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
    glEnableVertexAttribArray(0);
}

Texture* MenuItem::getTexture() {
    return m_texture;
}

unsigned int MenuItem::getVertexArray() {
    return m_staticVertexArray;
}

}

