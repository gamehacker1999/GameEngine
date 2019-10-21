#version 330

//vertex shader for a skinned model

//Uniform keyword is similar to static, it stays the same
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

//Vertex attributes, position and texture coordinates
layout(location=0)in vec3 inPosition;
layout(location=1)in vec3 inNormalCoordinates;
layout(location = 2) in uvec4 inSkinBones; //bone locations
layout(location = 3) in vec4 inSkinWeight; //influence of each bone on vertex
layout(location=4)in vec2 inTextureCoordinates;


//position of camera in world space
out vec3 fragPosition;
//Normal of vertices in world space
out vec3 fragNormalPosition;

//Add texture coordinates as output
out vec2 fragTextureCoordinates;

//storing the matrix pallete of the vertex
uniform mat4 uMatrixPalette[96];

void main()
{
   //converting into homogenous coordinates so that matrix multiplication works
	vec4 position = vec4(inPosition,1.0);

	//skin the position of the vertex
	vec4 skinnedPos = (position*uMatrixPalette[inSkinBones.x])*inSkinWeight.x;
	skinnedPos+=(position*uMatrixPalette[inSkinBones.y])*inSkinWeight.y;
	skinnedPos+=(position*uMatrixPalette[inSkinBones.z])*inSkinWeight.z;
	skinnedPos+=(position*uMatrixPalette[inSkinBones.w])*inSkinWeight.w;
	
	//finding the world position of the camera
	skinnedPos = skinnedPos*uWorldTransform;

	//Saving the world position
	fragPosition=skinnedPos.xyz;

	//transform to clip spac
	gl_Position = skinnedPos*uViewProj;

	vec4 skinnedNormal = vec4(inNormalCoordinates,0.0f);

	skinnedNormal=(skinnedNormal*uMatrixPalette[inSkinBones.x])*inSkinWeight.x
	+(skinnedNormal*uMatrixPalette[inSkinBones.y])*inSkinWeight.y
	+(skinnedNormal*uMatrixPalette[inSkinBones.z])*inSkinWeight.z
	+(skinnedNormal*uMatrixPalette[inSkinBones.w])*inSkinWeight.w;

	fragNormalPosition = (skinnedNormal*uWorldTransform).xyz;

	fragTextureCoordinates=inTextureCoordinates;
}