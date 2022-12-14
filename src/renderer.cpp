#include "headers/renderer.hpp"

namespace dojo {

    Renderer::Renderer(Window *window) {
        glfwMakeContextCurrent(window->window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("failed to load opengl functions");
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        // allow color blending

        glViewport(0, 0, window->winWidth, window->winHeight);

        // initialize true type

        FT_Library freetype;
        if (FT_Init_FreeType(&freetype) != 0) {
            throw std::runtime_error("failed to initialize freetype font rendering library");
        }

        FT_Face ftArial;
        if (FT_New_Face(freetype, "fonts/arial.ttf", 0, &ftArial) != 0) {
            throw std::runtime_error("failed to load arial font");
        }

        FT_Set_Pixel_Sizes(ftArial, 0, 48);

        // create the fonts

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        characters = new std::map<char, Character>();

        for (unsigned char c = 0; c < 128; c++){

            // load the characters glyph
            if (FT_Load_Char(ftArial, c, FT_LOAD_RENDER) != 0) {
                throw std::runtime_error("failed to load character in font");
            }

            //generate the texture for the font

            unsigned int glyphTexture = 0;
            glGenTextures(1, &glyphTexture);
            glBindTexture(GL_TEXTURE_2D, glyphTexture);

            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED, // one byte
                    ftArial->glyph->bitmap.width,
                    ftArial->glyph->bitmap.rows,
                    0,
                    GL_RED, // one byte
                    GL_UNSIGNED_BYTE,
                    ftArial->glyph->bitmap.buffer
                    );

            // set the texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // create the character

            Character character = {
                glyphTexture,
                glm::ivec2(ftArial->glyph->bitmap.width, ftArial->glyph->bitmap.rows),
                glm::ivec2(ftArial->glyph->bitmap_left, ftArial->glyph->bitmap_top),
                ftArial->glyph->advance.x
            };

            characters->insert(std::pair<char, Character>(c, character));
        }

        FT_Done_Face(ftArial);
        FT_Done_FreeType(freetype);

        textVertices = new std::vector<float> {
            0.f, 0.f, 0.f, 1.f,
                0.f, 0.f, 0.f, 0.f,
                0.f, 0.f, 1.f, 0.f,
                0.f, 0.f, 1.f, 0.f,
                0.f, 0.f, 1.f, 1.f,
                0.f, 0.f, 0.f, 1.f,
        };

        glGenVertexArrays(1, &textVertexArray);
        glBindVertexArray(textVertexArray);
        glGenBuffers(1, &textVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, textVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textVertices->size(), textVertices->data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        textColor = glm::vec3(1.f, 1.f, 1.f);


        // initialize the window class and all its parts

        createShadowMapDependancies();
        createShadowCubeMapDependancies();
        createTextFramebuffer();

        createCollisionBoxRenderDependancies();
        colliderColor = glm::vec4(0.f, 1.f, 0.f, 0.2f);

        createShaderPrograms();
    }

    Renderer::~Renderer() {
        delete boxElements;
        delete boxVertices;

        glDeleteBuffers(1, &boxElementBuffer);
        glDeleteBuffers(1, &collisionBoxVertexBuffer);
        glDeleteVertexArrays(1, &collisionBoxVertexArray);
    }

void Renderer::clear(Window *window) {
    glfwSwapBuffers(window->window);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
}

void Renderer::render(Window *window, Camera3D *c, GameObject2DSprite *s) {
    glViewport(c->viewport.x * window->winWidth, c->viewport.y * window->winHeight, c->viewport.w * window->winWidth, c->viewport.h * window->winHeight);
    glUseProgram(shaderProgram2D);
    glBindVertexArray(s->vertexArrayObject);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s->texture);
    int textureLocation = glGetUniformLocation(shaderProgram2D, "inTexture");
    glUniform1i(textureLocation, 0);

    int flipUniformPos = glGetUniformLocation(shaderProgram2D, "flip");
    glUniform1i(flipUniformPos, s->flip);

    int objectTransformLocation = glGetUniformLocation(shaderProgram2D, "objectTransform");
    int viewTransformLocation = glGetUniformLocation(shaderProgram2D, "viewTransform");
    int projectionTransformLocation = glGetUniformLocation(shaderProgram2D, "projection");

    glUniformMatrix4fv(objectTransformLocation, 1, GL_FALSE, glm::value_ptr(s->transform));
    glUniformMatrix4fv(viewTransformLocation, 1, GL_FALSE, glm::value_ptr(c->getTransform()));
    glUniformMatrix4fv(projectionTransformLocation, 1, GL_FALSE, glm::value_ptr(c->projection));

    int animationFrameUniformLocation = glGetUniformLocation(shaderProgram2D, "animationFrame");
    glUniform1i(animationFrameUniformLocation, 0);

    int animationChunkSizeUniformLocation = glGetUniformLocation(shaderProgram2D, "animationChunkSize");
    glUniform1f(animationChunkSizeUniformLocation, 1.f);

    glDrawElements(GL_TRIANGLES, s->numElements(), GL_UNSIGNED_INT, 0);

}

void Renderer::render(Window *window, Camera3D *c, GameObject2DAnimatedSprite *s) {
    glViewport(c->viewport.x * window->winWidth, c->viewport.y * window->winHeight, c->viewport.w * window->winWidth, c->viewport.h * window->winHeight);
    glUseProgram(shaderProgram2D);
    glBindVertexArray(s->vertexArrayObject);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s->currentTexture());

