#ifndef __HEADER_STATIC_MESH_COMPONENT
#define __HEADER_STATIC_MESH_COMPONENT

#include <string>
#include "component.hpp"
#include "../gameObjects/objects3D/mesh.hpp"
#include "../fileHandler.hpp"

namespace dojo {

class StaticMeshComponent : public Component {
    public:
        StaticMeshComponent(const std::string& _meshPath);
        ~StaticMeshComponent();
        std::vector<Mesh> m_meshes;

    private:
};

}

#endif
