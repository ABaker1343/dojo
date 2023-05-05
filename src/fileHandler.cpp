#include "../include/fileHandler.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

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

    void FileHandler::loadImage(const std::string& _filepath, unsigned char* data, int* _width, int* _height, int* _numChannels) {
        data = stbi_load(_filepath.c_str(), _width, _height, _numChannels, STBI_rgb_alpha);
    }

    std::vector<Mesh> FileHandler::loadMeshesOBJ(const std::string& _filepath) {

        std::string::size_type lastSlash = _filepath.find_last_of('/');
        std::string dirpath = _filepath.substr(0, lastSlash);
        std::string mtlfilepath;

        std::ifstream f(_filepath, std::ios::binary | std::ios::ate);
        //std::streamsize filesize = f.tellg();

        f.seekg(0, std::ios::beg);
        std::map<std::string, Material> materialMap = std::map<std::string, Material>();
        std::vector<float> tempVerts = std::vector<float>();
        std::vector<float> tempTex = std::vector<float>();
        std::vector<float> tempNorm = std::vector<float>();
        std::vector<Mesh> meshes = std::vector<Mesh>();
        int currentMesh = -1;


        std::string line;
        while(std::getline(f, line)) {
            std::istringstream stream(line);
            std::string identifier;
            stream >> identifier;

            if (identifier == "v") {
                // vertex
                float x, y, z;
                stream >> x >> y >> z;
                tempVerts.push_back(x);
                tempVerts.push_back(y);
                tempVerts.push_back(z);
            } else if (identifier == "vt") {
                // texture coord
                float x, y;
                stream >> x >> y;
                tempTex.push_back(x);
                tempTex.push_back(y);
            } else if (identifier == "vn") {
                // vertex normal
                float x, y, z;
                stream >> x >> y >> z;
                tempNorm.push_back(x);
                tempNorm.push_back(y);
                tempNorm.push_back(z);
            } else if (identifier == "usemtl") {
                // next mesh
                std::string mat;
                stream >> mat;
                meshes.push_back(dojo::Mesh());
                currentMesh++;
                meshes[currentMesh].setMaterial(materialMap[mat]);
            } else if (identifier == "f") {
                int pointnum = 0;
                unsigned int prev1vert, prev1tex, prev1norm;
                unsigned int firstvert, firsttex, firstnorm;

                while(stream.rdbuf()->in_avail()) {
                    std::string point;
                    stream >> point;
                    if (point == "") {
                        continue;
                    }

                    std::replace(point.begin(), point.end(), '/', ' ');
                    std::stringstream pointstream(point);
                    unsigned int vert, tex, norm;
                    pointstream >> vert >> tex >> norm;
                    // obj files are not 0 indexed
                    vert = vert - 1;
                    tex = tex - 1;
                    norm = norm - 1;

                    if (pointnum < 3) {
                        meshes[currentMesh].getVertices()->push_back(tempVerts[vert * 3]);
                        meshes[currentMesh].getVertices()->push_back(tempVerts[vert * 3 + 1]);
                        meshes[currentMesh].getVertices()->push_back(tempVerts[vert * 3 + 2]);
                        meshes[currentMesh].getVertices()->push_back(tempTex[tex * 2]);
                        meshes[currentMesh].getVertices()->push_back(tempTex[tex * 2 + 1]);
                        meshes[currentMesh].getVertices()->push_back(tempNorm[norm * 3]);
                        meshes[currentMesh].getVertices()->push_back(tempNorm[norm * 3 + 1]);
                        meshes[currentMesh].getVertices()->push_back(tempNorm[norm * 3 + 2]);

                        switch (pointnum) {
                            case 0:
                                firstvert = vert;
                                firsttex = tex;
                                firstnorm = norm;
                                break;
                            case 2:
                                prev1vert = vert;
                                prev1tex = tex;
                                prev1norm = norm;
                                break;
                        }

                    } else {
                        // use prev 2 to create a new triangle
                        meshes[currentMesh].getVertices()->push_back(tempVerts[firstvert * 3]);
                        meshes[currentMesh].getVertices()->push_back(tempVerts[firstvert * 3 + 1]);
                        meshes[currentMesh].getVertices()->push_back(tempVerts[firstvert * 3 + 2]);
                        meshes[currentMesh].getVertices()->push_back(tempTex[firsttex * 2]);
                        meshes[currentMesh].getVertices()->push_back(tempTex[firsttex * 2 + 1]);
                        meshes[currentMesh].getVertices()->push_back(tempNorm[firstnorm * 3]);
                        meshes[currentMesh].getVertices()->push_back(tempNorm[firstnorm * 3 + 1]);
                        meshes[currentMesh].getVertices()->push_back(tempNorm[firstnorm * 3 + 2]);

                        meshes[currentMesh].getVertices()->push_back(tempVerts[vert * 3]);
                        meshes[currentMesh].getVertices()->push_back(tempVerts[vert * 3 + 1]);
                        meshes[currentMesh].getVertices()->push_back(tempVerts[vert * 3 + 2]);
                        meshes[currentMesh].getVertices()->push_back(tempTex[tex * 2]);
                        meshes[currentMesh].getVertices()->push_back(tempTex[tex * 2 + 1]);
                        meshes[currentMesh].getVertices()->push_back(tempNorm[norm * 3]);
                        meshes[currentMesh].getVertices()->push_back(tempNorm[norm * 3 + 1]);
                        meshes[currentMesh].getVertices()->push_back(tempNorm[norm * 3 + 2]);

                        meshes[currentMesh].getVertices()->push_back(tempVerts[prev1vert * 3]);
                        meshes[currentMesh].getVertices()->push_back(tempVerts[prev1vert * 3 + 1]);
                        meshes[currentMesh].getVertices()->push_back(tempVerts[prev1vert * 3 + 2]);
                        meshes[currentMesh].getVertices()->push_back(tempTex[prev1tex * 2]);
                        meshes[currentMesh].getVertices()->push_back(tempTex[prev1tex * 2 + 1]);
                        meshes[currentMesh].getVertices()->push_back(tempNorm[prev1norm * 3]);
                        meshes[currentMesh].getVertices()->push_back(tempNorm[prev1norm * 3 + 1]);
                        meshes[currentMesh].getVertices()->push_back(tempNorm[prev1norm * 3 + 2]);

                        prev1vert = vert;
                        prev1tex = tex;
                        prev1norm = norm;

                    }
                    pointnum++;
                }

            } else if (identifier == "mtllib") {
                stream >> mtlfilepath;
                std::cout << "loading materials: " << mtlfilepath << std::endl;
                loadMaterials(dirpath, mtlfilepath, materialMap);
                std::cout << "loaded materials" << std::endl;
            }
        }

        for (unsigned int i = 0; i < meshes.size(); i++){
            meshes[i].createBuffers();
        }
        return meshes;
    }

    void FileHandler::loadMaterials(const std::string& _dirpath, const std::string& _filepath, std::map<std::string, Material>& _materialMap) {
        std::ifstream f(_dirpath + "/" +_filepath);

        Material* currentMat;

        std::string line;
        while(std::getline(f, line)) {
            std::istringstream stream(line);
            std::string identifier;
            stream >> identifier;
            if (identifier == "newmtl") {
                std::string name;
                stream >> name;
                _materialMap[name] = Material();
                currentMat = &(_materialMap[name]);
            } else if (identifier == "Ns") {
                float value;
                stream >> value;
                currentMat->specExponent = value;
            } else if (identifier == "Ka") {
                glm::vec3 value;
                stream >> value.x >> value.y >> value.z;
                currentMat->ka = value;
            } else if (identifier == "Ks") {
                glm::vec3 value;
                stream >> value.x >> value.y >> value.z;
                currentMat->ks = value;
            } else if (identifier == "Kd") {
                glm::vec3 value;
                stream >> value.x >> value.y >> value.z;
                currentMat->kd = value;
            } else if (identifier == "Ke") {
                glm::vec3 value;
                stream >> value.x >> value.y >> value.z;
                currentMat->ke = value;
            } else if (identifier == "Ni") {
                float value;
                stream >> value;
                currentMat->opticalDensity = value;
            } else if (identifier == "d") {
                float value;
                stream >> value;
                currentMat->opacity = value;
            } else if (identifier == "illum") {
                float value;
                stream >> value;
                currentMat->illum = value;
            } else if (identifier == "map_Kd") {
                std::string path;
                stream >> path;
                path = _dirpath + "/" + path;
                currentMat->kd_map = new Texture(path);
                std::cout << path << std::endl;
            }
        }
    }

}