    int flipUniformPos = glGetUniformLocation(shaderProgram2D, "flip");
    glUniform1i(flipUniformPos, s->flip);

    int objectTransformLocation = glGetUniformLocation(shaderProgram2D, "objectTransform");
    int viewTransformLocation = glGetUniformLocation(shaderProgram2D, "viewTransform");
    int projectionTransformLocation = glGetUniformLocation(shaderProgram2D, "projection");

    glUniformMatrix4fv(objectTransformLocation, 1, GL_FALSE, glm::value_ptr(s->transform));
    glUniformMatrix4fv(viewTransformLocation, 1, GL_FALSE, glm::value_ptr(c->getTransform()));
    glUniformMatrix4fv(projectionTransformLocation, 1, GL_FALSE, glm::value_ptr(c->projection));

    int animationFrameUniformLocation = glGetUniformLocation(shaderProgram2D, "animationFrame");
    glUniform1i(animationFrameUniformLocation, s->currentFrame());

    int animationChunkSizeUniformLocation = glGetUniformLocation(shaderProgram2D, "animationChunkSize");
    glUniform1f(animationChunkSizeUniformLocation, s->currentAnimationChunkSize());

    glDrawElements(GL_TRIANGLES, s->numElements(), GL_UNSIGNED_INT, 0);
}

void Renderer::render(Window *window, Camera3D *c, GameObject3DTextured *obj, GameObjectSpotLightSource *light, float ambientIntensity) {
    glViewport(c->viewport.x * window->winWidth, c->viewport.y * window->winHeight, c->viewport.w * window->winWidth, c->viewport.h * window->winHeight);
    glUseProgram(spotLightShaderProgram);
    glBindVertexArray(obj->vertexArrayObject);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, obj->texture);
    int objectTextureLocation = glGetUniformLocation(spotLightShaderProgram, "inTexture");
    glUniform1i(objectTextureLocation, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    int depthMapTextureLocation = glGetUniformLocation(spotLightShaderProgram, "inDepthMap");
    glUniform1i(depthMapTextureLocation, 1);

    int flipUniformPos = glGetUniformLocation(spotLightShaderProgram, "flip");
    glUniform1i(flipUniformPos, obj->flip);

    int objectTransformLocation = glGetUniformLocation(spotLightShaderProgram, "objectTransform");
    int viewTransformLocation = glGetUniformLocation(spotLightShaderProgram, "viewTransform");
    int projectionTransformLocation = glGetUniformLocation(spotLightShaderProgram, "projection");

    glUniformMatrix4fv(objectTransformLocation, 1, GL_FALSE, glm::value_ptr(obj->transform));
    glUniformMatrix4fv(viewTransformLocation, 1, GL_FALSE, glm::value_ptr(c->getTransform()));
    glUniformMatrix4fv(projectionTransformLocation, 1, GL_FALSE, glm::value_ptr(c->getProjectionTransform()));

    int animationFrameUniformLocation = glGetUniformLocation(spotLightShaderProgram, "animationFrame");
    int animationChunkSizeUniformLocation = glGetUniformLocation(spotLightShaderProgram, "animationChunkSize");

    int lightColorLocation = glGetUniformLocation(spotLightShaderProgram, "lightColor");
    int lightPosLocation = glGetUniformLocation(spotLightShaderProgram, "lightPos");
    int ambientIntensityLocation = glGetUniformLocation(spotLightShaderProgram, "ambientIntensity");

    int lightViewTransformLocation = glGetUniformLocation(spotLightShaderProgram, "lightViewTransform");
    int lightProjectionTransformLocation = glGetUniformLocation(spotLightShaderProgram, "lightProjectionTransform");

    glUniform1i(animationFrameUniformLocation, 0);
    glUniform1f(animationChunkSizeUniformLocation, 1.f);

    glUniform3fv(lightColorLocation, 1, glm::value_ptr(light->lightColor));
    glUniform3fv(lightPosLocation, 1, glm::value_ptr(light->getPos()));
    glUniform1f(ambientIntensityLocation, ambientIntensity);

    glUniformMatrix4fv(lightViewTransformLocation, 1, GL_FALSE, glm::value_ptr(light->getViewTransform()));
    glUniformMatrix4fv(lightProjectionTransformLocation, 1, GL_FALSE, glm::value_ptr(light->getProjectionTransform()));

    glDrawArrays(GL_TRIANGLES, 0, obj->numVertices());

}

