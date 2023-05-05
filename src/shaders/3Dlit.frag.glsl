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
uniform vec3 in_kDiffuse;
uniform vec3 in_kSpecular;
uniform float in_specExponent;
uniform float in_opacity;

uniform vec3 in_cameraPos;

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
    vec3 diffuse = max(lightSimilarAngle, 0.0) * in_kDiffuse;

    vec3 viewDir = normalize(in_cameraPos - fragin.worldspace.xyz);
    vec3 reflectDir = reflect(-lightDir, norm); // here lightDir is from the point to the light so we reverse it
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), in_specExponent) * in_kSpecular;

    vec4 objectColor = texture(in_texture, fragin.tex);

    float shadow = shadowCalc(fragin.lightspace, 1.0 - lightSimilarAngle);

    vec3 lighting = ambient + ((diffuse + specular) * shadow);
    fragColor = vec4(objectColor.xyz * lighting, in_opacity);
}
