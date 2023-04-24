#ifndef __HEADER_TEXTURE
#define __HEADER_TEXTURE

#include <string>
#include <glad/glad.h>
#include <stdexcept>
#include <glm/glm.hpp>

#include "../../headers/stb_image.h"

namespace dojo {

class Texture {
    public:
        Texture(const std::string& _filepath);
        Texture(glm::ivec2 _size);
        ~Texture();

        unsigned int getHandle();
        glm::ivec2 getSize();

    protected:
        unsigned int m_Handle;
        int m_Width, m_Height, m_NumChannels;

};

}

#endif
