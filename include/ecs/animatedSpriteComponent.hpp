#ifndef __HEADER_ANIMATED_SPRITE_COMPONENT
#define __HEADER_ANIMATED_SPRITE_COMPONENT

#include <map>

#include "../gameObjects/texture.hpp"
#include "../window/renderInfo.hpp"
#include "component.hpp"

namespace dojo {

typedef struct {
    std::string name;
    Texture* texture;
    int numFrames;
    int currentFrame;
    int repeatFrames;
    int textureRows;
} Animation;

class AnimatedSpriteComponent : public Component {
    public:
        AnimatedSpriteComponent(const std::string& _texturePath, int _textureRows, int _numFrames, const std::string& _name = "default");
        ~AnimatedSpriteComponent();

        void addAnimation(const std::string& _name, const std::string& _animationPath, int _textureRows, int _numFrames);
        bool setAnimation(const std::string& _name);

        bool nextFrame();
        void resetAnimation();
        glm::vec2 getFlip();
        void flipx();
        void flipy();

        int getNumFrames();
        int getCurrentFrameNum();
        int getCurrentAnimationRows();
        std::string currentAnimation();

        Texture* getTexture();
        RenderInfo m_renderInfo;

    protected:
        std::map<std::string, Animation> m_Animations;
        Animation* m_CurrentAnimation;
        float m_TimeSinceLastAnimation;
        glm::ivec2 m_flip;
};

}

#endif
