#version 450

uniform sampler2D exitPoints;
uniform sampler3D volume;

uniform vec2 screenSize;
uniform int renderMode;
uniform vec3 volumeSize;
uniform bool singlePass;

layout(location = 0) in vec3 inPosition;
layout(location	= 1) in vec3 inRayDir;

layout(location = 0) out vec4 outColor;


const int MAX_STEPS = 1024;
const int MAX_SIZE = 128;

void main()
{
	switch (renderMode)
	{
	case 0: // Entry points
		outColor = vec4(inPosition, 1.0);
		break;
	case 1: // Exit points
		outColor = texture(exitPoints, gl_FragCoord.xy / screenSize);
		break;
	case 2: // Texture
		outColor = vec4(texture(volume, inPosition).rrr, 1.0);
		break;
	case 3: // Raycasting (MIP)
	case 4: // Raycasting (DRR)
		vec3 entryPos = inPosition;
		vec3 rayStep;

		if (!singlePass)
		{
			vec3 exitPos = texture(exitPoints, gl_FragCoord.xy / screenSize).xyz;
			vec3 ray = exitPos - entryPos;
			float rayLength = length(ray);
			vec3 rayDir = normalize(ray);
			rayStep = rayDir * length(ray / volumeSize);
		}
		else
		{
			vec3 volumeSize = textureSize(volume, 0).xyz;
			// Currently the minimum stepSize is estimated by taking the smallest dimension of the volume
			// This could be improved by calculating the actual length needed to traverse one voxel for the ray direction
			float stepSize = 1.0 / min(min(volumeSize.x, volumeSize.y), volumeSize.z); 
			rayStep = normalize(inRayDir) * stepSize; 
		}

		vec3 color = vec3(0.0);
		int i;
		for (i = 0; i < MAX_STEPS; i++)
		{
			vec3 samplePos = entryPos + (rayStep * float(i));

			if (samplePos.x < 0.0 || samplePos.x > 1.0 || samplePos.y < 0.0 || 
				samplePos.y > 1.0 || samplePos.z < 0.0 || samplePos.z > 1.0)
				break;

			float value = texture(volume, samplePos).r;

			if (renderMode == 3) // MIP
				color = vec3(max(color.r, value));

			if (renderMode == 4) // DRR
				color += vec3(value);
		}

		if (renderMode == 4) // DRR
			color /= float(i);

		outColor = vec4(color, 1.0);
		break;
	}
}
