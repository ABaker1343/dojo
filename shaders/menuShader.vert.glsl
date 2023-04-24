#version 400 core

layout(location=0) in vec2 in_pos;

out vec2 frag_texpos;
void main() {
    gl_Position = vec4(in_pos, -1.0, 1.0);
    frag_texpos = in_pos;
}
