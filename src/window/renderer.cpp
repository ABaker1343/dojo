#include "../../include/window/renderer.hpp"

namespace dojo {

Renderer::Renderer(Window* _window, glm::vec2 _VPPos, glm::vec2 _VPSize) {

    m_Window = _window;

    // glfw window hints

    m_VPScale = _VPSize;
    m_VPPos = _VPPos;
    m_clearColor = glm::vec4(0.0, 0.0, 0.0, 1.0); // black
                                                  //
    glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    setShaderPath("shaders/");
    m_Shaders = std::map<std::string, unsigned int>();
    m_useDefaultShaders = true;
    resize();
    _window->bindRenderer(this);

    initFreetype();

    // generate framebuffers
    makeTextureRenderDependancies();
    makeShadowDependencies();
    
}

Renderer::~Renderer() {
    // remove all the programs
    std::map<std::string, unsigned int>::iterator shaderit;
    size_t size = m_Shaders.size();
    unsigned int i = 0;
    for (shaderit = m_Shaders.begin(); shaderit != m_Shaders.end(); shaderit++){
        std::cout << "deleting shader " << ++i << " of " << size << ": " << shaderit->first << std::endl;
        glDeleteProgram(shaderit->second);
        std::cout << "done" << std::endl;
    }
    std::cout << "all shaders have been deleted" << std::endl;
    
    //delete freetype
    for (auto fontit = m_fontMapTerm.begin(); fontit != m_fontMapTerm.end(); fontit++) {
        glDeleteTextures(1, &fontit->second.textureHandle);
    }
    std::cout << "fonts deleted" << std::endl;
    
}

Window* Renderer::getWindow() {
    return m_Window;
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::clearShadow(Light* _light) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFramebuffer);
    glBindTexture(GL_TEXTURE_2D, _light->getShadowMap()->getHandle());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _light->getShadowMap()->getHandle(), 0);
    glClear(GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::clear(DirectionalLightComponent* _light) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFramebuffer);
    glBindTexture(GL_TEXTURE_2D, _light->getShadowMap()->getHandle());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _light->getShadowMap()->getHandle(), 0);
    glClear(GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::draw(Camera* _camera, Renderable* _object) {

    if (m_useDefaultShaders) setShader("default");

    //bind uniforms
    glViewport(m_VPAbsPos.x, m_VPAbsPos.y, m_VPAbsScale.x, m_VPAbsScale.y);
    setUniformMat4("in_worldTransform", _object->getWorldTransform());
    setUniformMat4("in_cameraTransform", _camera->getCameraTransform());
    setUniformMat4("in_projectionTransform", _camera->getProjectionTransform());
    
    glBindVertexArray(_object->getVertexArray());

    glDrawArrays(GL_TRIANGLES, 0, _object->getVertexBufferSize() / 3);
}

void Renderer::draw(Camera* _camera, StaticSpriteComponent* _sprite, TransformComponent* _transform) {
    if (m_useDefaultShaders) setShader("2DStatic");

    glViewport(m_VPAbsPos.x, m_VPAbsPos.y, m_VPAbsScale.x, m_VPAbsScale.y);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _sprite->m_texture->getHandle());
    setUniformInt("in_texture", 0);

    setUniformMat4("in_worldTransform", _transform->m_worldTransform);
    setUniformMat4("in_cameraTransform", _camera->getCameraTransform());
    setUniformMat4("in_projectionTransform", _camera->getProjectionTransform());
    setUniformIVec2("in_flip", _sprite->flip);
    
    glBindVertexArray(_sprite->m_renderInfo.vertexArray);

    glDrawArrays(GL_TRIANGLES, 0, _sprite->m_renderInfo.numVertices);
}

