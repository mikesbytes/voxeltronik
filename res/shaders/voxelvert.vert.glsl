#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 texCoord;

uniform mat4 proj, view, model; //transform matrix

out vec3 texCoordInterp;
out vec3 lightDataInterp;
out vec4 eyeSpacePos;

void main() {
    vec4 eyeSpacePosVert = view * model * vec4(position, 1.0);
    gl_Position = proj * eyeSpacePosVert;
    texCoordInterp = texCoord;
    lightDataInterp = vec3(1.0f, 1.0f, 1.0f);
    eyeSpacePos = eyeSpacePosVert;
}
