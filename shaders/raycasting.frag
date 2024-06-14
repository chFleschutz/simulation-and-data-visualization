#version 450

layout(binding = 0) uniform sampler3D volume;
layout(binding = 1) uniform sampler1D transferFunction;

uniform int renderMode;

layout(location = 0) in vec3 inEntryPos;
layout(location	= 1) in vec3 inRayDir;

layout(location = 0) out vec4 outColor;

const int MAX_STEPS = 1024;


vec4 raycastingMIP(vec3 entryPos, vec3 rayStep);
vec4 raycastingDRR(vec3 entryPos, vec3 rayStep);
vec4 raycastingTFN(vec3 entryPos, vec3 rayStep);

void main()
{
	vec3 volumeSize = textureSize(volume, 0).xyz;
	// Currently the minimum stepSize is estimated by taking the smallest dimension of the volume
	// This could be improved by calculating the actual length needed to traverse one voxel for the ray direction
	float stepSize = 1.0 / min(min(volumeSize.x, volumeSize.y), volumeSize.z); 
	vec3 rayStep = normalize(inRayDir) * stepSize; 
	vec3 rayStart = inEntryPos + (rayStep * 0.5); // Start in the middle of the first voxel to avoid artifacts

	switch (renderMode)
	{
	case 0: // Entry points
		outColor = vec4(inEntryPos, 1.0);
		break;
	case 1: // Texture
		outColor = vec4(texture(volume, inEntryPos).rrr, 1.0);
		break;
	case 2: // Raycasting (MIP)
		outColor = raycastingMIP(rayStart, rayStep);
		break;
	case 3: // Raycasting (DRR)
		outColor = raycastingDRR(rayStart, rayStep);
		break;
	case 4: // Transfer function (TFN)
		outColor = raycastingTFN(rayStart, rayStep);
		break;
	}
}

vec4 raycastingMIP(vec3 entryPos, vec3 rayStep)
{
	float value = 0.0;
	for (int i = 0; i < MAX_STEPS; i++)
	{
		vec3 samplePos = entryPos + (rayStep * float(i));
		if (samplePos.x < 0.0 || samplePos.x > 1.0 || samplePos.y < 0.0 || 
			samplePos.y > 1.0 || samplePos.z < 0.0 || samplePos.z > 1.0)
			break;

		value = max(value, texture(volume, samplePos).r);
	}

	return vec4(value, value, value, 1.0);
}

vec4 raycastingDRR(vec3 entryPos, vec3 rayStep)
{
	float value = 0.0;
	int i;
	for (i = 0; i < MAX_STEPS; i++)
	{
		vec3 samplePos = entryPos + (rayStep * float(i));
		if (samplePos.x < 0.0 || samplePos.x > 1.0 || samplePos.y < 0.0 || 
			samplePos.y > 1.0 || samplePos.z < 0.0 || samplePos.z > 1.0)
			break;

		value += texture(volume, samplePos).r;
	}

	value /= float(i);
	return vec4(value, value, value, 1.0);
}

vec4 raycastingTFN(vec3 entryPos, vec3 rayStep)
{
	float value = 0.0;
	vec4 color = vec4(0.0);
	for (int i = 0; i < MAX_STEPS; i++)
	{
		vec3 samplePos = entryPos + (rayStep * float(i));
		if (samplePos.x < 0.0 || samplePos.x > 1.0 || samplePos.y < 0.0 || 
			samplePos.y > 1.0 || samplePos.z < 0.0 || samplePos.z > 1.0)
			break;

		float intensity = texture(volume, samplePos).r;
		vec4 transferColor = texture(transferFunction, intensity).rgba;

		color = color + (1.0 - color.a) * transferColor;

		if (color.a >= 0.99)
			break;
	}

	return color;
}
