#version 440
#extension GL_EXT_texture_array : enable

in vec3 texCoordInterp;
in vec3 lightDataInterp;

out vec4 outColor;

uniform sampler2DArray texture;

void main() {
    vec4 texel = texture2DArray(texture, texCoordInterp);
    outColor = texel * vec4(lightDataInterp, 1.0f);
}
