#include "implicitShape.h"

#include <math.h>


/*** Public Member Functions ***/

ImplicitShape::ImplicitShape()
{
	c200 = 0.0;
	c020 = 0.0;
	c002 = 0.0;
	
	c110 = 0.0;
	c101 = 0.0;
	c011 = 0.0;
	
	c100 = 0.0;
	c010 = 0.0;
	c001 = 0.0;
	
	c000 = 0.0;
	
	color = RGB(0.5, 0.5, 0.5);
	reflectionCoefficient = 0.25;
	refractionCoeffieient = 0.25;
	refractiveIndex = 1.5;
	phongExponent = 6;
}

ImplicitShape::ImplicitShape(
			float _c200, float _c020, float _c002,
			float _c110, float _c101, float _c011,
			float _c100, float _c010, float _c001,
			float _c000)
{
	c200 = _c200;
	c020 = _c020;
	c002 = _c002;
	
	c110 = _c110;
	c101 = _c101;
	c011 = _c011;
	
	c100 = _c100;
	c010 = _c010;
	c001 = _c001;
	
	c000 = _c000;
	
	color = RGB(0.5, 0.5, 0.5);
	reflectionCoefficient = 0.25;
	refractionCoeffieient = 0.25;
	refractiveIndex = 1.5;
	phongExponent = 6;
}

ImplicitShape::~ImplicitShape()
{
}

FCoord3D ImplicitShape::getNormal(FCoord3D point)
{
	FCoord3D n = FCoord3D(
		(2.0 * c200 * point.x) + (c110 * point.y) + (c101 * point.z) + c100,
		(2.0 * c020 * point.y) + (c110 * point.x) + (c011 * point.z) + c010,
		(2.0 * c002 * point.z) + (c101 * point.x) + (c011 * point.y) + c001
	);
	return n.makeUnit();
}

void ImplicitShape::setCoefficients(
	float _c200, float _c020, float _c002,
	float _c110, float _c101, float _c011,
	float _c100, float _c010, float _c001,
	float _c000
)
{
	c200 = _c200;
	c020 = _c020;
	c002 = _c002;
	
	c110 = _c110;
	c101 = _c101;
	c011 = _c011;
	
	c100 = _c100;
	c010 = _c010;
	c001 = _c001;
	
	c000 = _c000;
}


/** Implemented for Shape **/

bool ImplicitShape::rayIntersects(FCoord3D p0, FCoord3D d, float &t, FCoord3D &normal)
{
	float a =
		c200 * pow(d.x, 2.0) +
		c020 * pow(d.y, 2.0) +
		c002 * pow(d.z, 2.0) +
		c110 * d.x * d.y +
		c101 * d.x * d.z +
		c011 * d.y * d.z;
	
	float b =
		c200 * 2 * p0.x * d.x +
		c020 * 2 * p0.y * d.y +
		c002 * 2 * p0.z * d.z +
		c110 * (p0.x * d.y + p0.y * d.x) +
		c101 * (p0.x * d.z + p0.z * d.x) +
		c011 * (p0.y * d.z + p0.z * d.y) +
		c100 * d.x +
		c010 * d.y +
		c001 * d.z;
	
	float c =
		c200 * pow(p0.x, 2.0) +
		c020 * pow(p0.y, 2.0) +
		c002 * pow(p0.z, 2.0) +
		c110 * p0.x * p0.y +
		c101 * p0.x * p0.z +
		c011 * p0.y * p0.z +
		c100 * p0.x +
		c010 * p0.y +
		c001 * p0.z +
		c000;
	
	float discriminant = pow(b, 2.0) - (4.0 * a * c);
	if (discriminant < 0.0)
	{ // No solutions.
		return false;
	}
	else if (discriminant == 0.0)
	{ // One solution.
		float t1 = -b / (2 * a);
		if (t1 > 0)
		{
			t = t1;
		}
		else
		{
			return false;
		}
	}
	else
	{ // Two solutions. Solution we want is lower t.
		float t1 = (-b - sqrt(pow(b, 2.0) - (4.0 * a * c))) / (2.0 * a);
		float t2 = (-b + sqrt(pow(b, 2.0) - (4.0 * a * c))) / (2.0 * a);
		
		if (t1 > 0)
		{
			t = t1;
		}
		else if (t2 > 0)
		{
			t = t2;
		}
		else
		{
			return false;
		}
	}
	
	normal = getNormal(p0.plus(d.multiply(t)));
	return true;
}

bool ImplicitShape::lineSegmentIntersects(FCoord3D p0, FCoord3D p1)
{
	FCoord3D dir = p1.minus(p0).makeUnit();
	
	float dummyT;
	FCoord3D dummyNorm;
	return (rayIntersects(p0, dir, dummyT, dummyNorm) && rayIntersects(p1, dir.negate(), dummyT, dummyNorm));
}

void ImplicitShape::read(std::istream& s)
{
	readAttributes(s);
	
	s >> c200
		>> c020
		>> c020
		>> c002
		>> c110
		>> c101
		>> c011
		>> c100
		>> c010
		>> c001
		>> c000; 
}

void ImplicitShape::write(std::ostream& s)
{
	s << "IMPLICIT_SHAPE" << std::endl;
	writeAttributes(s);
	
	s << c200 << " "
		<< c020 << " "
		<< c020 << " "
		<< c002 << " "
		<< c110 << " "
		<< c101 << " "
		<< c011 << " "
		<< c100 << " "
		<< c010 << " "
		<< c001 << " "
		<< c000;
	s << std::endl;
}
