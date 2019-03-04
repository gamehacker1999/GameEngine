#include"functions.h"

bool CircleIntersect(CircleComponent& a, CircleComponent&b)
{
	Vector2 diff = a.GetCenter()-b.GetCenter();
	float distSq = Vector2::SqMagnitude(diff);

	float radiiSq = a.GetRadius() + b.GetRadius();
	radiiSq *= radiiSq;

	return radiiSq <= distSq;
}