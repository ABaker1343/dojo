#version 400 core

layout(location=0) in vec3 in_pos;
layout(location=1) in vec2 in_tex;
layout(location=2) in vec3 in_norm;

uniform mat4 in_worldTransform;
uniform mat4 in_cameraTransform;
uniform mat4 in_projectionTransform;

out vec2 frag_tex;
out vec3 frag_norm;

void main() {
    vec4 position = in_projectionTransform * in_cameraTransform * in_worldTransform * vec4(in_pos, 1);
    gl_Position = position;

    frag_tex = in_tex;
    frag_norm = in_norm;
}
