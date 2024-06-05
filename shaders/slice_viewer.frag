#version 450

uniform sampler3D tex;

uniform float slice;
uniform float windowLevel;
uniform float windowWidth;
uniform bool enableWindowing;

layout(location = 0) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;

float windowRemap(float value, float level, float width)
{
	float lower = level - (0.5 * width);
	return clamp((value - lower) / width, 0.0, 1.0);
}

void main()
{
    vec3 coord = vec3(inTexCoord, slice);
    float value = texture(tex, coord).r;
 
    if (enableWindowing)
    {
        value = windowRemap(value, windowLevel, windowWidth);
    }

    outColor = vec4(value, value, value, 1.0);
}
