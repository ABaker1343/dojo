#include <glm/glm.hpp>
#include <glad/glad.h>

namespace dojo {

class Renderable {
    public:
        Renderable();
        ~Renderable();
        
        unsigned int getVertexBuffer();
        unsigned int getVertexBufferSize();
        unsigned int getVertexArray();
        
    protected:
        unsigned int m_ShaderProgram;
        unsigned int m_VertexBuffer;
        unsigned int m_VertexArrayObject;
        unsigned int m_VertexBufferSize;

        virtual void createBuffers();
};

}