void Renderer::render(Window *window, Camera3D *c, GameObject3DTextured *obj, GameObjectPointLightSource *light, float ambientIntensity) {
    glViewport(c->viewport.x * window->winWidth, c->viewport.y * window->winHeight, c->viewport.w * window->winWidth, c->viewport.h * window->winHeight);
    glUseProgram(pointLightShaderProgram);
    glBindVertexArray(obj->vertexArrayObject);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, obj->texture);
    int objectTextureLocation = glGetUniformLocation(pointLightShaderProgram, "inTexture");
    glUniform1i(objectTextureLocation, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    int depthMapTextureLocation = glGetUniformLocation(pointLightShaderProgram, "inDepthMap");
    glUniform1i(depthMapTextureLocation, 1);

    int flipUniformPos = glGetUniformLocation(pointLightShaderProgram, "flip");
    glUniform1i(flipUniformPos, obj->flip);

    int objectTransformLocation = glGetUniformLocation(pointLightShaderProgram, "objectTransform");
    int viewTransformLocation = glGetUniformLocation(pointLightShaderProgram, "viewTransform");
    int projectionTransformLocation = glGetUniformLocation(pointLightShaderProgram, "projection");

    glUniformMatrix4fv(objectTransformLocation, 1, GL_FALSE, glm::value_ptr(obj->transform));
    glUniformMatrix4fv(viewTransformLocation, 1, GL_FALSE, glm::value_ptr(c->getTransform()));
    glUniformMatrix4fv(projectionTransformLocation, 1, GL_FALSE, glm::value_ptr(c->getProjectionTransform()));

    int animationFrameUniformLocation = glGetUniformLocation(pointLightShaderProgram, "animationFrame");
    int animationChunkSizeUniformLocation = glGetUniformLocation(pointLightShaderProgram, "animationChunkSize");

    int lightColorLocation = glGetUniformLocation(pointLightShaderProgram, "lightColor");
    int lightPosLocation = glGetUniformLocation(pointLightShaderProgram, "lightPos");
    int ambientIntensityLocation = glGetUniformLocation(spotLightShaderProgram, "ambientIntensity");

    int farPlaneLocation = glGetUniformLocation(pointLightShaderProgram, "farPlane");

    glUniform1i(animationFrameUniformLocation, 0);
    glUniform1f(animationChunkSizeUniformLocation, 1.f);

    glUniform3fv(lightColorLocation, 1, glm::value_ptr(light->lightColor));
    glUniform3fv(lightPosLocation, 1, glm::value_ptr(light->getPos()));
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(farPlaneLocation, light->farPlane);

    glDrawArrays(GL_TRIANGLES, 0, obj->numVertices());

}

void Renderer::render(Window *window, Camera3D *c, GameObject2DCollisionBox *b) {
    glViewport(c->viewport.x * window->winWidth, c->viewport.y * window->winHeight, c->viewport.w * window->winWidth, c->viewport.h * window->winHeight);

    glUseProgram(collisionBoxShaderProgram);
    glBindVertexArray(collisionBoxVertexArray);

    int objectTransformLocation = glGetUniformLocation(collisionBoxShaderProgram, "objectTransform");
    int viewTransformLocation = glGetUniformLocation(collisionBoxShaderProgram, "viewTransform");
    int projectionTransformLocation = glGetUniformLocation(collisionBoxShaderProgram, "projection");
    int colorUniformLocation = glGetUniformLocation(collisionBoxShaderProgram, "inColor");

    glUniformMatrix4fv(objectTransformLocation, 1, GL_FALSE, glm::value_ptr(b->getTransform()));
    glUniformMatrix4fv(viewTransformLocation, 1, GL_FALSE, glm::value_ptr(c->getTransform()));
    glUniformMatrix4fv(projectionTransformLocation, 1, GL_FALSE, glm::value_ptr(c->projection));
    glUniform4fv(colorUniformLocation, 1, glm::value_ptr(colliderColor));

    glDrawElements(GL_TRIANGLES, boxElements->size(), GL_UNSIGNED_INT, 0);
}

