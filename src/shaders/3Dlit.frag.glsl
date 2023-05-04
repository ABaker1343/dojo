#version 400 core

in vec2 frag_tex;
in vec4 frag_lightspace;

out vec4 fragColor;

uniform sampler2D in_texture;
uniform sampler2D in_shadowMap;

float shadowCalc(vec4 pointLightSpace) {
    vec3 proj = pointLightSpace.xyz / pointLightSpace.w;
    proj = proj * 0.5 + 0.5;
    
    return pointLightSpace.z > texture(in_shadowMap, proj.xy).z ? 0.0 : 1.0;
}

void main() {

    float shadow = shadowCalc(frag_lightspace);

    fragColor = texture(in_texture, frag_tex) * shadow;

    vec3 proj = frag_lightspace.xyz / frag_lightspace.w;
    proj = proj * 0.5 + 0.5;
    float depthOnMap = texture(in_shadowMap, proj.xy).r;
    fragColor = vec4(proj.z, proj.z, proj.z, 1.0);
    //fragColor.w = 1.0;
}
