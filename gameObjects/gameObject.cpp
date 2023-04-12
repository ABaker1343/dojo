#include "headers/gameObject.hpp"

namespace dojo {

    GameObject::GameObject() {
        setScale(glm::vec3(0, 0, 0));
        setPos(glm::vec3(0, 0, 0));
    }

    GameObject::GameObject(glm::vec3 _pos, glm::vec3 _scale) {
        setPos(_pos);
        setScale(_scale);
    }

    void GameObject::setPos(glm::vec3 _pos) {
        m_Scale = _pos;
    }

    void GameObject::setScale(glm::vec3 _scale) {
        m_Scale = _scale;
    }

    GameObject::~GameObject() {
        
    }


}
