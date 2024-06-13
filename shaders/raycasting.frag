#version 450

layout(binding = 0) uniform sampler3D volume;
layout(binding = 1) uniform sampler1D transferFunction;

uniform int renderMode;

layout(location = 0) in vec3 inEntryPos;
layout(location	= 1) in vec3 inRayDir;

layout(location = 0) out vec4 outColor;

const int MAX_STEPS = 1024;


vec3 raycastingMIP(vec3 entryPos, vec3 rayStep);
vec3 raycastingDRR(vec3 entryPos, vec3 rayStep);

void main()
{
	vec3 volumeSize = textureSize(volume, 0).xyz;
	// Currently the minimum stepSize is estimated by taking the smallest dimension of the volume
	// This could be improved by calculating the actual length needed to traverse one voxel for the ray direction
	float stepSize = 1.0 / min(min(volumeSize.x, volumeSize.y), volumeSize.z); 
	vec3 rayStep = normalize(inRayDir) * stepSize; 

	switch (renderMode)
	{
	case 0: // Entry points
		outColor = vec4(inEntryPos, 1.0);
		outColor = vec4(texture(transferFunction, inEntryPos.z).rrr + 0.1, 1.0);
		break;
	case 1: // Texture
		outColor = vec4(texture(volume, inEntryPos).rrr, 1.0);
		break;
	case 2: // Raycasting (MIP)
		outColor = vec4(raycastingMIP(inEntryPos, rayStep), 1.0);
		break;
	case 3: // Raycasting (DRR)
		outColor = vec4(raycastingDRR(inEntryPos, rayStep), 1.0);
		break;
	}
}

vec3 raycastingMIP(vec3 entryPos, vec3 rayStep)
{
	float value = 0.0;
	for (int i = 0; i < MAX_STEPS; i++)
	{
		vec3 samplePos = inEntryPos + (rayStep * float(i));
		if (samplePos.x < 0.0 || samplePos.x > 1.0 || samplePos.y < 0.0 || 
			samplePos.y > 1.0 || samplePos.z < 0.0 || samplePos.z > 1.0)
			break;

		value = max(value, texture(volume, samplePos).r);
	}

	return texture(transferFunction, value).rgb;
}

vec3 raycastingDRR(vec3 entryPos, vec3 rayStep)
{
	float value = 0.0;
	int i;
	for (i = 0; i < MAX_STEPS; i++)
	{
		vec3 samplePos = inEntryPos + (rayStep * float(i));
		if (samplePos.x < 0.0 || samplePos.x > 1.0 || samplePos.y < 0.0 || 
			samplePos.y > 1.0 || samplePos.z < 0.0 || samplePos.z > 1.0)
			break;

		value += texture(volume, samplePos).r;
	}

	value /= float(i);
	return texture(transferFunction, value).rgb;
}