void Renderer::draw(Camera* _camera, StaticMeshComponent* _mesh, TransformComponent* _transform) {
    if (m_useDefaultShaders) setShader("default3D");

    glViewport(m_VPAbsPos.x, m_VPAbsPos.y, m_VPAbsScale.x, m_VPAbsScale.y);

    setUniformMat4("in_worldTransform", _transform->m_worldTransform);
    setUniformMat4("in_cameraTransform", _camera->getCameraTransform());
    setUniformMat4("in_projectionTransform", _camera->getProjectionTransform());
    setUniformInt("in_texture", 0);

    auto meshes = _mesh->m_meshes;
    for (unsigned int i = 0; i < meshes.size(); i++) {
        Material* material = meshes[i].getMaterial();
        setUniformVec3("in_ambient", material->ka);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->kd_map->getHandle());
        glBindVertexArray(meshes[i].getVertexArray());
        glDrawArrays(GL_TRIANGLES, 0, meshes[i].getVertexBufferSize() / 8);
    }
    
}

void Renderer::draw(DirectionalLightComponent* _light, ViewerTransformComponent* _lightTransform, StaticMeshComponent* _mesh, TransformComponent* _meshTransform) {
    glm::vec2 shadowMapSize = _light->getShadowMap()->getSize();
    glViewport(0, 0, shadowMapSize.x, shadowMapSize.y);

    setShader("shadowMap");

    setUniformMat4("in_worldTransform", _meshTransform->m_worldTransform);
    setUniformMat4("in_lightTransform", _lightTransform->getViewTransform());
    setUniformMat4("in_projectionTransform", _lightTransform->getProjectionTransform());

    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFramebuffer);
    glBindTexture(GL_TEXTURE_2D, _light->getShadowMap()->getHandle());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _light->getShadowMap()->getHandle(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    auto meshes = _mesh->m_meshes;
    for (unsigned int i = 0; i < meshes.size(); i++) {
        glBindVertexArray(meshes[i].getVertexArray());
        glDrawArrays(GL_TRIANGLES, 0, meshes[i].getVertexBufferSize() / 8);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::draw(Camera* _camera, DirectionalLightComponent* _light, ViewerTransformComponent* _lightTransform, StaticMeshComponent* _mesh, TransformComponent* _meshTransform) {
    if (m_useDefaultShaders) setShader("3Dlit");

    glViewport(m_VPAbsPos.x, m_VPAbsPos.y, m_VPAbsScale.x, m_VPAbsScale.y);

    setUniformMat4("in_worldTransform", _meshTransform->m_worldTransform);
    setUniformMat4("in_cameraTransform", _camera->getCameraTransform());
    setUniformMat4("in_projectionTransform", _camera->getProjectionTransform());

    setUniformMat4("in_lightTransform", _lightTransform->getViewTransform());
    setUniformMat4("in_lightProjection", _lightTransform->getProjectionTransform());

    setUniformVec3("in_lightColor", _light->getColor());
    setUniformVec3("in_lightPos", _lightTransform->getPos());
    setUniformVec3("in_cameraPos", _camera->getPosition());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _light->getShadowMap()->getHandle());
    setUniformInt("in_shadowMap", 1);
    setUniformInt("in_texture", 0);

    auto meshes = _mesh->m_meshes;
    for (unsigned int i = 0; i < meshes.size(); i++) {
        Material* material = meshes[i].getMaterial();
        setUniformVec3("in_kAmbient", material->ka);
        setUniformVec3("in_kDiffuse", material->kd);
        setUniformVec3("in_kSpecular", material->ks);
        setUniformFloat("in_specExponent", material->specExponent);
        setUniformFloat("in_opacity", material->opacity);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->kd_map->getHandle());

        glBindVertexArray(meshes[i].getVertexArray());
        glDrawArrays(GL_TRIANGLES, 0, meshes[i].getVertexBufferSize() / 8);
    }
}

void Renderer::draw(Camera* _camera, GameObject* _object) {
    // gameobjects are not renderable but this function is needed when using them generically
}

