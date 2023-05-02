#include "../../../include/gameObjects/objects3D/mesh.hpp"

namespace dojo {

    Mesh::Mesh() {
        m_vertices = std::vector<float>();
    }
    
    Mesh::~Mesh() {

    }

    std::vector<float>* Mesh::getVertices() {
        return &m_vertices;
    }

    void Mesh::setMaterial(Material _material) {
        m_material = _material;
    }
    Material* Mesh::getMaterial() {
        return &m_material;
    }

    void Mesh::createBuffers() {
        glGenVertexArrays(1, &m_vertexArrayObject);
        glGenBuffers(1, &m_vertexBuffer);

        glBindVertexArray(m_vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
        glEnableVertexAttribArray(0);
        // texture
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*) (sizeof(float) * 3));
        glEnableVertexAttribArray(1);
        //normal
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*) (sizeof(float) * 5));
        glEnableVertexAttribArray(2);


        m_vertexBufferSize = m_vertices.size();
    }

}
