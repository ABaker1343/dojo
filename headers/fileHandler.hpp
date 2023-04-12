#ifndef __HEADER_FILE_HANDLER
#define __HEADER_FILE_HANDLER

#include <fstream>
#include <vector>
#include <iostream>

namespace dojo {

class FileHandler {
    
    public:
        std::string static loadShaderCode(const std::string& _filepath);
};

}

#endif