void Renderer::draw(Camera* _camera, GameObject2DStatic* _object) {

    if (m_useDefaultShaders) setShader("2DStatic");

    glViewport(m_VPAbsPos.x, m_VPAbsPos.y, m_VPAbsScale.x, m_VPAbsScale.y);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _object->getTexture()->getHandle());
    setUniformInt("in_texture", 0);

    setUniformMat4("in_worldTransform", _object->getWorldTransform());
    setUniformMat4("in_cameraTransform", _camera->getCameraTransform());
    setUniformMat4("in_projectionTransform", _camera->getProjectionTransform());
    setUniformIVec2("in_flip", _object->getFlip());
    
    glBindVertexArray(_object->getVertexArray());

    glDrawArrays(GL_TRIANGLES, 0, _object->getVertexBufferSize() / 3);
}

void Renderer::draw(Camera* _camera, GameObject2DAnimated* _object) {

    if (m_useDefaultShaders) setShader("2DAnimated");

    glViewport(m_VPAbsPos.x, m_VPAbsPos.y, m_VPAbsScale.x, m_VPAbsScale.y);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _object->getTexture()->getHandle());
    setUniformInt("in_texture", 0);

    setUniformMat4("in_worldTransform", _object->getWorldTransform());
    setUniformMat4("in_cameraTransform", _camera->getCameraTransform());
    setUniformMat4("in_projectionTransform", _camera->getProjectionTransform());
    setUniformIVec2("in_flip", _object->getFlip());

    setUniformInt("in_animationData.numFrames", _object->getNumFrames());
    setUniformInt("in_animationData.currentFrame", _object->getCurrentFrameNum());
    setUniformInt("in_animationData.rows", _object->getCurrentAnimationRows());

    glBindVertexArray(_object->getVertexArray());

    glDrawArrays(GL_TRIANGLES, 0, _object->getVertexBufferSize() / 3);
}

void Renderer::draw(Camera* _camera, GameObject3D* _object) {
    if (m_useDefaultShaders) setShader("default3D");

    glViewport(m_VPAbsPos.x, m_VPAbsPos.y, m_VPAbsScale.x, m_VPAbsScale.y);

    setUniformMat4("in_worldTransform", _object->getWorldTransform());
    setUniformMat4("in_cameraTransform", _camera->getCameraTransform());
    setUniformMat4("in_projectionTransform", _camera->getProjectionTransform());
    setUniformInt("in_texture", 0);

    auto meshes = _object->getMeshes();
    for (unsigned int i = 0; i < meshes->size(); i++) {
        Material* material = meshes->at(i).getMaterial();
        setUniformVec3("in_ambient", material->ka);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->kd_map->getHandle());
        glBindVertexArray(meshes->at(i).getVertexArray());
        glDrawArrays(GL_TRIANGLES, 0, meshes->at(i).getVertexBufferSize() / 8);
    }
    
}

void Renderer::drawLit(Camera* _camera, GameObject3D* _object, Light* _light) {
    if (m_useDefaultShaders) setShader("3Dlit");

    glViewport(m_VPAbsPos.x, m_VPAbsPos.y, m_VPAbsScale.x, m_VPAbsScale.y);

    setUniformMat4("in_worldTransform", _object->getWorldTransform());
    setUniformMat4("in_cameraTransform", _camera->getCameraTransform());
    setUniformMat4("in_projectionTransform", _camera->getProjectionTransform());

    setUniformMat4("in_lightTransform", _light->getLightTransform());
    setUniformMat4("in_lightProjection", _light->getProjectionTransform());

    setUniformVec3("in_lightColor", _light->getColor());
    setUniformVec3("in_lightPos", _light->getPos());
    setUniformVec3("in_cameraPos", _camera->getPosition());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _light->getShadowMap()->getHandle());
    setUniformInt("in_shadowMap", 1);
    setUniformInt("in_texture", 0);

    auto meshes = _object->getMeshes();
    for (unsigned int i = 0; i < meshes->size(); i++) {
        Material* material = meshes->at(i).getMaterial();
        setUniformVec3("in_kAmbient", material->ka);
        setUniformVec3("in_kDiffuse", material->kd);
        setUniformVec3("in_kSpecular", material->ks);
        setUniformFloat("in_specExponent", material->specExponent);
        setUniformFloat("in_opacity", material->opacity);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->kd_map->getHandle());

        glBindVertexArray(meshes->at(i).getVertexArray());
        glDrawArrays(GL_TRIANGLES, 0, meshes->at(i).getVertexBufferSize() / 8);
    }
}

