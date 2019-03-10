#pragma once
class Vector2
{
	public:

		float vx;
		float vy;
		static Vector2 zero;
		

		Vector2(float x=0, float y=0);
		~Vector2();
		static float Dot(Vector2 &v1, Vector2&v2);
		static float Magnitude(Vector2 &v);
		static float SqMagnitude(Vector2 &v);
		static float Distance(Vector2 &v1, Vector2 &v2);
		Vector2& operator +(const Vector2 &v);
		Vector2& operator -(const Vector2 &v);
		Vector2& operator *(const float num);
		Vector2& operator /(const float num);
		Vector2& operator*=(const float num);
		Vector2& operator+=(const Vector2 &v);
		Vector2& operator =(const Vector2 v);
		void Normalize();
		Vector2 Normalized();
		Vector2 Transform(const Vector2& vec, const float mat[3][3], float w= 1.0f);
		

};

