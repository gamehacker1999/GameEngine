#version 330

//Uniform keyword is similar to static, it stays the same
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

//Vertex attributes, position and texture coordinates
layout(location=0)in vec3 inPosition;
layout(location=1)in vec2 inTextureCoordinates;

//Add texture coordinates as output
out vec2 fragTextureCoordinates;

void main()
{
	vec4 position = vec4(inPosition,1.0);
	gl_Position = position*uWorldTransform*uViewProj;

	fragTextureCoordinates=inTextureCoordinates;
}