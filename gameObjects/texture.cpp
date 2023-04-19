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

        // generate texture
        glGenTextures(1, &m_Handle);
        glBindTexture(GL_TEXTURE_2D, m_Handle);
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        unsigned char *data;

        if (_filepath.ends_with(".png")) {
            data = stbi_load(_filepath.c_str(), &m_Width, &m_Height, &m_NumChannels, STBI_rgb_alpha);
            if (!data) {
                stbi_image_free(data);
                throw std::runtime_error("failed to load: " + _filepath);
            }
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }

        else if (_filepath.ends_with(".jpg")) {
            data = stbi_load(_filepath.c_str(), &m_Width, &m_Height, &m_NumChannels, STBI_rgb);
            if (!data) {
                stbi_image_free(data);
                throw std::runtime_error("failed to load: " + _filepath);
            }
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }

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
