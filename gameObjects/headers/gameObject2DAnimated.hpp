#ifndef __HEADER_GAME_OBJECT_2D_ANIMATED
#define __HEADER_GAME_OBJECT_2D_ANIMATED

#include <map>

#include "gameObject2D.hpp"
#include "renderable.hpp"
#include "texture.hpp"

namespace dojo {

typedef struct {
    Texture* texture;
    int numFrames;
    int currentFrame;
    int repeatFrames;
    int textureRows;
} Animation;

class GameObject2DAnimated : public GameObject2D {
    public:
        GameObject2DAnimated(const std::string& _texturePath, int _textureRows, int _numFrames, const std::string& _name = "default");
        ~GameObject2DAnimated();

        void addAnimation(const std::string& _name, const std::string& _animationPath, int _textureRows, int _numFrames);
        bool setAnimation(const std::string& _name);

        bool nextFrame();
        void resetAnimation();

        int getNumFrames();
        int getCurrentFrameNum();
        int getCurrentAnimationRows();
        Texture* getTexture();

    protected:
        std::map<std::string, Animation> m_Animations;
        Animation* m_CurrentAnimation;
};

}

#endif
