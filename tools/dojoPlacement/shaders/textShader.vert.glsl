#version 400 core

layout(location=0) in vec4 vertex; //pos + tex

uniform mat4 in_projection;
uniform mat4 in_cameraTransform;

out vec2 frag_texcoords;

void main() {
    gl_Position = in_cameraTransform * in_projection * vec4(vertex.xy, 0.0, 1.0);
    frag_texcoords = vertex.zw;
}

