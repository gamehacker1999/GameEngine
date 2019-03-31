#version 330

//Output color of the pixel
out vec4 outColor;

//recieving texture coordinates
in vec2 fragTextureCoordinates;

//Samples the 2D texture to get color code
//You dont't need to bind it in c++ since 
//opengl automatically knows it is the only texture
uniform sampler2D uTexture;

struct DirectionalLight
{
	//Direction of light
	vec3 direction;

	//Diffuse color
	vec3 diffuseColor;

	//Specularity
	vec3 specularity;

};

//uniforms for lighting
uniform vec3 uCameraPos;

//Ambient light level
uniform vec3 ambientLight;

//specular power
uniform float specularPower;

//directional light
uniform DirectionalLight directionalLight;

//position of camera in world space
in vec3 fragPosition;
//Normal of vertices in world space
in vec3 fragNormalPosition;

void main()
{
	//surface normal, we have to normalize it since openGl interpolates the normal
	vec3 N = normalize(fragNormalPosition);
	//direction from light to surface
	vec3 L = normalize(-directionalLight.direction);
	//vector from surface to camera
	vec3 V = normalize(uCameraPos-fragPosition);
	//reflection of negative l along N
	vec3 R = normalize(reflect(-L,N));

	//computing phong reflection
	vec3 phong = ambientLight;
	float NdotL = dot(N,L);
	if(NdotL>0)
	{
	//calculating diffuse
		vec3 diffuse = directionalLight.diffuseColor*NdotL;
		//calculating specular color
		//if r dot v is negative, the max selects 0.0
		vec3 specular = directionalLight.specularity*
		pow(max(0.0,dot(R,V)),specularPower);

		phong+=diffuse+specular;
		
	}

	//final color is color of texture times phong
	outColor=texture(uTexture,fragTextureCoordinates)*vec4(phong,1.0);
}