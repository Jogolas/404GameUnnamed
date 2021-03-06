#include "Utils.h"
#include "AABB.h"
#include <limits>

using namespace std;

//UP: vec3(0, 1, 0)
const vec3 Utils::up = vec3(0, 1, 0);
//UP: vec3(0, -1, 0)
const vec3 Utils::down = vec3(0, -1, 0);

//RIGHT: vec3(1, 0, 0)
const vec3 Utils::right = vec3(1, 0, 0);
//LEFT: vec3(-1, 0, 0)
const vec3 Utils::left = vec3(-1, 0, 0);

//FRONT: vec3(0, 0, 1)
const vec3 Utils::front = vec3(0, 0, 1);
//BACK: vec3(0, 0, -1)
const vec3 Utils::back = vec3(0, 0, -1);

float Utils::DistanceBetweenPoints(const vec3 & P, const vec3 & Q)
{
	return distance(P, Q);
}

float Utils::DistanceBetweenPointAndPlane(const vec3 & P, const vec3 & n, float d)
{
	return abs(dot(P, n) + d);
}

bool Utils::PointIsAbovePlane(const vec3 & P, const vec3 & n, float d)
{
	return dot(n, P) + d > FLT_EPSILON;
}

vec3 Utils::projectPointOnAABB(const vec3 & P, const AABB* const aabb)
{
	vec3 center = aabb->getCentre();
	vec3 radii = aabb->getRadius();

	char mask = Utils::computePointMask(P, aabb);
	vec3 point = P;

	if ((mask & 1) == 1)	// +x
	{
		point = projectPointOnPlane(point, Utils::right, center.x + radii.x);
		//cout << "X" << endl;
	}
	if ((mask & 2) == 2)	// -x
	{
		point = projectPointOnPlane(point, Utils::left, center.x - radii.x);
		//cout << "-X" << endl;
	}
	if ((mask & 4) == 4)	// +y
	{
		point = projectPointOnPlane(point, Utils::up, center.y + radii.y);
		//cout << "Y" << endl;
	}
	if ((mask & 8) == 8)	// -y
	{
		point = projectPointOnPlane(point, Utils::down, center.y - radii.y);
		//cout << "-Y" << endl;
	}
	if ((mask & 16) == 16) // +z
	{
		point = projectPointOnPlane(point, Utils::front, center.z + radii.z);
		//cout << "Z" << endl;
	}
	if ((mask & 32) == 32) // -z
	{
		point = projectPointOnPlane(point, Utils::back, center.z - radii.z);
		//cout << "-Z" << endl;
	}

	return point;
}

vec3 Utils::projectPointOnPlane(const vec3& P, const vec3& n, float c)
{
	float d = Utils::DistanceBetweenPointAndPlane(P, n, c);
	return P - d*n;
}

/*
Mask is computed as in the following ASCII image.

This is foe the 2D case with codes for +/-x and +/-y

0110	|	+y 0100   	|	0101
-----------------------------------
-x 0010	|	   0000		|	+x 0001
-----------------------------------
1010	|	-y 1000  	|	1001

the 3D case for +/-z is similar but using 6 bits instead of 4

*/

char Utils::computePointMask(const vec3& P, const AABB* aabb)
{
	vec3 center = aabb->getCentre();
	vec3 radii = aabb->getRadius();

	char mask = 0;
	if (PointIsAbovePlane(P, Utils::right, -center.x - radii.x))
	{
		//cout << "X" << endl;
		mask |= 1;// +x
	}
	if (PointIsAbovePlane(P, Utils::left, center.x - radii.x))
	{
		//cout << "X" << endl;
		mask |= 2;	// -x
	}
	if (PointIsAbovePlane(P, Utils::up, -center.y - radii.y))
	{
		//cout << "Y" << endl;
		mask |= 4;	// +y
	}
	if (PointIsAbovePlane(P, Utils::down, center.y - radii.y))
	{
		//cout << "-Y" << endl;
		mask |= 8;	// -y
	}
	if (PointIsAbovePlane(P, Utils::front, -center.z - radii.z))
	{
		//cout << "Z" << endl;
		mask |= 16;	// +z
	}
	if (PointIsAbovePlane(P, Utils::back, center.z - radii.z))
	{
		//cout << "-Z" << endl;
		mask |= 32;	// -z
	}

	return mask;
}