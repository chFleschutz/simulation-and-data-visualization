#version 450

uniform sampler2D exitPoints;
uniform sampler3D volume;

uniform vec2 screenSize;
uniform int renderMode;

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 outColor;

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
	case 2: // texture
		outColor = vec4(texture(volume, inPosition + vec3(0.5)).rrr, 1.0);
		break;
	case 3: // Raycasting
		outColor = vec4(1.0, 0.0, 0.0, 0.0);
		break;
	}
}
