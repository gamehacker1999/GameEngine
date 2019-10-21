#pragma once
#include "Component.h"
#include"Actor.h"
#include"Math.h"
class CameraComponent :
	public Component
{
public:
	CameraComponent(class Actor* owner);
	virtual ~CameraComponent();
protected:
	void SetViewMatrix(const Matrix4& view);
	void SetSkyBoxMatrix(const Matrix4& skyboxView);
};

