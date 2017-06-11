#include "shape.h"

#include <math.h>

#include "implicitShape.h"
#include "surfaceShape.h"


/*** Public Member Functions ***/

/** Virtual **/

Shape::~Shape()
{
}

void Shape::readAttributes(std::istream& s)
{
	RGB c;
	float f;
	int i;
	
	c.read(s);
	setColor(c);
	
	s >> f;
	setRefl(f);
	
	s >> f;
	setRefr(f);
	
	s >> f;
	setRefractiveIndex(f);
	
	s >> i;
	setPhongExponent(i);
}

void Shape::writeAttributes(std::ostream& s)
{
	getColor().write(s);
	s << getRefl() << std::endl;
	s << getRefr() << std::endl;
	s << getRefractiveIndex() << std::endl;
	s << getPhongExponent() << std::endl;
}

RGB Shape::getColor()
{
	return color;
}

void Shape::setColor(RGB c)
{
	color = c;
}

float Shape::getRefl()
{
	return reflectionCoefficient;
}

void Shape::setRefl(float f)
{
	reflectionCoefficient = f;
}

float Shape::getRefr()
{
	return refractionCoeffieient;
}

void Shape::setRefr(float f)
{
	refractionCoeffieient = f;
}

float Shape::getRefractiveIndex()
{
	return refractiveIndex;
}

void Shape::setRefractiveIndex(float f)
{
	refractiveIndex = f;
}

int Shape::getPhongExponent()
{
	return phongExponent;
}

void Shape::setPhongExponent(int i)
{
	phongExponent = i;
}


/** Static **/

Shape* Shape::createCube(FCoord3D origin, float sideLen, RGB color, float refl, float refr, float refrIndex, int phongE)
{
	float halfSide = sideLen / 2.0;
	
	SurfaceShape* shape = new SurfaceShape();
	shape->addPoint(origin.x - halfSide, origin.y - halfSide, origin.z - halfSide);
	shape->addPoint(origin.x - halfSide, origin.y + halfSide, origin.z - halfSide);
	shape->addPoint(origin.x + halfSide, origin.y - halfSide, origin.z - halfSide);
	shape->addPoint(origin.x + halfSide, origin.y + halfSide, origin.z - halfSide);
	
	shape->addPoint(origin.x - halfSide, origin.y - halfSide, origin.z + halfSide);
	shape->addPoint(origin.x - halfSide, origin.y + halfSide, origin.z + halfSide);
	shape->addPoint(origin.x + halfSide, origin.y - halfSide, origin.z + halfSide);
	shape->addPoint(origin.x + halfSide, origin.y + halfSide, origin.z + halfSide);
	
	shape->addSurfaceByIndices(1, 3, 2);
	shape->addSurfaceByIndices(3, 4, 2);
	
	shape->addSurfaceByIndices(5, 6, 8);
	shape->addSurfaceByIndices(8, 7, 5);
	
	shape->addSurfaceByIndices(1, 5, 3);
	shape->addSurfaceByIndices(5, 7, 3);
	
	shape->addSurfaceByIndices(4, 6, 2);
	shape->addSurfaceByIndices(4, 8, 6);
	
	shape->addSurfaceByIndices(2, 6, 1);
	shape->addSurfaceByIndices(5, 1, 6);
	
	shape->addSurfaceByIndices(4, 3, 8);
	shape->addSurfaceByIndices(3, 7, 8);
	
	shape->setColor(color);
	shape->setRefl(refl);
	shape->setRefr(refr);
	shape->setRefractiveIndex(refrIndex);
	shape->setPhongExponent(phongE);
	return shape;
}

Shape* Shape::createSphere(FCoord3D origin, float radius, RGB color, float refl, float refr, float refrIndex, int phongE)
{
	ImplicitShape* shape = new ImplicitShape(
		1, 1, 1,
		0, 0, 0,
		-2.0 * origin.x, -2.0 * origin.y, -2.0 * origin.z,
		pow(origin.x, 2.0) + pow(origin.y, 2.0) + pow(origin.z, 2.0) - pow(radius, 2.0)
	);
	
	shape->setColor(color);
	shape->setRefl(refl);
	shape->setRefr(refr);
	shape->setRefractiveIndex(refrIndex);
	shape->setPhongExponent(phongE);
	return shape;
}