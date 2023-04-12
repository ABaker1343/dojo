#include <fstream>
#include <vector>

namespace dojo {

class FileHandler {
    
    public:
        std::string static loadShaderCode(const std::string& _filepath);
};

}
