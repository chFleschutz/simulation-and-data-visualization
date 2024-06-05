#version 450

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 inPosition;

layout(location	= 0) out vec3 outPosition;

void main()
{
	gl_Position = projection * view * model * vec4(inPosition, 1.0);
	outPosition = inPosition;
}
