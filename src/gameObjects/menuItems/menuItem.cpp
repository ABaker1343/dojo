#include "../../../include/gameObjects/menuItems/menuItem.hpp"


namespace dojo {

unsigned int MenuItem::m_staticVertexArray = 0;
unsigned int MenuItem::m_staticVertexBuffer = 0;

MenuItem::MenuItem(glm::vec2 _pos, glm::vec2 _scale, const std::string& _displayText) {
    m_texture = new Texture(glm::ivec2(500, 500));
    m_pos = _pos;
    m_scale = _scale;
};

MenuItem::MenuItem(glm::vec2 _pos, glm::vec2 _scale, Texture* _texture) {
    m_texture = _texture;
    createBuffers();
    m_pos = _pos;
    m_scale = _scale;
}

MenuItem::~MenuItem() {
    delete m_texture;
}

bool MenuItem::isMouseOver(glm::vec2 _windowSize, glm::vec2 _mousePos) {
    // mouse pos is given to the function with top left being 0,0
    // function expects bottom left so we flip the mouse position
    _mousePos = _windowSize - _mousePos;
    glm::vec2 relativePos = _mousePos / _windowSize;
    std::cout << "relativePos: " << relativePos.x << " " << relativePos.y << std::endl;
    if (relativePos.x > m_pos.x && relativePos.x < m_pos.x + m_scale.x &&
            relativePos.y > m_pos.y && relativePos.y < m_pos.y + m_scale.y) {
        return true;
    }
    return false;
}

void MenuItem::setPos(glm::vec2 _pos) {
    m_pos = _pos;
}

void MenuItem::setScale(glm::vec2 _scale) {
    m_scale = _scale;
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

glm::vec4 MenuItem::getLocation() {
    return glm::vec4(m_pos.x, m_pos.y, m_scale.x,  m_scale.y);
}

}

