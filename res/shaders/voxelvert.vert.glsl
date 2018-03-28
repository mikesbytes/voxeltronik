#version 330

layout(location = 0) in uint position;
//layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 texCoord;
layout(location = 1) in uint face_attrib;

uniform mat4 proj, view, model; //transform matrix

out vec3 texCoordInterp;
out vec3 lightDataInterp;
out vec4 eyeSpacePos;

void main() {
	 vec3 pos_unpacked;
	 pos_unpacked.x = float((position >> 8u) & 15u) / 15.0f;
	 pos_unpacked.y = float((position >> 4u) & 15u) / 15.0f;
	 pos_unpacked.z = float((position) & 15u) / 15.0f;

	 vec2 uv_unpacked;
	 uv_unpacked.x = float((position >> 16u) & 15u) / 15.0f;
	 uv_unpacked.y = float((position >> 12u) & 15u) / 15.0f;

	 vec3 offset_unpacked;
	 offset_unpacked.x = float((face_attrib >> 10u) & 31u);
	 offset_unpacked.y = float((face_attrib >> 5u) & 31u);
	 offset_unpacked.z = float((face_attrib) & 31u);

	 vec3 final_position = pos_unpacked + offset_unpacked;
    vec4 eyeSpacePosVert = view * model * vec4(final_position, 1.0);
    gl_Position = proj * eyeSpacePosVert;
    texCoordInterp = vec3(uv_unpacked, 0.0f);//texCoord;
    lightDataInterp = vec3(1.0f, 1.0f, 1.0f);
    eyeSpacePos = eyeSpacePosVert;
}
