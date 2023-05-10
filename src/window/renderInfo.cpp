#include "../../include/window/renderInfo.hpp"

namespace dojo {

    RenderInfo RenderInfo::genNewRenderInfo(std::vector<float> _vertices) {
        RenderInfo newInfo;
        glGenVertexArrays(1, &newInfo.vertexArray);
        glGenBuffers(1, &newInfo.vertexBuffer);

        glBindVertexArray(newInfo.vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, newInfo.vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 5));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        newInfo.numVertices = _vertices.size() / 8;

        return newInfo;
    }

}
