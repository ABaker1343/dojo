#include "walker.hpp"

Walker::Walker() {
    m_object = new dojo::GameObject2DAnimated("Knight/Idle.png", 1, 6, "idle");
    m_object->addAnimation("walking", "Knight/Walk.png", 1, 8);
    m_object->addAnimation("die", "Knight/Dead.png", 1, 4);
    m_object->setPos(0, 0, 0);

    m_collider = new dojo::BoxCollider(m_object->getPos(), m_object->getScale());
}

Walker::~Walker() {
    delete m_object;
    delete m_collider;
}

void Walker::move(glm::vec2 _movement) {
    m_object->setPos(m_object->getPos() + glm::vec3(_movement, 0.0));
}
