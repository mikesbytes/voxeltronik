#version 150

in vec4 wSpacePos;

out vec4 outColor;

const vec4 horizonCol = vec4(0.46f, 0.8f, 0.99f, 1.0f);
const vec4 topCol = vec4(0.0f, 0.27f, 0.86f, 1.0f);

void main() {
	vec3 normalized = normalize(wSpacePos.xyz);
	outColor = mix(horizonCol, topCol, normalized.y);
}
