#version 450

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = vec4(inPosition, 0.0);
}
