#version 400 core

in vec2 frag_texcoords;

out vec4 fragColor;

uniform sampler2D in_text;
uniform vec3 in_color;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(in_text, frag_texcoords).r);
    fragColor = vec4(in_color, 1.0) * sampled;
    fragColor = sampled;
    //fragColor = vec4(in_color, 1.0);
}

