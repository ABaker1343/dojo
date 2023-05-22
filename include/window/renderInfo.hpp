#ifndef __HEADER_RENDER_INFO
#define __HEADER_RENDER_INFO

#include "../glad/glad.h"
#include <vector>

namespace dojo {
    
struct RenderInfo {
    GLuint vertexArray;
    GLuint vertexBuffer;
    unsigned int numVertices;

    static RenderInfo genNewRenderInfo(std::vector<float> _vertices);
};

}

#endif
