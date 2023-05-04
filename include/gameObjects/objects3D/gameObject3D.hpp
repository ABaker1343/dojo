#ifndef __HEADER_GAME_OBJECT_3D
#define __HEADER_GAME_OBJECT_3D

#include <string>

#include "../gameObject.hpp"
#include "../renderable.hpp"
#include "../../fileHandler.hpp"
#include "mesh.hpp"

namespace dojo {
    
    class GameObject3D : public GameObject, public Renderable {
    public:
        GameObject3D(const std::string& _filepath);
        ~GameObject3D();
        std::unique_ptr<GameObject> clone() override;

        void setPos(glm::vec3 _pos) override;
        void setPos(float _x, float _y, float _z) override;
        void setScale(glm::vec3 _pos) override;
        void setScale(float _x, float _y, float _z) override;

        glm::vec3 getPos() override;
        glm::vec3 getScale() override;

        std::vector<Mesh>* getMeshes();


        protected:
            std::vector<Mesh> m_meshes;

            void createBuffers() override;

    };
}

#endif
