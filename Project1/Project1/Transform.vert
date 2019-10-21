#version 460

//Uniform keyword is similar to static, it stays the same
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

in vec3 inPosition;

void main()
{
	vec4 position = vec4(inPosition,1.0);
	gl_Position = position*uWorldTransform*uViewProj;

}