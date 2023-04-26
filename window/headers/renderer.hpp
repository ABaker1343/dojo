#ifndef __HEADER_RENDERER
#define __HEADER_RENDERER

#include <iostream>
#include <string>
#include <exception>
#include <map>

#include "../../headers/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "window.hpp"
#include "../../gameObjects/headers/camera.hpp"
#include "../../gameObjects/headers/renderable.hpp"
#include "../../headers/fileHandler.hpp"
#include "../../gameObjects/headers/objects2D/gameObject2DAnimated.hpp"
#include "../../gameObjects/headers/objects2D/gameObject2DStatic.hpp"
#include "../../gameObjects/headers/menuItems/menuItem.hpp"

namespace dojo{

typedef struct {
    unsigned int textureHandle;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
} FontCharacter;

class Renderer {
    public:
        Renderer(Window* _window, glm::vec2 _VPPos = glm::vec2(0,0), glm::vec2 _VPSize = glm::vec2(1,1));
        ~Renderer();

        void draw(Camera* _camera, Renderable* _object);
        void draw(Camera* _camera, GameObject2DStatic* _object);
        void draw(Camera* _camera, GameObject2DAnimated* _object);
        void draw(MenuItem* _item);

        void drawToTexture(Texture* _texture, const std::string& _text, float _x, float _y, float _scale);

        void clear();
        void setShader(const std::string& _shader);
        void setShaderPath(const std::string& _shaderPath);
        void loadShaders();
        void resize();

        Window* getWindow();


    private:

        Window* m_Window;
        std::string m_ShaderPath;
        unsigned int m_CurrentShader;
        glm::vec2 m_VPAbsPos;
        glm::vec3 m_VPAbsScale;
        glm::vec2 m_VPScale;
        glm::vec2 m_VPPos;

        unsigned int m_textureFramebuffer;
        unsigned int m_textureRenderbuffer;
        unsigned int m_textureDepthStencilTexture;
        unsigned int m_textVertexArray;
        unsigned int m_textVertexBuffer;

        std::map<std::string, unsigned int> m_Shaders;
        std::map<char, FontCharacter> m_fontMapTerm;

        void drawMenuText(const std::string& _text, float _x, float _y, float _scale);

        void setUniformMat4(const char* _name, glm::mat4 _value);
        void setUniformVec4(const char* _name, glm::vec4 _value);
        void setUniformVec3(const char* _name, glm::vec3 _value);
        void setUniformIVec2(const char* _name, glm::vec2 _value);
        void setUniformInt(const char* _name, int _value);

        void makeTextureRenderDependancies();
        void initFreetype();

        

};

}

#endif
