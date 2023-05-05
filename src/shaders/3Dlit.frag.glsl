#version 400 core

struct vertOut {
    vec2 tex;
    vec4 worldspace;
    vec4 lightspace;
    vec3 normal;
};

in vertOut fragin;

out vec4 fragColor;

uniform sampler2D in_texture;
uniform sampler2D in_shadowMap;

uniform vec3 in_lightPos;
uniform vec3 in_lightColor;
uniform vec3 in_kAmbient;

float shadowCalc(vec4 pointLightSpace, float lightAngle) {
    vec3 proj = pointLightSpace.xyz / pointLightSpace.w;
    proj = proj * 0.5 + 0.5;
    float depthOnMap = texture(in_shadowMap, proj.xy).r;

    //float depthBias = 0.000;
    float depthBias = max(0.005 * lightAngle, 0);

    return proj.z > depthOnMap + depthBias ? 0.0 : 1.0;
}

void main() {

    vec3 ambient = 0.2 * in_lightColor * in_kAmbient;

    vec3 norm = normalize(fragin.normal);
    vec3 lightDir = normalize(in_lightPos - fragin.worldspace.xyz);
    float lightSimilarAngle = dot(norm, lightDir); // tells us how similar the lightDir and normal are
    float diffuse = max(lightSimilarAngle, 0.0);
    vec4 objectColor = texture(in_texture, fragin.tex);

    float shadow = shadowCalc(fragin.lightspace, 1.0 - lightSimilarAngle);

    vec3 lighting = ambient + (diffuse * shadow);
    objectColor.xyz = objectColor.xyz * lighting;
    fragColor = vec4(objectColor);
}
