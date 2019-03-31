#pragma once
#include"Math.h"
struct DirectionalLight
{
public:
	DirectionalLight();
	~DirectionalLight();
	//direction of light
	Vector3 direction;
	//difuse color
	Vector3 diffuseColor;
	//specularity
	Vector3 speculariy;
};

