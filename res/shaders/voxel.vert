
#version 430

layout(location = 0) in uint position;
//layout(location = 1) in vec3 texCoord;
//layout(location = 2) in vec3 lightData;

uniform mat4 proj, view, model; //transform matrix

out vec3 texCoordInterp;
out vec3 lightDataInterp;
out vec4 eyeSpacePos;

void main() {
	 vec3 pos_unpacked;
	 pos_unpacked.x = float((position >> 8)&((1<<4)-1))
	 pos_unpacked.y = float((position >> 4)&((1<<4)-1))
	 pos_unpacked.z = float((position >> 0)&((1<<4)-1))
	 
	vec4 eyeSpacePosVert = view * model * vec4(position, 1.0);
    gl_Position = proj * eyeSpacePosVert;
    //texCoordInterp = texCoord;
	texCoordInterp = vec3(0.0, 0.0, 0.0);
    lightDataInterp = lightData;
	eyeSpacePos = eyeSpacePosVert;
}