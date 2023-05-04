#version 400 core

in vec2 frag_tex;
in vec4 frag_lightspace;

out vec4 fragColor;

uniform sampler2D in_texture;
uniform sampler2D in_shadowMap;

float shadowCalc(vec4 pointLightSpace) {
    vec3 proj = pointLightSpace.xyz / pointLightSpace.w;
    proj = proj * 0.5 + 0.5;
    float depthOnMap = texture(in_shadowMap, proj.xy).r;

    float depthBias = 0.05;

    return proj.z > depthOnMap + depthBias ? 0.0 : 1.0;
}

void main() {
    fragColor = texture(in_texture, frag_tex) * shadowCalc(frag_lightspace);
    fragColor.w = 1.0;
}