void Renderer::draw(MenuItem* _item) {
    
    if (m_useDefaultShaders) setShader("menuShader");

    glViewport(m_VPAbsPos.x, m_VPAbsPos.y, m_VPAbsScale.x, m_VPAbsScale.y);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _item->getTexture()->getHandle());
    setUniformInt("in_texture", 0);
    setUniformVec4("in_location", _item->getLocation());

    glBindVertexArray(_item->getVertexArray());
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::draw(Camera* _camera, const std::string& _text) {
    if (m_useDefaultShaders) setShader("textShader");
    glViewport(m_VPAbsPos.x, m_VPAbsPos.y, m_VPAbsScale.x, m_VPAbsScale.y);
    drawText(_camera, _text, glm::vec3(1), 1, glm::vec4(1));
}

void Renderer::drawText(Camera* _camera, const std::string& _text, glm::vec3 _pos, float _scale, glm::vec4 _color) {
    setShader("textShader");
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_textVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, m_textVertexBuffer);

    setUniformMat4("in_projection", _camera->getProjectionTransform());
    setUniformMat4("in_cameraTransform", _camera->getCameraTransform());
    setUniformVec3("in_color", _color);

    //std::string::iterator it;
    for (auto it = _text.begin(); it != _text.end(); it++) {
        FontCharacter c = m_fontMapTerm[*it];

        float xpos = _pos.x + c.bearing.x * _scale;
        float ypos = _pos.y - (c.size.y - c.bearing.y) * _scale;

        float w = c.size.x * _scale;
        float h = c.size.y * _scale;

        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };

        glBindTexture(GL_TEXTURE_2D, c.textureHandle);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        _pos.x += (c.advance >> 6) * _scale;
    }
    
}

void Renderer::textToTexture(Texture* _texture, const std::string& _text, glm::vec4 _textColor, glm::vec3 _backgroundColor) {
    glm::ivec2 textureSize = _texture->getSize();

    glBindFramebuffer(GL_FRAMEBUFFER, m_textureFramebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture->getHandle(), 0);

    //GLenum buffers[1] = {GL_COLOR_ATTACHMENT0};
    //glDrawBuffers(1, buffers);
    
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("incomplete framebuffer when rendering to texture");
    }

    glViewport(0, 0, textureSize.x, textureSize.y);
    glClearColor(_backgroundColor.x, _backgroundColor.y, _backgroundColor.x, _backgroundColor.y);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    unsigned int prev_shader = m_CurrentShader;
    setShader("textShader");

    drawMenuText(_text, 0, 0, 1, _textColor);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // regenerate the mipmaps
    glBindTexture(GL_TEXTURE_2D, _texture->getHandle());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    m_CurrentShader = prev_shader;
    glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
}

