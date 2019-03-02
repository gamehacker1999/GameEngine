
#include "stdafx.h"
#include "Vector2.h"
#include <math.h>


Vector2::Vector2(float x,float y)
{
	this->vx = x;
	this->vy = y;
}


Vector2::~Vector2()
{
}

float Vector2::Dot(Vector2 &v1, Vector2&v2)
{
	float dotProduct = (v1.vx*v2.vx) + (v1.vy*v2.vy);

	return dotProduct;

}

float Vector2::Magnitude(Vector2 &v)
{
	float magnitude = sqrt((v.vx*v.vx)+(v.vy*v.vy));
	return magnitude;

}

float Vector2::SqMagnitude(Vector2 &v)
{
	float sqMagnitude = ((v.vx*v.vx) + (v.vy*v.vy));
	return sqMagnitude;

}

Vector2& Vector2::operator+(const Vector2 &v)
{
	vx += v.vx;
	vy += v.vy;

	return *this;
}

Vector2& Vector2::operator-(const Vector2 &v)
{
	vx -= v.vx;
	vy -= v.vy;

	return *this;
}

float Vector2::Distance(Vector2 &v1, Vector2 &v2)
{
	
	float dist = sqrt((powf((v1.vx-v1.vy),2))+(powf((v1.vy-v2.vy),2)));
	return dist;
}

void Vector2::Normalize()
{
	this->vx = (this->vx) / Vector2::Magnitude(*this);
	this->vy = (this->vy) / Vector2::Magnitude(*this);
}

Vector2 Vector2::Normalized()
{
	float x = (this->vx) / Vector2::Magnitude(*this);
	float y = (this->vy) / Vector2::Magnitude(*this);

	Vector2 vector2(x, y);

	return vector2;
}

Vector2& Vector2::operator*(float num)
{
	this->vy *= num;
	this->vx *= num;

	return *this;
}

Vector2& Vector2::operator/(float num)
{
	this->vx /= num;
	this->vy /= num;

	return *this;
}

Vector2& Vector2::operator=(Vector2 v)
{
	this->vx = v.vx;
	this->vy = v.vy;

	return *this;
}

Vector2& Vector2::operator+=(const Vector2 &v)
{
	this->vx += v.vx;
	this->vy += v.vy;
	return *this;
}

