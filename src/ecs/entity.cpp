#include "../../include/ecs/entity.hpp"

namespace dojo {

    Entity::Entity() {
        m_ID = genNewID();
        m_componentBits = 0;
        m_components = std::map<uint32_t, Component*>();
    }

    Entity::~Entity() {
        for (auto it = m_components.begin(); it != m_components.end(); it++) {
            delete it->second;
        }
    }

    uint32_t Entity::genNewID() {
        static uint32_t newID = 0;
        return ++newID;
    }

    bool Entity::hasComponents(uint32_t _componentBits) {
        if ((m_componentBits & _componentBits) > 0) {
            return true;
        }
        return false;
    }

    void Entity::setHasComponent(uint32_t _componentBits, bool _value) {
        if (_value == true) {
            if (!hasComponents(_componentBits)) {
                m_componentBits += _componentBits;
            }
        } else {
            if (hasComponents(_componentBits)) {
                m_componentBits -= _componentBits;
            }
        }
    }

    void Entity::addComponent(Component* _component, Component::ComponentBits _type) {
        setHasComponent(_type, true);
        m_components[_type] = _component;
    }

    void Entity::removeComponent(Component::ComponentBits _componentType) {
        if (!hasComponents(_componentType)) return;

        delete m_components[_componentType];
        m_components.erase(_componentType);
        setHasComponent(_componentType, false);
    }

    Component* Entity::getComponent(Component::ComponentBits _component) {
        if (m_components.find(_component) == m_components.end()) {
            return nullptr;
        }
        return m_components[_component];
    }

}
