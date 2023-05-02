#ifndef __HEADER_RENDERABLE
#define __HEADER_RENDERABLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glad/glad.h"


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
        unsigned int m_vertexBuffer;
        unsigned int m_vertexArrayObject;
        unsigned int m_vertexBufferSize;

        glm::mat4 m_worldTransform;

        virtual void createBuffers();
};

}

#endif
