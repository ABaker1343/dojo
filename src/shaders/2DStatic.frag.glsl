#version 400 core

in vec2 frag_texCoords;

out vec4 fragColor;

uniform sampler2D in_texture;
uniform ivec2 in_flip;

void main() {
    vec2 texCoords = frag_texCoords * in_flip;
    fragColor = texture(in_texture, texCoords);
}