void Renderer::render(Window *window, Camera3D *c, GameObjectSpotLightSource *light) {
    glViewport(c->viewport.x * window->winWidth, c->viewport.y * window->winHeight, c->viewport.w * window->winWidth, c->viewport.h * window->winHeight);

    glUseProgram(collisionBoxShaderProgram);
    glBindVertexArray(collisionBoxVertexArray);

    int objectTransformLocation = glGetUniformLocation(collisionBoxShaderProgram, "objectTransform");
    int viewTransformLocation = glGetUniformLocation(collisionBoxShaderProgram, "viewTransform");
    int projectionTransformLocation = glGetUniformLocation(collisionBoxShaderProgram, "projection");
    int colorUniformLocation = glGetUniformLocation(collisionBoxShaderProgram, "inColor");

    glUniformMatrix4fv(objectTransformLocation, 1, GL_FALSE, glm::value_ptr(light->getTransform()));
    glUniformMatrix4fv(viewTransformLocation, 1, GL_FALSE, glm::value_ptr(c->transform));
    glUniformMatrix4fv(projectionTransformLocation, 1, GL_FALSE, glm::value_ptr(c->projection));
    glUniform4fv(colorUniformLocation, 1, glm::value_ptr(glm::vec4(light->lightColor, 1.f)));

    glDrawElements(GL_TRIANGLES, boxElements->size(), GL_UNSIGNED_INT, 0);
    
}

void Renderer::render(Window *window, Camera3D *c, GameObjectPointLightSource *light) {
    glViewport(c->viewport.x * window->winWidth, c->viewport.y * window->winHeight, c->viewport.w * window->winWidth, c->viewport.h * window->winHeight);

    glUseProgram(collisionBoxShaderProgram);
    glBindVertexArray(collisionBoxVertexArray);

    int objectTransformLocation = glGetUniformLocation(collisionBoxShaderProgram, "objectTransform");
    int viewTransformLocation = glGetUniformLocation(collisionBoxShaderProgram, "viewTransform");
    int projectionTransformLocation = glGetUniformLocation(collisionBoxShaderProgram, "projection");
    int colorUniformLocation = glGetUniformLocation(collisionBoxShaderProgram, "inColor");

    glUniformMatrix4fv(objectTransformLocation, 1, GL_FALSE, glm::value_ptr(light->getTransform()));
    glUniformMatrix4fv(viewTransformLocation, 1, GL_FALSE, glm::value_ptr(c->transform));
    glUniformMatrix4fv(projectionTransformLocation, 1, GL_FALSE, glm::value_ptr(c->projection));
    glUniform4fv(colorUniformLocation, 1, glm::value_ptr(glm::vec4(light->lightColor, 1.f)));

    glDrawElements(GL_TRIANGLES, boxElements->size(), GL_UNSIGNED_INT, 0);
}

