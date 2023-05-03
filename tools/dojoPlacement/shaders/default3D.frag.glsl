#version 400 core

in vec2 frag_tex;
in vec3 frag_norm;

uniform sampler2D in_texture;
uniform vec3 in_ambient;

out vec4 frag_color;

void main() {
        if (frag_tex.x > 1 || frag_tex.y > 1) {
                frag_color = vec4(0, 0, 0, 1);
        } else {
            frag_color = texture(in_texture, frag_tex);
        }
        //frag_color = vec4(frag_norm, 1.0);
        //frag_color = vec4(1.0, 0, 0, 1.0);
}
