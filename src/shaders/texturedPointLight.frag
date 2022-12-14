#version 400

out vec4 fragColor;

in VS_OUT {
    vec3 normal;
    vec3 fragPos;
    vec2 texPos;
} fs_in;

uniform sampler2D inTexture;
uniform samplerCube inDepthMap;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambientIntensity;

uniform float farPlane;

uniform int flip;

float ShadowCalculation(vec3 fragPos) {
    vec3 fragToLight = fragPos - lightPos;
    float closestDepth = texture(inDepthMap, fragToLight).r;
    
    closestDepth *= farPlane;
    float currentDepth = length(fragToLight);

    float bias = 0.05;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main() {

    vec3 ambientLight = ambientIntensity * lightColor;

    vec3 normal = normalize(fs_in.normal);
    vec3 lightDir = normalize(lightPos - fs_in.fragPos);

    float diffuse = max(dot(normal, lightDir), 0.0);
    vec3 diffusionLight = diffuse * lightColor;

    float shadow = ShadowCalculation(fs_in.fragPos);

    vec3 lighting = ambientLight + diffusionLight * (1 - shadow);

    fragColor = texture(inTexture, fs_in.texPos) * vec4(lighting, 1.0);
}
