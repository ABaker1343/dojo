#include "../../../include/gameObjects/objects3D/gameObject3D.hpp"

namespace dojo {

    GameObject3D::GameObject3D(const std::string& _filepath) {
        
        if (_filepath.ends_with("obj")) {
            m_meshes = FileHandler::loadMeshesOBJ(_filepath);
        }
        
        
        m_worldTransform = glm::identity<glm::mat4>();
        setPos(glm::vec3(0));
        setScale(glm::vec3(1));

    }

    GameObject3D::~GameObject3D() {

    }

    std::unique_ptr<GameObject> GameObject3D::clone() {
        return std::make_unique<GameObject3D>(*this);
    }

    void GameObject3D::setPos(glm::vec3 _pos) {
        m_worldTransform[3][0] = _pos.x;
        m_worldTransform[3][1] = _pos.y;
        m_worldTransform[3][2] = _pos.z;
    }
    void GameObject3D::setPos(float _x, float _y, float _z) {
        m_worldTransform[3][0] = _x;
        m_worldTransform[3][1] = _y;
        m_worldTransform[3][2] = _z;
    }
    glm::vec3 GameObject3D::getPos() {
        return glm::vec3(m_worldTransform[3][0], m_worldTransform[3][1], m_worldTransform[3][2]);
    }

    void GameObject3D::setScale(glm::vec3 _scale) {
        m_worldTransform[0][0] = _scale.x;
        m_worldTransform[1][1] = _scale.y;
        m_worldTransform[2][2] = _scale.z;
    }
    void GameObject3D::setScale(float _x, float _y, float _z) {
        m_worldTransform[0][0] = _x;
        m_worldTransform[1][1] = _y;
        m_worldTransform[2][2] = _z;
    }
    glm::vec3 GameObject3D::getScale() {
        return glm::vec3(m_worldTransform[0][0], m_worldTransform[1][1], m_worldTransform[2][2]);
    }

    std::vector<Mesh>* GameObject3D::getMeshes() {
        return &m_meshes;
    }

    void GameObject3D::createBuffers() {

    }

}
