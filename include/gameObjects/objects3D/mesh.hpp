#ifndef __HEADER_MESH
#define __HEADER_MESH

#include <vector>
#include <iostream>
#include "../../../include/glad/glad.h"

#include "../renderable.hpp"
#include "../texture.hpp"

namespace dojo {

    struct Material {
        Texture* kd_map;
        float ns;
        float ni;
        float d;
        float illum;
        glm::vec3 ka;
        glm::vec3 ks;
        glm::vec3 ke;
    };

    class Mesh : public Renderable {
        public:


        public:
            Mesh();
            ~Mesh();

            std::vector<float>* getVertices();
            void setMaterial(Material _material);
            void createBuffers();

            Material* getMaterial();
            

        protected:
            //Texture* m_texture;
            Material m_material;
            std::vector<float> m_vertices;

    };
}


#endif
