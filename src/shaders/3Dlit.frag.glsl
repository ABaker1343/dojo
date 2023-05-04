#version 400 core

in vec2 frag_tex;
in vec3 frag_lightspace;

out vec4 fragColor;

uniform sampler2D in_texture;
uniform sampler2D in_shadowMap;

void main() {
    float shadow = frag_lightspace.z > texture(in_shadowMap, frag_lightspace.xy).r ? 1.0 : 0.0;

    fragColor = texture(in_texture, frag_tex) * shadow;
    fragColor.w = 1.0;
}
