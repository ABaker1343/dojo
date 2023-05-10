#include "../../include/ecs/component.hpp"

namespace dojo {

    Component::Component() {
        m_id = genNewID();
    }

    Component::~Component() {

    }

    uint32_t Component::genNewID() {
        static uint32_t newID = 0;
        return ++newID;
    }

    void Component::setOwner(Entity* _owner) {
        m_owner = _owner;
    }

}
