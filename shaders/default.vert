#version 400 core

layout(location=0) in vec3 in_position;
layout(location=1) in vec2 in_texCoords;

uniform mat4 in_worldTransform;

out vec2 frag_texCoords;

void main() {

    vec4 position = in_worldTransform * vec4(in_position, 1.0);

    gl_Position = position;

    frag_texCoords = in_texCoords;
}
