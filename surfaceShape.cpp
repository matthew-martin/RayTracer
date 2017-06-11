#include "surfaceShape.h"

#include <assert.h>
#include <math.h>

#include "shape.h"


/*** Public Member Functions ***/

SurfaceShape::SurfaceShape()
{
	points = new std::vector<FCoord3D>();
	surfaceIndices = new std::vector<SurfaceIndices>();
	
	color = RGB(0.5, 0.5, 0.5);
	reflectionCoefficient = 0.25;
	refractionCoeffieient = 0.25;
	refractiveIndex = 1.5;
	phongExponent = 3;
}

SurfaceShape::~SurfaceShape()
{
	delete points;
	delete surfaceIndices;
}


/** Implemented for Shape **/

bool SurfaceShape::rayIntersects(FCoord3D p0, FCoord3D d, float &t, FCoord3D &normal)
{
	float lowestT = 0.0;
	int lowestTSurface = -1;
	float currT = 0.0;
	bool intersected = false;
	for (int i = 0; i < numSurfaces(); i++)
	{
		if (rayIntersectsSurface(p0, d, getSurface(i), currT))
		{
			if (intersected)
			{
				if (currT < lowestT)
				{
					lowestT = currT;
					lowestTSurface = i;
				}
			}
			else
			{
				intersected = true;
				lowestT = currT;
				lowestTSurface = i;
			}
		}
	}
	
	if (intersected)
	{
		t = lowestT;
		normal = getSurface(lowestTSurface).getNormal();
	}
	return intersected;
}

bool SurfaceShape::lineSegmentIntersects(FCoord3D p0, FCoord3D p1)
{
	for (int i = 0; i < numSurfaces(); i++)
	{
		if (lineSegmentIntersectsSurface(p0, p1, getSurface(i)))
		{
			return true;
		}
	}
	return false;
}

void SurfaceShape::read(std::istream& s)
{
	readAttributes(s);
	
	int n;
	float x, y, z;
	int s1, s2, s3;
	
	s >> n;
	for (int i = 0; i < n; i++)
	{
		s >> x >> y >> z;
		addPoint(FCoord3D(x, y, z));
	}
	
	s >> n;
	for (int i = 0; i < n; i++)
	{
		s >> s1 >> s2 >> s3;
		addSurfaceByIndices(s1, s2, s3);
	}
}

void SurfaceShape::write(std::ostream& s)
{
	s << "SURFACE_SHAPE" << std::endl;
	writeAttributes(s);
	
	s << numPoints() << std::endl;
	for (int i = 0; i < numPoints(); i++)
	{
		s << getPoint(i).x << " " << getPoint(i).y << " " << getPoint(i).z << std::endl;
	}
	
	s << numSurfaces() << std::endl;
	for (int i = 0; i < numSurfaces(); i++)
	{
		s << getSurfaceByIndices(i).a << " " << getSurfaceByIndices(i).b << " " << getSurfaceByIndices(i).c << std::endl;
	}
}


/** Points **/

void SurfaceShape::addPoint(FCoord3D coord)
{
	points->push_back(coord);
}

void SurfaceShape::addPoint(float x, float y, float z)
{
	addPoint(FCoord3D(x, y, z));
}

FCoord3D SurfaceShape::getPoint(int index)
{
	return points->at(index);
}

int SurfaceShape::numPoints()
{
	return points->size();
}

FCoord3D SurfaceShape::getPointNormal(int pointIndex)
{
	assert(pointIndex >= 0 && pointIndex < numPoints());
	
	int num = 0;
	FCoord3D sum = FCoord3D(0.0, 0.0, 0.0);
	
	for (int i = 0; i < numSurfaces(); i++)
	{
		if (getSurfaceByIndices(i).contains(pointIndex + 1))
		{
			num++;
			sum = sum.plus(getSurfaceNormal(i));
		}
	}
	
	// The point must be connected to at least one surface.
	assert(num != 0);
	
	return FCoord3D(sum.x / (float)num, sum.y / (float)num, sum.z / (float)num).makeUnit();
}


/** Surfaces **/

void SurfaceShape::addSurfaceByIndices(SurfaceIndices s)
{
	surfaceIndices->push_back(s);
}

void SurfaceShape::addSurfaceByIndices(int a, int b, int c)
{
	surfaceIndices->push_back(SurfaceIndices(a, b, c));
}

SurfaceIndices SurfaceShape::getSurfaceByIndices(int index)
{
	return surfaceIndices->at(index);
}

Surface SurfaceShape::getSurface(int index)
{
	if (!isValid()) return Surface();
	
	SurfaceIndices si = getSurfaceByIndices(index);
	return Surface(getPoint(si.a - 1), getPoint(si.b - 1), getPoint(si.c - 1));
}

int SurfaceShape::numSurfaces()
{
	return surfaceIndices->size();
}

FCoord3D SurfaceShape::getSurfaceNormal(int surfaceIndex)
{
	return getSurface(surfaceIndex).getNormal();
}


/** Misc. **/

