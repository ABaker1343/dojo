#version 400 core

in vec2 frag_texCoords;

out vec4 fragColor;

uniform sampler2D in_texture;

void main() {
    fragColor = texture(in_texture, frag_texCoords);
}
