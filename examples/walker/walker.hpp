#include "../../include/dojo.hpp"

class Walker {
    public:
        enum STATE {
            IDLE, WALKING, DIE,
        };

    public:
        Walker();
        ~Walker();

        void move(glm::vec2 _movement);

        dojo::GameObject2DAnimated* m_object;
        dojo::BoxCollider* m_collider;
        STATE m_state;
        STATE m_prevState;
};

