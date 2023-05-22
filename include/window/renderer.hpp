#ifndef __HEADER_RENDERER
#define __HEADER_RENDERER

#include <iostream>
#include <string>
#include <exception>
#include <map>

#include "../glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "window.hpp"
#include "../gameObjects/camera.hpp"
#include "../gameObjects/renderable.hpp"
#include "../fileHandler.hpp"
#include "../gameObjects/objects2D/gameObject2DAnimated.hpp"
#include "../gameObjects/objects2D/gameObject2DStatic.hpp"
#include "../gameObjects/menuItems/menuItem.hpp"
#include "../gameObjects/objects3D/gameObject3D.hpp"
#include "../gameObjects/lights/light.hpp"

#include "../ecs/staticSpriteComponent.hpp"
#include "../ecs/animatedSpriteComponent.hpp"
#include "../ecs/staticMeshComponent.hpp"
#include "../ecs/transformComponent.hpp"
#include "../ecs/directionalLightComponent.hpp"
#include "../ecs/viewerTransformComponent.hpp"

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

        void draw(Camera* _camera, StaticSpriteComponent* _sprite, TransformComponent* _transform);
        void draw(Camera* _camera, AnimatedSpriteComponent* _sprite, TransformComponent* _transform);
        void draw(Camera* _camera, StaticMeshComponent* _mesh, TransformComponent* _transform);

        void draw(DirectionalLightComponent* _light, ViewerTransformComponent* _lightView, StaticMeshComponent* _mesh, TransformComponent* _meshTransform);

        void draw(Camera* _camera, DirectionalLightComponent* _light, ViewerTransformComponent* _lightView, StaticMeshComponent* _mesh, TransformComponent* _meshTransform);

        void draw(Camera* _camera, Renderable* _object);
        void draw(Camera* _camera, GameObject* _object);
        void draw(Camera* _camera, GameObject2DStatic* _object);
        void draw(Camera* _camera, GameObject2DAnimated* _object);
        void draw(Camera* _camera, GameObject3D* _object);
        void draw(MenuItem* _item);
        void draw(Camera* _camera, const std::string& _text);

        void drawLit(Camera* _camera, GameObject3D* _object, Light* light);

        void drawShadow(Light* _light, GameObject3D* _object);
        void drawShadow(Light* _light, GameObject2D* _object);

        void textToTexture(Texture* _texture, const std::string& _text, glm::vec4 _textColor = glm::vec4(0, 0, 0, 1), glm::vec3 _backgroundColor = glm::vec3(0.5));
        void drawText(Camera* _camera, const std::string& _text, glm::vec3 _pos, float _scale, glm::vec4 _color);

        void clear();
        void clearShadow(Light* _light);
        void clear(DirectionalLightComponent* _light);
        void setShader(const std::string& _shader);
        void setShaderPath(const std::string& _shaderPath);
        void loadShaders();
        void resize();

        Window* getWindow();


    private:

        Window* m_Window;
        std::string m_ShaderPath;
        unsigned int m_CurrentShader;
        bool m_useDefaultShaders;

        glm::vec2 m_VPAbsPos;
        glm::vec3 m_VPAbsScale;
        glm::vec2 m_VPScale;
        glm::vec2 m_VPPos;
        glm::vec4 m_clearColor;

        unsigned int m_textureFramebuffer;
        unsigned int m_textureRenderbuffer;
        unsigned int m_textureDepthStencilTexture;
        unsigned int m_textVertexArray;
        unsigned int m_textVertexBuffer;

        unsigned int m_shadowFramebuffer;

        std::map<std::string, unsigned int> m_Shaders;
        std::map<char, FontCharacter> m_fontMapTerm;

        void drawMenuText(const std::string& _text, float _x, float _y, float _scale, glm::vec4 _textColor);

        void setUniformMat4(const char* _name, glm::mat4 _value);
        void setUniformVec4(const char* _name, glm::vec4 _value);
        void setUniformVec3(const char* _name, glm::vec3 _value);
        void setUniformIVec2(const char* _name, glm::vec2 _value);
        void setUniformInt(const char* _name, int _value);
        void setUniformFloat(const char* _name, float _value);

        void makeTextureRenderDependancies();
        void makeShadowDependencies();
        void initFreetype();

};

}

#endif