void Renderer::render(Window *window, MenuItem* item) {

    glViewport(0, 0, window->winWidth, window->winHeight);

    if (item->texture == 0) {

        glUseProgram(menuItemShaderProgram);
        glBindVertexArray(item->vertexArrayObject);

        int backgroundColorLocation = glGetUniformLocation(menuItemShaderProgram, "backgroundColor");

        glUniform3fv(backgroundColorLocation, 1, glm::value_ptr(item->backgroundColor));

        glDrawArrays(GL_TRIANGLES, 0 , item->numVertices());

        // render the text on top of the menu item

        glUseProgram(textShaderProgram);
        glBindVertexArray(textVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, textVertexBuffer);

        textColor = item->textColor;

        int textColorLocation = glGetUniformLocation(textShaderProgram, "textColor");
        glUniform3fv(textColorLocation, 1, glm::value_ptr(textColor));

        glActiveTexture(GL_TEXTURE0);
        int textureLocation = glGetUniformLocation(textShaderProgram, "glyphTexture");
        glUniform1i(textureLocation, 0);

        int numCharacters = item->content.length();
        float scaleX = ( item->screenPos.z * 2 ) / numCharacters;
        float scaleY = ( item->screenPos.w * 2) / numCharacters;

        float textOffsetX = 0;
        float textOffsetY = (item->screenPos.w);

        for (char c : item->content) {

            Character charac = characters->at(c);

            float x = item->screenPos.x * 2 - 1 + textOffsetX;
            float y = item->screenPos.y * 2 - 1 + textOffsetY;
            float w = scaleX;
            float h = scaleY;

            (*textVertices)[0] = x;
            (*textVertices)[1] = y;

            (*textVertices)[4] = x;
            (*textVertices)[5] = y + h;

            (*textVertices)[8] = x + w;
            (*textVertices)[9] = y + h;

            (*textVertices)[12] = x + w;
            (*textVertices)[13] = y + h;

            (*textVertices)[16] = x + w;
            (*textVertices)[17] = y;

            (*textVertices)[20] = x;
            (*textVertices)[21] = y;

            textOffsetX+=scaleX;
            //textOffsetY+=scaleY;

            glBindTexture(GL_TEXTURE_2D, charac.texture);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * textVertices->size(), textVertices->data());

            glDrawArrays(GL_TRIANGLES, 0, 6);

        }

    } else {

        glUseProgram(menuItemTexturedShaderProgram);
        glBindVertexArray(item->vertexArrayObject);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, item->texture);
        int textureLocation = glGetUniformLocation(menuItemTexturedShaderProgram, "inTexture");
        glUniform1i(textureLocation, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

    }

}

void Renderer::createCollisionBoxRenderDependancies() {
    // create the buffers


    boxVertices = new std::vector<float> {
        0.0f, 0.0f, 0.0f,
        0.f, 1.f, 0.f,
        1.f, 1.f, 0.f,
        1.f, 0.f, 0.f,
    };

    boxElements = new std::vector<unsigned int> {
        0, 1, 2,
        2, 3, 0
    };

    collisionBoxVertexBuffer = Renderable::createVertexBuffer(boxVertices);
    glGenVertexArrays(1, &collisionBoxVertexArray);
    glBindVertexArray(collisionBoxVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, collisionBoxVertexBuffer);
    glGenBuffers(1, &boxElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * boxElements->size(), boxElements->data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0 );
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);

    // create shaders


}

