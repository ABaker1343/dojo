#version 400 core

layout(location=0) in vec3 in_pos;
layout(location=1) in vec2 in_tex;
layout(location=2) in vec3 in_norm;

uniform mat4 in_worldTransform;
uniform mat4 in_cameraTransform;
uniform mat4 in_projectionTransform;

uniform mat4 in_lightTransform;
uniform mat4 in_lightProjection;

struct vertOut {
    vec2 tex;
    vec4 worldspace;
    vec4 lightspace;
    vec3 normal;
};

out vertOut fragin;

void main() {
    vec4 worldspace = in_worldTransform * vec4(in_pos, 1.0);
    vec4 position = in_projectionTransform * in_cameraTransform * worldspace;
    vec4 lightspace = in_lightProjection * in_lightTransform * worldspace;
    gl_Position = position;

    fragin.tex = in_tex;
    fragin.worldspace = worldspace;
    fragin.lightspace = lightspace;
    fragin.normal = in_norm;
}
