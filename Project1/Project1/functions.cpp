#include"functions.h"

bool CircleIntersect(CircleComponent& a, CircleComponent&b)
{
	Vector3 diff = a.GetCenter()-b.GetCenter();
	float distSq = diff.LengthSq();

	float radiiSq = a.GetRadius() + b.GetRadius();
	radiiSq *= radiiSq;

	return radiiSq <= distSq;
}