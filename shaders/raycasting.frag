#version 450

layout(binding = 0) uniform sampler3D volume;
layout(binding = 1) uniform sampler1D transferFunction;

uniform int renderMode;

layout(location = 0) in vec3 inEntryPos;
layout(location	= 1) in vec3 inRayDir;

layout(location = 0) out vec4 outColor;

const int MAX_STEPS = 1024;
const vec3 LIGHT_DIR = normalize(vec3(1.0, 1.0, 1.0));

vec4 raycastingMIP(vec3 entryPos, vec3 rayStep);
vec4 raycastingDRR(vec3 entryPos, vec3 rayStep);
vec4 raycastingTFN(vec3 entryPos, vec3 rayStep);
vec4 raycastingPhong(vec3 entryPos, vec3 rayStep);

vec3 gradient(vec3 pos, vec3 voxelSize);
vec3 phong(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 color);

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
	case 5: // Phong shading
		outColor = raycastingPhong(rayStart, rayStep);
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

		color += (1.0 - color.a) * transferColor;
		if (color.a >= 0.99)
			break;
	}

	return color;
}

vec4 raycastingPhong(vec3 entryPos, vec3 rayStep)
{
	vec3 voxelSize = vec3(1.0) / textureSize(volume, 0).xyz;
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

		// Phong shading
		vec3 normal = normalize(gradient(samplePos, voxelSize));
		vec3 phongColor = phong(normal, LIGHT_DIR, -normalize(samplePos), transferColor.rgb);

		color.rgb += (1.0 - color.a) * phongColor * transferColor.a;
		color.a += (1.0 - color.a) * transferColor.a;
		if (color.a >= 0.99)
			break;
	}

	return color;
}

vec3 gradient(vec3 pos, vec3 voxelSize)
{
	vec3 gradient = vec3(0.0);
	gradient.x = texture(volume, pos + vec3(voxelSize.x, 0.0, 0.0)).r - texture(volume, pos - vec3(voxelSize.x, 0.0, 0.0)).r;
	gradient.y = texture(volume, pos + vec3(0.0, voxelSize.y, 0.0)).r - texture(volume, pos - vec3(0.0, voxelSize.y, 0.0)).r;
	gradient.z = texture(volume, pos + vec3(0.0, 0.0, voxelSize.z)).r - texture(volume, pos - vec3(0.0, 0.0, voxelSize.z)).r;
	return gradient;
}

vec3 phong(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 materialColor)
{
	// Light properties
	vec3 ia = vec3(1.0, 1.0, 1.0);
	vec3 id = vec3(1.0, 1.0, 1.0);
	vec3 is = vec3(1.0, 1.0, 1.0);

	// Material properties
	vec3 ka = materialColor;
	vec3 kd = materialColor;
	vec3 ks = vec3(1.0, 1.0, 1.0);
	float shininess = 32.0;

	// Ambient
	vec3 ambient = ka * ia;

	// Diffuse
	vec3 diffuse = kd * id * max(dot(normal, lightDir), 0.0);

	// Specular
	vec3 reflection = reflect(-lightDir, normal);
	vec3 specular = ks * is * pow(max(dot(reflection, viewDir), 0.0), shininess);

	return ambient + diffuse + specular;
}
