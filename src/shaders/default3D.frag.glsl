#version 400 core

in vec2 frag_tex;
in vec3 frag_norm;

uniform sampler2D in_texture;

out vec4 frag_color;

void main() {
        frag_color = texture(in_texture, frag_tex);
        //frag_color = vec4(1.0, 0, 0, 1.0);
}
