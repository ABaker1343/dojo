#ifndef __HEADER_FILE_HANDLER
#define __HEADER_FILE_HANDLER

#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>

#include "gameObjects/objects3D/mesh.hpp"

/*namespace dojo {
    class Mesh;
    struct Material;
}*/

namespace dojo {

class FileHandler {
    
    public:
        std::string static loadShaderCode(const std::string& _filepath);
        std::vector<Mesh> static loadMeshesOBJ(const std::string& _filepath);
        static void loadMaterials(const std::string& _dirpath, const std::string& _filepath, std::map<std::string, Material>& _materialMap);
        static void loadImage(const std::string& _filepath, unsigned char* data, int* width, int* height, int* numChannels);
};


}




#endif
