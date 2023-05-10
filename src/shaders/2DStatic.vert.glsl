#version 400 core

layout(location=0) in vec3 in_position;
layout(location=1) in vec2 in_texCoords;
layout(location=2) in vec3 in_normal;

uniform mat4 in_worldTransform;
uniform mat4 in_cameraTransform;
uniform mat4 in_projectionTransform;

out vec2 frag_texCoords;

void main() {

    vec4 position = in_projectionTransform * in_cameraTransform * in_worldTransform * vec4(in_position, 1.0);

    gl_Position = position;

    frag_texCoords = in_texCoords;
}