bool SurfaceShape::isValid()
{
	for (int i = 0; i < numSurfaces(); i++)
	{
		if (getSurfaceByIndices(i).a > numPoints() ||
			getSurfaceByIndices(i).b > numPoints() ||
			getSurfaceByIndices(i).c > numPoints() ||
			getSurfaceByIndices(i).a <= 0 ||
			getSurfaceByIndices(i).b <= 0 ||
			getSurfaceByIndices(i).c <= 0)
		{
			return false;
		}
	}
	return true;
}

FCoord3D SurfaceShape::centroid()
{
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	for (int i = 0; i < numPoints(); i++)
	{
		x += points->at(i).x;
		y += points->at(i).y;
		z += points->at(i).z;
	}
	return FCoord3D(x / (float)numPoints(), y / (float)numPoints(), z / (float)numPoints());
}

/** Transformations **/

void SurfaceShape::translate(float x, float y, float z)
{
	for (int i = 0; i < numPoints(); i++)
	{
		points->at(i).x += x;
		points->at(i).y += y;
		points->at(i).z += z;
	}
}

void SurfaceShape::scale(float a, float b, float c)
{
	FCoord3D cent = centroid();
	
	translate(-cent.x, -cent.y, -cent.z);
	scaleOrigin(a, b, c);
	translate(cent.x, cent.y, cent.z);
}

void SurfaceShape::scaleOrigin(float a, float b, float c)
{
	for (int i = 0; i < numPoints(); i++)
	{
		points->at(i).x *= a;
		points->at(i).y *= b;
		points->at(i).z *= c;
	}
}

void SurfaceShape::rotate(FCoord3D p1, FCoord3D p2, float angle)
{
	FCoord3D d = FCoord3D(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z).makeUnit();
	
	if (d.x == 0.0 && d.y == 0.0 && d.z == 0.0) return;
	
	translate(-p1.x, -p1.y, -p1.z);
	if (sqrt(pow(d.y, 2.0) + pow(d.z, 2.0)) == 0.0)
	{
		rotateDegX(d.x > 0.0 ? angle : -angle);
	}
	else
	{
		rotateDIntoZ(d);
		rotateDegZ(angle);
		rotateDOutOfZ(d);
	}
	translate(p1.x, p1.y, p1.z);
}

void SurfaceShape::rotateDIntoZ(FCoord3D d)
{
	float l = sqrt(pow(d.y, 2.0) + pow(d.z, 2.0));
	
	for (int i = 0; i < numPoints(); i++)
	{
		FCoord3D temp = points->at(i);
		points->at(i).x = (l) * temp.x + (-d.x) * temp.z;
		points->at(i).y = (-d.x * d.y / l) * temp.x + (d.z / l) * temp.y + (-d.y) * temp.z;
		points->at(i).z = (d.x * d.z / l) * temp.x + (d.y / l) * temp.y + (d.z) * temp.z;
	}
}

void SurfaceShape::rotateDOutOfZ(FCoord3D d)
{
	float l = sqrt(pow(d.y, 2.0) + pow(d.z, 2.0));
	float lsqxsq = pow(d.x, 2.0) + pow(d.y, 2.0) + pow(d.z, 2.0);
	float ysqzsq = pow(d.y, 2.0) + pow(d.z, 2.0);
	
	for (int i = 0; i < numPoints(); i++)
	{
		FCoord3D temp = points->at(i);
		points->at(i).x = (l / lsqxsq) * temp.x +
			(-l * d.x * d.y / (lsqxsq * ysqzsq)) * temp.y +
			(l * d.x * d.z / (lsqxsq * ysqzsq)) * temp.z;
		points->at(i).y = (l * d.z / ysqzsq) * temp.y +
			(l * d.y / ysqzsq) * temp.z;
		points->at(i).z = (-d.x / lsqxsq) * temp.x +
			(-pow(l, 2.0) * d.y / (lsqxsq * ysqzsq)) * temp.y + 
			(pow(l, 2.0) * d.z / (lsqxsq * ysqzsq)) * temp.z;
	}
}

void SurfaceShape::rotateDegX(float angle)
{
	rotateRadX(angle * M_PI / 180);
}

void SurfaceShape::rotateRadX(float angle)
{
	float ca = cos(angle);
	float sa = sin(angle);
	float newY, newZ;
	
	for (int i = 0; i < numPoints(); i++)
	{
		float y = points->at(i).y;
		float z = points->at(i).z;
		newY = (y * ca) - (z * sa);
		newZ = (y * sa) + (z * ca); 
		points->at(i).y = newY;
		points->at(i).z = newZ;
	}
}

void SurfaceShape::rotateDegZ(float angle)
{
	rotateRadZ(angle * M_PI / 180);
}

void SurfaceShape::rotateRadZ(float angle)
{
	float ca = cos(angle);
	float sa = sin(angle);
	float newX, newY;
	
	for (int i = 0; i < numPoints(); i++)
	{
		float x = points->at(i).x;
		float y = points->at(i).y;
		newX = (x * ca) - (y * sa);
		newY = (x * sa) + (y * ca); 
		points->at(i).x = newX;
		points->at(i).y = newY;
	}
}