void Renderer::createShadowMapDependancies() {

    // create a new framebuffer to render our depth map to
    glGenFramebuffers(1, &depthMapFrameBuffer);

    // create the texture for the depth map
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    float shadowBorderColor[] = {1.f, 1.f, 1.f, 1.f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, shadowBorderColor);

    // attach the texture to the framebuffers depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::createShadowCubeMapDependancies() {
    
    // generate the cubeMap texture
    glGenFramebuffers(1, &depthCubeMapFrameBuffer);
    glGenTextures(1, &depthCubeMap);

    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    for (unsigned int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // bind the cube map texture to the depth buffer
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFrameBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void Renderer::createTextFramebuffer() {
    glGenFramebuffers(1, &textFramebuffer);
}

void Renderer::createShaderPrograms() {
    // create shaders for 2D gameObjects
    shaderProgram2D = Renderable::createBasicShaderProgram("basic2DVert.vert", "basic2DFrag.frag");
    collisionBoxShaderProgram = Renderable::createBasicShaderProgram("basicSolidColorVert.vert", "basicSolidColorFrag.frag");
    textShaderProgram = Renderable::createBasicShaderProgram("textShader.vert", "textShader.frag");

    menuItemShaderProgram = Renderable::createBasicShaderProgram("menuItem.vert", "menuItem.frag");
    menuItemTexturedShaderProgram = Renderable::createBasicShaderProgram("menuItem.vert", "menuItemTextured.frag");

    // create shader programs that will render to framebuffers for depth mapping
    shadowMapShaderProgram = Renderable::createBasicShaderProgram("shadowVert.vert", "emptyFrag.frag");
    cubeMapShaderProgram = Renderable::createBasicShaderProgramWithGeometry("cubeMapShader.vert", "cubeMapShader.geom", "cubeMapShader.frag");

    // create shader programs that will render with a light
    pointLightShaderProgram = Renderable::createBasicShaderProgram("texturedPointLight.vert", "texturedPointLight.frag");
    spotLightShaderProgram = Renderable::createBasicShaderProgram("texturedSpotLight.vert", "texturedSpotLight.frag");
}

void Renderer::renderShadows(GameObject3DTextured *obj, GameObjectSpotLightSource *light) {

    //glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(shadowMapShaderProgram);
    glBindVertexArray(obj->vertexArrayObject);

    int lightViewLocation = glGetUniformLocation(shadowMapShaderProgram, "lightViewTransform");
    int lightProjectionLocation = glGetUniformLocation(shadowMapShaderProgram, "lightProjectionTransform");
    int objectTransformLocation = glGetUniformLocation(shadowMapShaderProgram, "objectTransform");
    
    glUniformMatrix4fv(lightViewLocation, 1, GL_FALSE, glm::value_ptr(light->getViewTransform()));
    glUniformMatrix4fv(lightProjectionLocation, 1, GL_FALSE, glm::value_ptr(light->getProjectionTransform()));
    glUniformMatrix4fv(objectTransformLocation, 1, GL_FALSE, glm::value_ptr(obj->getTransform()));

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFrameBuffer);

    glDrawArrays(GL_TRIANGLES, 0, obj->numVertices());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::renderShadows(GameObject3DTextured* obj, GameObjectPointLightSource *light) {
    glUseProgram(cubeMapShaderProgram);
    glBindVertexArray(obj->vertexArrayObject);

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

    int objectTransformLocation = glGetUniformLocation(cubeMapShaderProgram, "objectTransform");
    int lightViewTransformsLocation = glGetUniformLocation(cubeMapShaderProgram, "lightViewTransforms");
    int lightProjectionTransformLocation = glGetUniformLocation(cubeMapShaderProgram, "lightProjectionTransform");
    int farPlaneLocation = glGetUniformLocation(cubeMapShaderProgram, "farPlane");

    glUniformMatrix4fv(objectTransformLocation, 1, GL_FALSE, glm::value_ptr(obj->getTransform()));
    glUniformMatrix4fv(lightViewTransformsLocation, 6, GL_FALSE, glm::value_ptr(light->getViewTransforms()[0]));
    glUniformMatrix4fv(lightProjectionTransformLocation, 1, GL_FALSE, glm::value_ptr(light->getProjectionTransform()));
    glUniform1f(farPlaneLocation, light->farPlane);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFrameBuffer);

    glDrawArrays(GL_TRIANGLES, 0, obj->numVertices());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int Renderer::renderTextToTexture(std::string _text, glm::ivec2 _size, glm::vec3 _textColor, glm::vec3 _backgroundColor) {

    glViewport(0, 0, _size.x, _size.y);
    glBindFramebuffer(GL_FRAMEBUFFER, textFramebuffer);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _size.x, _size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);

    glBindRenderbuffer(1, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _size.x, _size.y);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        throw std::runtime_error("failed to render text to texture");
    }
    
    // render the text
    
    glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec2 offset;
    offset.x = 0.f;
    offset.y = 0.5f;
    glm::vec2 scale;
    scale.x = 2.f / (float)_text.length();
    scale.y = 1.f;

    textColor = _textColor;

    glUseProgram(textShaderProgram);
    glBindVertexArray(textVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, textVertexBuffer);

    int colorLocation = glGetUniformLocation(textShaderProgram, "textColor");
    glUniform3fv(colorLocation, 1, glm::value_ptr(textColor));

    glActiveTexture(GL_TEXTURE0);
    int textureLocation = glGetUniformLocation(textShaderProgram, "glyphTexture");
    glUniform1i(textureLocation, 0);

    for (char c : _text) {
        Character ch = characters->at(c);

        float x = -1 + offset.x;
        float y = -1 + offset.y;
        float w = scale.x;
        float h = scale.y;

        (*textVertices)[0] = x;
        (*textVertices)[1] = y;

        (*textVertices)[4] = x;
        (*textVertices)[5] = y + h;

        (*textVertices)[8] = x + w;
        (*textVertices)[9] = y + h;

        (*textVertices)[12] = x + w;
        (*textVertices)[13] = y + h;

        (*textVertices)[16] = x + w;
        (*textVertices)[17] = y;

        (*textVertices)[20] = x;
        (*textVertices)[21] = y;
        
        offset.x += scale.x;
        offset.y += 0;

        glBindTexture(GL_TEXTURE_2D, ch.texture);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * textVertices->size(), textVertices->data());

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteRenderbuffers(1, &renderbuffer);

    return texture;

}

void Renderer::clearShadow() {
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFrameBuffer);
    glClear(GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFrameBuffer);
    glClear(GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}
