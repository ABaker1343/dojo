#version 400

in vec2 texPos;
out vec4 color;

uniform sampler2D glyphTexture;
uniform vec3 textColor;

void main () {
    vec4 sampled = vec4(textColor, texture(glyphTexture, texPos).r);
    color = vec4(textColor , 1.0) * sampled;
    //color = vec4(textColor, 1.0);
}
