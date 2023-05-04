#version 400 core

layout(location=0) in vec3 in_pos;
layout(location=1) in vec2 in_tex;
layout(location=2) in vec3 in_normal;

uniform mat4 in_worldTransform;
uniform mat4 in_lightTransform;
uniform mat4 in_projectionTransform;

void main() {
    vec4 position = in_projectionTransform * in_lightTransform * in_worldTransform * vec4(in_pos, 1.0);
    gl_Position = position;
}

