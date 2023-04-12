#include <glm/glm.hpp>

namespace dojo {

class Renderable {
    public:
        Renderable();
        ~Renderable();
        
        unsigned int getVertexBuffer();
        unsigned int getVertexBufferSize();
        
    private:
        unsigned int m_ShaderProgram;
        unsigned int m_VertexBuffer;
        unsigned int m_VertexArrayObject;
};

}
