#version 330

//Uniform keyword is similar to static, it stays the same
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

//Vertex attributes, position and texture coordinates
layout(location=0)in vec3 inPosition;
layout(location=1)in vec3 inNormalCoordinates;
layout(location=2)in vec2 inTextureCoordinates;

//position of camera in world space
out vec3 fragPosition;
//Normal of vertices in world space
out vec3 fragNormalPosition;

//Add texture coordinates as output
out vec2 fragTextureCoordinates;

void main()
{
   //converting into homogenous coordinates so that matrix multiplication works
	vec4 position = vec4(inPosition,1.0);
	
	//finding the world position of the camera
	position = position*uWorldTransform;

	//Saving the world position
	fragPosition=position.xyz;

	//transform to clip spac
	gl_Position = position*uViewProj;

	//transform normal into world space
     fragNormalPosition = (vec4(inNormalCoordinates,0.0f)*uWorldTransform).xyz;

	fragTextureCoordinates=inTextureCoordinates;
}