#version 450

uniform sampler2D exitPoints;

uniform vec2 screenSize;

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = texture(exitPoints, gl_FragCoord.xy / screenSize);
}
