#version 150

in vec3 position;

out vec4 wSpacePos;

uniform mat4 view, proj;


void main() {
	wSpacePos = vec4(position, 1.0f);
	gl_Position = proj * view *  vec4(position, 1.0);
}
