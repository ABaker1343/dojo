#version 400 core

layout(location=0) in vec3 in_pos;
layout(location=1) in vec2 in_tex;
layout(location=2) in vec3 in_norm;

uniform mat4 in_worldTransform;
uniform mat4 in_cameraTransform;
uniform mat4 in_projectionTransform;

uniform mat4 in_lightTransform;
uniform mat4 in_lightProjection;

out vec2 frag_tex;
out vec4 frag_lightspace;

void main() {
    vec4 position = in_projectionTransform * in_cameraTransform * in_worldTransform * vec4(in_pos, 1.0);
    vec4 lightSpace = in_lightProjection * in_lightTransform * in_worldTransform * vec4(in_pos, 1.0);
    gl_Position = position;
    
    frag_tex = in_tex;
    frag_lightspace = lightSpace;
    }
