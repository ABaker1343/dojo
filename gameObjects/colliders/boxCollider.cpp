#include "../headers/colliders/boxCollider.hpp"

namespace dojo {

BoxCollider::BoxCollider(glm::vec3 _center, glm::vec3 _scale) {
    m_Center = _center;
    m_Scale = _scale;
}

BoxCollider::~BoxCollider() {

}

glm::vec3 BoxCollider::getCenter() {
    return m_Center;
}

glm::vec3 BoxCollider::getScale() {
    return m_Scale;
}

void BoxCollider::setScale(glm::vec3 _scale) {
    m_Scale = _scale;
}

void BoxCollider::setCenter(glm::vec3 _center) {
    m_Center = _center;
}

bool BoxCollider::isColliding(BoxCollider *_other) {
    auto otherPos = _other->getCenter() - _other->getScale();
    auto otherDim = _other->getScale() * glm::vec3(2);

    auto thisPos = m_Center - m_Scale;
    auto thisDim = m_Scale * glm::vec3(2);

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

}
