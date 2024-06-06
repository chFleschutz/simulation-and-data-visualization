#version 450

uniform sampler2D exitPoints;
uniform sampler3D volume;

uniform vec2 screenSize;
uniform int renderMode;
uniform vec3 volumeSize;

layout(location = 0) in vec3 inPosition;

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
		vec3 exitPos = texture(exitPoints, gl_FragCoord.xy / screenSize).xyz;
		vec3 ray = exitPos - entryPos;
		vec3 rayDir = normalize(ray);
		float rayLength = length(ray);
		vec3 rayStep = rayDir * length(ray / volumeSize);
		vec3 color = vec3(0.0);

		int i;
		for (i = 0; i < MAX_STEPS; i++)
		{
			vec3 currentRay = rayStep * float(i);
			if (length(currentRay) > rayLength)
				break;

			vec3 samplePos = entryPos + currentRay;
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
