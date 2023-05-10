#include "../../include/ecs/staticMeshComponent.hpp"

namespace dojo {

StaticMeshComponent::StaticMeshComponent(const std::string& _meshPath) {
    if (_meshPath.ends_with(".obj")) {
        m_meshes = FileHandler::loadMeshesOBJ(_meshPath);
    }
}

StaticMeshComponent::~StaticMeshComponent() {

}

}
