#version 400 core

in vec2 frag_texcoords;

uniform sampler2D in_texture;

out vec4 fragColor;

void main() {
        fragColor = texture(in_texture, frag_texcoords);
    }
