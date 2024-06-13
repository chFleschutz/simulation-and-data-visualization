#version 450

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 inPosition;

layout(location	= 0) out vec3 outPosition;
layout(location	= 1) out vec3 outRayDir;

void main()
{
	mat4 modelView = view * model;
	mat4 inverseModelView = inverse(modelView);
	vec4 viewPos = modelView * vec4(inPosition, 1.0);
	
	gl_Position = projection * viewPos;
	outPosition = inPosition;
	outRayDir = vec3(inverseModelView * vec4(viewPos.xyz, 0.0)); // Interpret view pos as a direction (w = 0)
}
