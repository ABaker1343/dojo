#ifndef __HEADER_RENDERABLE
#define __HEADER_RENDERABLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "texture.hpp"

namespace dojo {

class Renderable {
    public:
        Renderable();
        virtual ~Renderable() {};
        
        unsigned int getVertexBuffer();
        unsigned int getVertexBufferSize();
        unsigned int getVertexArray();
        glm::mat4 getWorldTransform();
        
    protected:
        unsigned int m_VertexBuffer;
        unsigned int m_VertexArrayObject;
        unsigned int m_VertexBufferSize;

        glm::mat4 m_WorldTransform;

        virtual void createBuffers();
};

}

#endif
