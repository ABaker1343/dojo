#include "headers/texture.hpp"

namespace dojo {

    Texture::Texture(const std::string& _filepath) {

        stbi_set_flip_vertically_on_load(true);

        // set the wrap parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);

        // filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        unsigned char *data = stbi_load(_filepath.c_str(), &m_Width, &m_Height, &m_NumChannels, 0);

        if (!data) {
            stbi_image_free(data);
            throw std::runtime_error("failed to load texture: " + _filepath);
        }

        glGenTextures(1, &m_Handle);
        glBindTexture(GL_TEXTURE_2D, m_Handle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_Handle);
    }

    unsigned int Texture::getHandle() {
        return m_Handle;
    }

}
