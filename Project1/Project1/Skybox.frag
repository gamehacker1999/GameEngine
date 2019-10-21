#version 450

in vec3 TexCoordinates;

out vec4 FragColor;

uniform samplerCube skybox;

void main()
{
	FragColor=texture(skybox,TexCoordinates);
}