#version 400 core

in vec2 frag_texCoords;

out vec4 fragColor;

uniform sampler2D in_texture;

struct AnimationData {
    int numFrames;
    int currentFrame;
    int rows;
};

uniform AnimationData in_animationData;

void main() {
    // calculate the size of the chunk

    float animationChunkY = 1.0/in_animationData.rows;
    float animationChunkX = 1.0/(in_animationData.numFrames/in_animationData.rows);

    // convert any 1.0 texCoords to be only 1 chunk in size
    vec2 texCoords;
    texCoords.x = frag_texCoords.x * animationChunkX;
    texCoords.y = frag_texCoords.y * animationChunkY;

    // move the coords to the chunk they should be rendering
    texCoords.x = texCoords.x + (animationChunkX * in_animationData.currentFrame);

    fragColor = texture(in_texture, texCoords);
}
