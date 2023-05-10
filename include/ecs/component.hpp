#ifndef __HEADER_COMPONENT
#define __HEADER_COMPONENT

#include <cstdint>


namespace dojo {

class Entity;

class Component {
    public:

        enum ComponentBits {
            StaticSpriteBit = 1,
            TransformBit = 1 << 1,
            StaticMeshBit = 1 << 2,
            LightBit = 1 << 3
        };

    public:
        Component();
        ~Component();
        
        void setOwner(Entity* _owner);

        uint32_t m_id;
        Entity* m_owner;

    private:
        static uint32_t genNewID();
};

}

#include "entity.hpp"

#endif
