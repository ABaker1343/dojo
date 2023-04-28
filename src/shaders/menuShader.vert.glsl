#version 400 core

layout(location=0) in vec2 in_position;

uniform vec4 in_location;

out vec2 frag_texcoords;

void main() {
    vec2 pos = (in_position * in_location.zw * 2) + (in_location.xy * 2) - 1;
    gl_Position = vec4(pos, -1.0, 1.0);

    frag_texcoords = in_position;
    }
