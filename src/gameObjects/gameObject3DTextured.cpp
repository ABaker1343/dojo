#include "headers/gameObject3DTextured.hpp"

namespace dojo {

GameObject3DTextured::GameObject3DTextured(Shape shape, std::string texturePath,
        glm::vec3 pos, glm::vec3 scale) : GameObject(pos, scale) {

    unsigned int textureOffset;
    unsigned int normalOffset;

    switch (shape) {
        case cube:
            createCubeVertices(textureOffset, normalOffset);
    }

    createTexturedObjectBuffersWithOffsetTextures(vertexArrayObject, vertexBuffer, elementBuffer,
            vertices, textureOffset, normalOffset, elements);
    texture = loadTextureFromFile(texturePath.c_str());

    this->isClone = false;
}

void GameObject3DTextured::createCubeVertices(unsigned int &textureOffset, unsigned int &normalOffset) {

    elements = new std::vector<unsigned int>();

    vertices = new std::vector<float> {
        -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f
    };

    vertexCount = vertices->size() / 3;

    //textures

    std::vector<float> *textures = new std::vector<float> {

        0.0f, 0.0f,
            1.f, 0.f,
            1.f, 1.f,
            1.f, 1.f,
            0.f, 1.f,
            0.f, 0.f,

            0.0f, 0.0f,
            1.f, 0.f,
            1.f, 1.f,
            1.f, 1.f,
            0.f, 1.f,
            0.f, 0.f,
            
            0.0f, 0.0f,
            1.f, 0.f,
            1.f, 1.f,
            1.f, 1.f,
            0.f, 1.f,
            0.f, 0.f,

            0.0f, 0.0f,
            1.f, 0.f,
            1.f, 1.f,
            1.f, 1.f,
            0.f, 1.f,
            0.f, 0.f,

            0.0f, 0.0f,
            1.f, 0.f,
            1.f, 1.f,
            1.f, 1.f,
            0.f, 1.f,
            0.f, 0.f,

            0.0f, 0.0f,
            1.f, 0.f,
            1.f, 1.f,
            1.f, 1.f,
            0.f, 1.f,
            0.f, 0.f,
    };


    // normals
    std::vector<float> *normals = new std::vector<float> {
        0.0f,  0.0f, -1.0f,
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f, -1.0f,

            0.0f,  0.0f,  1.0f,
            0.0f,  0.0f,  1.0f,
            0.0f,  0.0f,  1.0f,
            0.0f,  0.0f,  1.0f,
            0.0f,  0.0f,  1.0f,
            0.0f,  0.0f,  1.0f,

            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,

            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,

            0.0f, -1.0f,  0.0f,
            0.0f, -1.0f,  0.0f,
            0.0f, -1.0f,  0.0f,
            0.0f, -1.0f,  0.0f,
            0.0f, -1.0f,  0.0f,
            0.0f, -1.0f,  0.0f,

            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
    };

    textureOffset = vertices->size();
    vertices->insert(vertices->end(), textures->begin(), textures->end());

    normalOffset = vertices->size();
    vertices->insert(vertices->end(), normals->begin(), normals->end());

}

GameObject3DTextured::GameObject3DTextured(std::string objectPath, std::string texturePath,
        glm::vec3 pos, glm::vec3 scale) : GameObject(pos, scale) {

    std::vector<float> *textureCoords = new std::vector<float>();
    std::vector<float> *normals = new std::vector<float>();
    vertices = new std::vector<float>();
    elements = new std::vector<unsigned int>();

    FileHandler::loadModel(objectPath.c_str(), vertices, textureCoords, normals, elements);

    unsigned int textureOffset = vertices->size();
    vertexCount = vertices->size() / 3;
    vertices->insert(vertices->end(), textureCoords->begin(), textureCoords->end());

    unsigned int normalsOffset = vertices->size();
    vertices->insert(vertices->end(), normals->begin(), normals->end());

    Renderable::createTexturedObjectBuffersWithOffsetTextures(vertexArrayObject, vertexBuffer, elementBuffer,
            vertices, textureOffset, normalsOffset, elements);
    texture = loadTextureFromFile(texturePath.c_str());

    
    delete textureCoords;

    this->isClone = false;

}

GameObject3DTextured::GameObject3DTextured(GameObject3DTextured *obj, glm::vec3 pos, glm::vec3 scale) : GameObject(pos, scale) {
    this->vertexArrayObject = obj->vertexArrayObject;
    this->texture = obj->texture;
    this->elements = obj->elements;
    this->vertices = obj->vertices;
    this->vertexCount = obj->numVertices();

    this->isClone = true;
}

GameObject3DTextured::~GameObject3DTextured() {
    if (!isClone){
        delete vertices;
        delete elements;
        glDeleteVertexArrays(1, &vertexArrayObject);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &elementBuffer);
        glDeleteTextures(1, &texture);
    }
}

}