void Renderer::drawMenuText(const std::string& _text, float _x, float _y, float _scale, glm::vec4 _textColor) {

    std::string::const_iterator it;

    glm::vec2 padding = glm::vec2(0.1, 0);

    glm::vec2 totalTextSize = glm::vec2(0);
    float maxyBearing = 0;

    for (it = _text.begin(); it != _text.end(); it++) {
        FontCharacter c = m_fontMapTerm[*it];
        totalTextSize.x += (c.advance >> 6);
        if (c.size.y > totalTextSize.y) {
            totalTextSize.y = c.size.y;
        }
        if (c.bearing.y > maxyBearing) {
            maxyBearing = c.bearing.y;
        }
    }
    totalTextSize.x = totalTextSize.x * _scale;
    totalTextSize.y = totalTextSize.y * _scale + (maxyBearing/2);
    glm::vec2 absPadding = totalTextSize * padding;
    
    glm::mat4 projection = glm::ortho(-absPadding.x, totalTextSize.x + absPadding.x, -maxyBearing/2 -absPadding.y, totalTextSize.y + absPadding.y);
    glm::mat4 cameraTransform = glm::identity<glm::mat4>();

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_textVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, m_textVertexBuffer);

    setUniformMat4("in_projection", projection);
    setUniformMat4("in_cameraTransform", cameraTransform);
    setUniformVec3("in_color", _textColor);

    for (it = _text.begin(); it != _text.end(); it++) {
        FontCharacter c = m_fontMapTerm[*it];

        float xpos = _x + c.bearing.x * _scale;
        float ypos = _y - (c.size.y - c.bearing.y) * _scale;

        float w = c.size.x * _scale;
        float h = c.size.y * _scale;

        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };

        glBindTexture(GL_TEXTURE_2D, c.textureHandle);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        _x += (c.advance >> 6) * _scale;
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::drawShadow(Light* _light, GameObject2D* _object) {
    glm::vec2 shadowMapSize = _light->getShadowMap()->getSize();
    glViewport(0, 0, shadowMapSize.x, shadowMapSize.y);

    setShader("shadowMap");

    setUniformMat4("in_worldTransform", _object->getWorldTransform());
    setUniformMat4("in_lightTransform", _light->getLightTransform());
    setUniformMat4("in_projectionTransform", _light->getProjectionTransform());

    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFramebuffer);
    glBindTexture(GL_TEXTURE_2D, _light->getShadowMap()->getHandle());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _light->getShadowMap()->getHandle(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        //throw std::runtime_error("incomplete framebuffer when rendering to texture");
    //}

    glBindVertexArray(_object->getVertexArray());
    glDrawArrays(GL_TRIANGLES, 0, _object->getVertexBufferSize() / 3);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::drawShadow(Light* _light, GameObject3D* _object) {
    glm::vec2 shadowMapSize = _light->getShadowMap()->getSize();
    glViewport(0, 0, shadowMapSize.x, shadowMapSize.y);

    setShader("shadowMap");

    setUniformMat4("in_worldTransform", _object->getWorldTransform());
    setUniformMat4("in_lightTransform", _light->getLightTransform());
    setUniformMat4("in_projectionTransform", _light->getProjectionTransform());

    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFramebuffer);
    glBindTexture(GL_TEXTURE_2D, _light->getShadowMap()->getHandle());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _light->getShadowMap()->getHandle(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        //throw std::runtime_error("incomplete framebuffer when rendering to texture");
    //}

    auto meshes = _object->getMeshes();
    for (unsigned int i = 0; i < meshes->size(); i++) {
        Material* material = meshes->at(i).getMaterial();
        setUniformVec3("in_ambient", material->ka);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, material->kd_map->getHandle());
        glBindVertexArray(meshes->at(i).getVertexArray());
        glDrawArrays(GL_TRIANGLES, 0, meshes->at(i).getVertexBufferSize() / 8);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::setUniformMat4(const char* _name, glm::mat4 _value) {
    unsigned int loc = glGetUniformLocation(m_CurrentShader, _name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(_value));
}

void Renderer::setUniformVec4(const char* _name, glm::vec4 _value) {
    unsigned int loc = glGetUniformLocation(m_CurrentShader, _name);
    glUniform4fv(loc, 1, glm::value_ptr(_value));
}

void Renderer::setUniformVec3(const char* _name, glm::vec3 _value) {
    unsigned int loc = glGetUniformLocation(m_CurrentShader, _name);
    glUniform3fv(loc, 1, glm::value_ptr(_value));
}

void Renderer::setUniformIVec2(const char* _name, glm::vec2 _value) {
    unsigned int loc = glGetUniformLocation(m_CurrentShader, _name);
    glUniform2i(loc, _value.x, _value.y);
}

void Renderer::setUniformInt(const char* _name, int _value) {
    unsigned int loc = glGetUniformLocation(m_CurrentShader, _name);
    glUniform1i(loc, _value);
}

void Renderer::setUniformFloat(const char* _name, float _value) {
    unsigned int loc = glGetUniformLocation(m_CurrentShader, _name);
    glUniform1f(loc, _value);
}

void Renderer::setShader(const std::string& _shader) {
    if (m_Shaders.count(_shader) < 1) {
        throw std::runtime_error("shader " + _shader + "is not loaded");
    }
    m_CurrentShader = m_Shaders[_shader];
    glUseProgram(m_CurrentShader);
}

void Renderer::setShaderPath(const std::string& _shaderPath) {
    m_ShaderPath = _shaderPath;
}

void Renderer::loadShaders() {
    // load the shaders to use
    std::vector<std::string> shaderNames = {"default", "2DAnimated", "2DStatic", "menuShader", "textShader", "default3D", "shadowMap", "3Dlit"};

    for (const std::string& shader : shaderNames) {
        // load the shader
        std::string vertSource = FileHandler::loadShaderCode(m_ShaderPath + shader + ".vert.glsl");
        std::string fragSource = FileHandler::loadShaderCode(m_ShaderPath + shader + ".frag.glsl");

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* vertSourceC = vertSource.c_str();
        const char* fragSourceC = fragSource.c_str();

        glShaderSource(vertexShader, 1, &vertSourceC, NULL);
        glShaderSource(fragmentShader, 1, &fragSourceC, NULL);

        int success;
        char info[512];

        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, info);
            std::cout << info << std::endl;
            throw std::runtime_error("failed to compile vertex shader: " + shader);
        }

        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, info);
            std::cout << info << std::endl;
            throw std::runtime_error("failed to compile fragment shader: " + shader);
        }

        unsigned int program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, info);
            std::cout << info << std::endl;
            throw std::runtime_error("failed to link shader program: " + shader);
        }

        m_Shaders[shader] = program;

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    setShader("default");
}

