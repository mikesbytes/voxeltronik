#version 330
#extension GL_EXT_texture_array : enable

in vec3 texCoordInterp;
in vec3 lightDataInterp;
in vec4 eyeSpacePos;

out vec4 outColor;

uniform sampler2DArray texture;

const vec4 fogColor = vec4(0.46f, 0.8f, 0.99f, 1.0f);
const float fogDensity = 0.005f;

float getFogFactor(float fogCoord) {
	
	//get factor
	float fResult = 0.0f;
	fResult = exp(-fogDensity*fogCoord);
	fResult = 1.0-clamp(fResult, 0.0, 1.0);
	return fResult;
}

void main() {
	//fog calculations

    vec4 texel = texture2DArray(texture, texCoordInterp);
    outColor = texel * vec4(lightDataInterp, 1.0f);

	float fogCoord = abs(eyeSpacePos.z/eyeSpacePos.w);
	outColor = mix(outColor, fogColor, getFogFactor(fogCoord));
}
