#ifndef __HEADER_ENTITY
#define __HEADER_ENTITY

#include <cstdint>
#include <map>

#include "component.hpp"

namespace dojo {

class Entity {
    public:
        Entity();
        ~Entity();

        bool hasComponents(uint32_t _componentBits);
        void setHasComponent(uint32_t _componentBit, bool _value);
        void addComponent(Component* _component, Component::ComponentBits _type);
        void removeComponent(Component::ComponentBits _componentType);
        Component* getComponent(Component::ComponentBits _componentBit);

    private:
        uint32_t m_ID;
        uint32_t m_componentBits;
        std::map<uint32_t, Component*> m_components;
        
        static uint32_t genNewID();
};

}

#endif