void Renderer::makeTextureRenderDependancies() {
    glGenFramebuffers(1, &m_textureFramebuffer);
}
void Renderer::makeShadowDependencies() {
    glGenFramebuffers(1, &m_shadowFramebuffer);
}

void Renderer::initFreetype() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        throw std::runtime_error("failed to load freetype library");
    }
    FT_Face face;
    if (FT_New_Face(ft, "fonts/term.ttf", 0, &face)) {
        throw std::runtime_error("failed to load font");
    }
    FT_Set_Pixel_Sizes(face, 0, 512);

    m_fontMapTerm = std::map<char, FontCharacter>();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //disable byte alignment restriction
    
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            throw std::runtime_error("failed to load character");
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                face->glyph->bitmap.width, face->glyph->bitmap.rows,
                0, GL_RED, GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
                );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        FontCharacter character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x),
        };
        
        m_fontMapTerm[c] = character;

    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &m_textVertexArray);
    glGenBuffers(1, &m_textVertexBuffer);

    glBindVertexArray(m_textVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, m_textVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::resize() {
    glm::vec2 winDim = m_Window->getDimensions();
    m_VPAbsScale.x = winDim.x * m_VPScale.x;
    m_VPAbsScale.y = winDim.y * m_VPScale.y;
    m_VPAbsPos.x = winDim.x * m_VPPos.x;
    m_VPAbsPos.y = winDim.y * m_VPPos.y;
}

}
