#version 450

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 inPosition;

layout(location	= 0) out vec3 outPosition;
layout(location	= 1) out vec3 outRayDir;

void main()
{
	gl_Position = projection * view * model * vec4(inPosition, 1.0);
	outPosition = inPosition;

	mat4 modelView = view * model;
	mat4 inverseModelView = inverse(modelView);

	vec3 viewPos = vec3(modelView * vec4(inPosition, 1.0));
	outRayDir = vec3(inverseModelView * vec4(viewPos, 0.0));
}
