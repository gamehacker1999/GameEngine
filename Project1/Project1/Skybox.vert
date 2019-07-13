#version 450

in vec3 position;

out vec3 TexCoordinates;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 uViewProj;
uniform mat4 rot;

void main()
{
	TexCoordinates=position;
	gl_Position=vec4(position*50,1.0)*rot*uViewProj;
}