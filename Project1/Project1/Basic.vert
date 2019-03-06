#version 460

//Position of the vertex
in vec3 inPosition;

void main()
{
	//store vertex position output of shader
	gl_Position=vec4(inPosition,1.0);

}