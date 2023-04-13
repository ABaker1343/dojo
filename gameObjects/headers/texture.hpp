#ifndef __HEADER_TEXTURE
#define __HEADER_TEXTURE

#include <string>
#include <glad/glad.h>
#include <stdexcept>

#include "../../headers/stb_image.h"

namespace dojo {

class Texture {
    public:
        Texture(const std::string& _filepath);
        ~Texture();

        unsigned int getHandle();

    protected:
        unsigned int m_Handle;
        int m_Width, m_Height, m_NumChannels;

};

}

#endif
