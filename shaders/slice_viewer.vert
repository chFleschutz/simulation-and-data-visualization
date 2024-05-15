#version 450

layout(location = 0) in vec3 position;

layout(location = 0) uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(position, 1.0);
}
