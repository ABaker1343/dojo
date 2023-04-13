#define STB_IMAGE_IMPLEMENTATION

#include "headers/fileHandler.hpp"
#include "headers/stb_image.h"

namespace dojo {


    std::string FileHandler::loadShaderCode(const std::string& _filepath) {

        std::ifstream f(_filepath, std::ios::binary | std::ios::ate);
        std::streamsize filesize = f.tellg();

        f.seekg(0, std::ios::beg);
        std::vector<char>* buffer = new std::vector<char>(filesize + 1);

        if (f.read(buffer->data(), filesize)) {
            f.close();
            (*buffer)[buffer->size()] = '\0';
            return std::string(buffer->data());
        }
        
        f.close();
        throw std::runtime_error("failed to read shader: " + _filepath);
    }

}
