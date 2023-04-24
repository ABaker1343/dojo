#include "headers/renderer.hpp"
#include <GLFW/glfw3.h>

namespace dojo {

Renderer::Renderer(Window* _window, glm::vec2 _VPPos, glm::vec2 _VPSize) {

    m_Window = _window;

    // glfw window hints

    m_VPScale = _VPSize;
    m_VPPos = _VPPos;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    setShaderPath("shaders/");
    resize();
    _window->bindRenderer(this);

    initFreetype();

    // generate framebuffers
    makeTextureRenderDependancies();
    
}

Renderer::~Renderer() {
    // remove all the programs
    std::map<std::string, unsigned int>::iterator it;
    for (it = m_Shaders.begin(); it != m_Shaders.end(); it++){
        glDeleteProgram(it->second);
    }
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(Camera* _camera, Renderable* _object) {
    //bind uniforms
    glViewport(m_VPAbsPos.x, m_VPAbsPos.y, m_VPAbsScale.x, m_VPAbsScale.y);
    setUniformMat4("in_worldTransform", _object->getWorldTransform());
    setUniformMat4("in_cameraTransform", _camera->getCameraTransform());
    setUniformMat4("in_projectionTransform", _camera->getProjectionTransform());
    
    glBindVertexArray(_object->getVertexArray());

    glDrawArrays(GL_TRIANGLES, 0, _object->getVertexBufferSize() / 3);
}

void Renderer::draw(Camera* _camera, GameObject2DStatic* _object) {
    glViewport(m_VPAbsPos.x, m_VPAbsPos.y, m_VPAbsScale.x, m_VPAbsScale.y);
    glBindTexture(GL_TEXTURE_2D, _object->getTexture()->getHandle());

    setUniformMat4("in_worldTransform", _object->getWorldTransform());
    setUniformMat4("in_cameraTransform", _camera->getCameraTransform());
    setUniformMat4("in_projectionTransform", _camera->getProjectionTransform());
    setUniformIVec2("in_flip", _object->getFlip());
    
    glBindVertexArray(_object->getVertexArray());

    glDrawArrays(GL_TRIANGLES, 0, _object->getVertexBufferSize() / 3);
}

void Renderer::draw(Camera* _camera, GameObject2DAnimated* _object) {
    glViewport(m_VPAbsPos.x, m_VPAbsPos.y, m_VPAbsScale.x, m_VPAbsScale.y);
    glBindTexture(GL_TEXTURE_2D, _object->getTexture()->getHandle());

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

void Renderer::draw(MenuItem* _item) {
    glViewport(m_VPAbsPos.x, m_VPAbsPos.y, m_VPAbsScale.x, m_VPAbsScale.y);
    glBindTexture(GL_TEXTURE_2D, _item->getTexture()->getHandle());

    glBindVertexArray(_item->getVertexArray());
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::drawToTexture(Texture* _texture, const std::string& _text, float _x, float _y, float _scale) {
    glm::ivec2 textureSize = _texture->getSize();

    glBindFramebuffer(GL_FRAMEBUFFER, m_textureFramebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture->getHandle(), 0);

    //GLenum buffers[1] = {GL_COLOR_ATTACHMENT0};
    //glDrawBuffers(1, buffers);
    
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("incomplete framebuffer when rendering to texture");
    }

    std::cout << textureSize.x << std::endl;
    glViewport(0, 0, textureSize.x, textureSize.y);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    unsigned int prev_shader = m_CurrentShader;

    drawMenuText(_text, _x, _y, _scale);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    m_CurrentShader = prev_shader;
}

void Renderer::drawMenuText(const std::string& _text, float _x, float _y, float _scale) {
    setShader("textShader");
    //glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_textVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, m_textVertexBuffer);

    auto projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    setUniformMat4("in_projection", projection);
    setUniformVec3("in_color", glm::vec3(1.0));

    std::string::const_iterator it;

    for (it = _text.begin(); it != _text.end(); it++) {
        FontCharacter c = m_fontMapTerm[*it];

        float xpos = _x + c.bearing.x * _scale;
        float ypos = _y - (c.size.y - c.bearing.y) * _scale;

        float w = c.size.x * _scale;
        float h = c.size.y * _scale;

        std::cout << *it << std::endl;

        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        /*float vertices[24] = {
            0.0, 0.0,
            0.0, 0.0,
            0.0, 1.0,
            0.0, 1.0,
            1.0, 1.0,
            1.0, 1.0,
            1.0, 1.0,
            1.0, 1.0,
            1.0, 0.0,
            1.0, 0.0,
            0.0, 0.0,
            0.0, 0.0,
        };*/

        glBindTexture(GL_TEXTURE_2D, c.textureHandle);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        _x += (c.advance >> 6) * _scale;
    }
    glBindVertexArray(0);
}

void Renderer::setUniformMat4(const char* _name, glm::mat4 _value) {
    unsigned int loc = glGetUniformLocation(m_CurrentShader, _name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(_value));
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
    std::vector<std::string> shaderNames = {"default", "2DAnimated", "2DStatic", "menuShader", "textShader"};

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

void Renderer::initFreetype() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        throw std::runtime_error("failed to load freetype library");
    }
    FT_Face face;
    if (FT_New_Face(ft, "fonts/term.ttf", 0, &face)) {
        throw std::runtime_error("failed to load font");
    }
    FT_Set_Pixel_Sizes(face, 0, 48);

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
