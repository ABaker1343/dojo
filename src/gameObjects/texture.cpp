#include "../../include/gameObjects/texture.hpp"

#include "../../include/stb_image.h"

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
        
        unsigned char* data;

        if (_filepath.ends_with(".png")) {
            data = stbi_load(_filepath.c_str(), &m_Width, &m_Height, &m_NumChannels, STBI_rgb_alpha);
            if (!data) {
                stbi_image_free(data);
                throw std::runtime_error("failed to load: " + _filepath);
            }
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }

        else if (_filepath.ends_with(".jpg") || _filepath.ends_with("jpeg")) {
            data = stbi_load(_filepath.c_str(), &m_Width, &m_Height, &m_NumChannels, STBI_rgb);
            if (!data) {
                stbi_image_free(data);
                throw std::runtime_error("failed to load: " + _filepath);
            }
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }

        else {
            throw std::runtime_error("invalid file format for texture: " + _filepath);
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        
    }

    Texture::Texture(glm::ivec2 _size) {
        m_Width = _size.x;
        m_Height = _size.y;

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

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glGenerateMipmap(GL_TEXTURE_2D);

    }

    Texture::Texture(glm::ivec2 _size, Use _use) {
        m_Width = _size.x;
        m_Height = _size.y;

        // set the wrap parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        
        float borderColor[] = {1.f, 1.f, 1.f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        // filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // generate texture
        glGenTextures(1, &m_Handle);
        glBindTexture(GL_TEXTURE_2D, m_Handle);
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
        glGenerateMipmap(GL_TEXTURE_2D);

    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_Handle);
    }

    unsigned int Texture::getHandle() {
        return m_Handle;
    }

    glm::ivec2 Texture::getSize() {
        return glm::vec2(m_Width, m_Height);
    }

}
