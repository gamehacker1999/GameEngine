#version 330

//Output color of the pixel
out vec4 outColor;

//recieving texture coordinates
in vec2 fragTextureCoordinates;

//Samples the 2D texture to get color code
//You dont't need to bind it in c++ since 
//opengl automatically knows it is the only texture
uniform sampler2D uTexture;

void main()
{
	outColor=texture(uTexture,fragTextureCoordinates);
}