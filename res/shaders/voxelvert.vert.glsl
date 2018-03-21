#version 330

layout(location = 0) in uint position;
//layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 texCoord;

uniform mat4 proj, view, model; //transform matrix

out vec3 texCoordInterp;
out vec3 lightDataInterp;
out vec4 eyeSpacePos;

void main() {
	 vec3 pos_unpacked;
	 pos_unpacked.x = float((position ) & 15u) / 15.0f;
	 pos_unpacked.y = float((position >> 4u) & 15u) / 15.0f;
	 pos_unpacked.z = float((position >> 8u) & 15u) / 15.0f;
    vec4 eyeSpacePosVert = view * model * vec4(pos_unpacked, 1.0);
    gl_Position = proj * eyeSpacePosVert;
    texCoordInterp = vec3(1.0f, 1.0f, 1.0f);//texCoord;
    lightDataInterp = vec3(1.0f, 1.0f, 1.0f);
    eyeSpacePos = eyeSpacePosVert;
}
