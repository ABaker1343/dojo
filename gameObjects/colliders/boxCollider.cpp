#include "../headers/colliders/boxCollider.hpp"

namespace dojo {

BoxCollider::BoxCollider(glm::vec3 _center, glm::vec3 _scale) {
    m_center = _center;
    m_scale = _scale;
}

BoxCollider::~BoxCollider() {

}

glm::vec3 BoxCollider::getCenter() {
    return m_center;
}

glm::vec3 BoxCollider::getScale() {
    return m_scale;
}

void BoxCollider::setScale(glm::vec3 _scale) {
    m_scale = _scale;
}

void BoxCollider::setCenter(glm::vec3 _center) {
    m_center = _center;
}

bool BoxCollider::isColliding(BoxCollider *_other) {
    auto otherPos = _other->getCenter() - _other->getScale();
    auto otherDim = _other->getScale() * glm::vec3(2);

    auto thisPos = m_center - m_scale;
    auto thisDim = m_scale * glm::vec3(2);

    if (
            thisPos.x < otherPos.x + otherDim.x &&
            thisPos.x + thisDim.x > otherPos.x &&

            thisPos.y < otherPos.y + otherDim.y &&
            thisPos.y + thisDim.y > otherPos.y &&

            thisPos.z < otherPos.z + otherDim.z &&
            thisPos.z + thisDim.z > otherPos.z
       ) {
        return true;
    }

    return false;
}

void BoxCollider::snapToSide(GameObject* _object, Side _side) {
    glm::vec3 newPos = _object->getPos();
    glm::vec3 objectScale = _object->getScale();

    switch(_side) {
        case LEFT:
            newPos.x = m_center.x - m_scale.x - objectScale.x;
            break;
        case RIGHT:
            newPos.x = m_center.x + m_scale.x + objectScale.x;
            break;
        case TOP:
            newPos.y = m_center.y + m_center.x + objectScale.y;
            break;
        case BOTTOM:
            newPos.y = m_center.y - m_center.x - objectScale.y;
            break;
    }
    _object->setPos(newPos);
}

}
