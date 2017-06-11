#include "viewport.h"

#include <assert.h>
#include <math.h>
#include <vector>

#include "phongLightSource.h"
#include "shapeCollection.h"
#include "surfaceShape.h"
#include "main.h"
#include "misc.h"

Viewport::Viewport(Coord _origin, int _size, ShapeCollection* _shapes)
{
	assert(_size > 0);
	
	origin = _origin;
	size = _size;
	shapes = _shapes;
	
	outlineColor = OUTLINE_COLOR_DEFAULT();
	backgroundColor = BACKGROUND_COLOR_DEFAULT();
	
	fromPoint = FCoord3D(1000, 100, 1000);
	atPoint = FCoord3D(0, 0, 0);
	upVector = FCoord3D(0, 0, 1);
	viewingAngleDeg = 30.0;
	
	lightSources = new std::vector<PhongLightSource*>();
	// ambientColor = RGB(0, 0, 1);
	ambientIntensity = 0.2;
	rayTracingRecursionLayers = 10;
}

void Viewport::pixelMake(int x, int y, RGB color)
{
	if (pixelIn(x, y))
	{
		makePix(origin.x + x, origin.y + y, color);
	}
}

RGB Viewport::pixelGet(int x, int y)
{
	if (pixelIn(x, y))
	{
		return getPix(origin.x + x, origin.y + y);
	}
	else
	{
		return RGB();
	}
}

bool Viewport::pixelIn(int x, int y)
{
	if (x < 0 || x >= size || y < 0 || y >= size) return false;
	else return true;
}


/** File I/O **/

void Viewport::readSceneAttributes(std::istream& s)
{
	backgroundColor.read(s);
	
	// CVM Parameters.
	fromPoint.read(s);
	atPoint.read(s);
	upVector.read(s);
	
	float f;
	s >> f;
	viewingAngleDeg = f;
	
	s >> f;
	ambientIntensity = f;
	
	// Phong Parameters.
	int n;
	s >> n;
	
	PhongLightSource* light;
	for (int i = 0; i < n; i++)
	{
		light = new PhongLightSource();
		light->read(s);
		addLight(light);
	}
}

void Viewport::writeSceneAttributes(std::ostream& s)
{
	backgroundColor.write(s);
		
	// CVM Parameters.
	fromPoint.write(s);
	atPoint.write(s);
	upVector.write(s);
	s << viewingAngleDeg << std::endl;
	s << ambientIntensity << std::endl;
	s << std::endl;
	
	// Phong Parameters.
	int n = lightSources->size();
	s << n << std::endl;
	for (int i = 0; i < n; i++)
	{
		lightSources->at(i)->write(s);
	}
	s << std::endl;
}



int Viewport::getSize()
{
	return size;
}

void Viewport::setFromPoint(FCoord3D ff)
{
	fromPoint = ff;
}

FCoord3D Viewport::getFromPoint()
{
	return fromPoint;
}

void Viewport::setAtPoint(FCoord3D a)
{
	atPoint = a;
}

FCoord3D Viewport::getAtPoint()
{
	return atPoint;
}

void Viewport::setUpVector(FCoord3D u)
{
	upVector = u;
}

FCoord3D Viewport::getUpVector()
{
	return upVector;
}

void Viewport::setViewingAngle(float alphaDeg)
{
	viewingAngleDeg = alphaDeg;
}

void Viewport::moveCamera(Direction d, float f)
{
	FCoord3D vForward = atPoint.minus(fromPoint).makeUnit();
	FCoord3D vRight = vForward.crossProduct(upVector).makeUnit();
	FCoord3D vUp = vRight.crossProduct(vForward).makeUnit();
	
	
	float multi = 1.0;
	
	switch (d)
	{
		case dLeft:
			multi = -1.0;
		case dRight:
			atPoint = atPoint.plus(vRight.multiply(multi * f));
			fromPoint = fromPoint.plus(vRight.multiply(multi * f));
			break;
		
		case dDown:
			multi = -1.0;
		case dUp:
			atPoint = atPoint.plus(vUp.multiply(multi * f));
			fromPoint = fromPoint.plus(vUp.multiply(multi * f));
			break;
		
		case dBackward:
			multi = -1.0;
		case dForward:
			atPoint = atPoint.plus(vForward.multiply(multi * f));
			fromPoint = fromPoint.plus(vForward.multiply(multi * f));
			break;
		
		default:
			break;
	}
}

void Viewport::moveAtPoint(Direction d, float f)
{
	FCoord3D vForward = atPoint.minus(fromPoint).makeUnit();
	FCoord3D vRight = vForward.crossProduct(upVector).makeUnit();
	FCoord3D vUp = vRight.crossProduct(vForward).makeUnit();
	
	
	float multi = 1.0;
	
	switch (d)
	{
		case dLeft:
			multi = -1.0;
		case dRight:
			atPoint = atPoint.plus(vRight.multiply(multi * f));
			break;
		
		case dDown:
			multi = -1.0;
		case dUp:
			atPoint = atPoint.plus(vUp.multiply(multi * f));
			break;
		
		case dBackward:
			multi = -1.0;
		case dForward:
			atPoint = atPoint.plus(vForward.multiply(multi * f));
			break;
		
		default:
			break;
	}
}

void Viewport::moveFromPoint(Direction d, float f)
{
	FCoord3D vForward = atPoint.minus(fromPoint).makeUnit();
	FCoord3D vRight = vForward.crossProduct(upVector).makeUnit();
	FCoord3D vUp = vRight.crossProduct(vForward).makeUnit();
	
	
	float multi = 1.0;
	
	switch (d)
	{
		case dLeft:
			multi = -1.0;
		case dRight:
			fromPoint = fromPoint.plus(vRight.multiply(multi * f));
			break;
		
		case dDown:
			multi = -1.0;
		case dUp:
			fromPoint = fromPoint.plus(vUp.multiply(multi * f));
			break;
		
		case dBackward:
			multi = -1.0;
		case dForward:
			fromPoint = fromPoint.plus(vForward.multiply(multi * f));
			break;
		
		default:
			break;
	}
}

void Viewport::addLight(PhongLightSource* light)
{
	lightSources->push_back(light);
}

void Viewport::deleteLight(int index)
{
	if (index < 0 || index >= (int)lightSources->size()) return;
	lightSources->erase(lightSources->begin() + index);
}

void Viewport::drawOutline()
{
	void drawLineBresenham(int x1, int y1, int x2, int y2, RGB color);
	
	drawLineBresenham(
		origin.x - 1, origin.y - 1,
		origin.x - 1, origin.y + size,
		outlineColor
	);
	drawLineBresenham(
		origin.x - 1, origin.y + size,
		origin.x + size, origin.y + size,
		outlineColor
	);
	drawLineBresenham(
		origin.x + size, origin.y + size,
		origin.x + size, origin.y - 1,
		outlineColor
	);
	drawLineBresenham(
		origin.x + size, origin.y - 1,
		origin.x - 1, origin.y - 1,
		outlineColor
	);
}

void Viewport::fillBackground()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			makePix(origin.x + i, origin.y + j, backgroundColor);
		}
	}
}

void Viewport::drawDiamond(Coord coord, RGB color)
{
	
	pixelMake(coord.x, coord.y + 3, color);
	
	pixelMake(coord.x - 1, coord.y + 2, color);
	pixelMake(coord.x, coord.y + 2, color);
	pixelMake(coord.x + 1, coord.y + 2, color);
	
	pixelMake(coord.x - 2, coord.y + 1, color);
	pixelMake(coord.x - 1, coord.y + 1, color);
	pixelMake(coord.x, coord.y + 1, color);
	pixelMake(coord.x + 1, coord.y + 1, color);
	pixelMake(coord.x + 2, coord.y + 1, color);
	
	pixelMake(coord.x - 3, coord.y, color);
	pixelMake(coord.x - 2, coord.y, color);
	pixelMake(coord.x - 1, coord.y, color);
	pixelMake(coord.x, coord.y, color);
	pixelMake(coord.x + 1, coord.y, color);
	pixelMake(coord.x + 2, coord.y, color);
	pixelMake(coord.x + 3, coord.y, color);
	
	pixelMake(coord.x - 2, coord.y - 1, color);
	pixelMake(coord.x - 1, coord.y - 1, color);
	pixelMake(coord.x, coord.y - 1, color);
	pixelMake(coord.x + 1, coord.y - 1, color);
	pixelMake(coord.x + 2, coord.y - 1, color);
	
	pixelMake(coord.x - 1, coord.y - 2, color);
	pixelMake(coord.x, coord.y - 2, color);
	pixelMake(coord.x + 1, coord.y - 2, color);
	
	pixelMake(coord.x, coord.y - 3, color);
}

void Viewport::drawLine(Coord p1, Coord p2, RGB color)
{
	int x1 = p1.x;
	int y1 = p1.y;
	int x2 = p2.x;
	int y2 = p2.y;
	
	int leftX = x1 < x2 ? x1 : x2;
	int leftY = x1 < x2 ? y1 : y2;
	int rightX = x1 < x2 ? x2 : x1;
	int rightY = x1 < x2 ? y2 : y1;
	
	int bottomX = y1 < y2 ? x1 : x2;
	int bottomY = y1 < y2 ? y1 : y2;
	//int topX = y1 < y2 ? x2 : x1;
	int topY = y1 < y2 ? y2 : y1;
	
	int dx = abs(x1 - x2);
	int dy = abs(y1 - y2);
	bool posSlope = (rightY - leftY) > 0;
	
	int predicate = 0;
	bool move = false;
	
	if (dx > dy) // The line is longer in the X direction than the Y direction (wide).
	{
		int j = 0;
		for (int i = 0; i <= (rightX - leftX); i++)
		{
			pixelMake(leftX + i, leftY + j, color);
			if (i == 0)
			{
				predicate = (2 * dy) - dx;
			}
			else
			{
				predicate = predicate + (2 * dy) - (2 * dx * (move ? 1 : 0));
			}
			
			move = (predicate > 0);
			if (move)
			{
				posSlope ? j++ : j--;
			}
		}
	}
	else // / The line is longer in the Y direction than the X direction (tall).
	{
		int i = 0;
		for (int j = 0; j <= (topY - bottomY); j++)
		{
			pixelMake(bottomX + i, bottomY + j, color);
			if (j == 0)
			{
				predicate = (2 * dx) - dy;
			}
			else
			{
				predicate = predicate + (2 * dx) - (2 * dy * (move ? 1 : 0));
			}
			
			move = (predicate > 0);
			if (move)
			{
				posSlope ? i++ : i--;
			}
		}
	}
}



void Viewport::redraw(bool loadingText)
{
	const int pixelEvery = size * size / 45;
	int counter = 0;
	
	if (loadingText)
	{
		std::cout << "|  Please wait while the scene is ray-traced  |" << std::endl << " ";
	}
	
	RGB color;
	float max;
	for (int j = 0; j < size; j++)
	{
		for (int i = 0; i < size; i++)
		{
			color = calculatePixelColor(i, j);
			
			max = color.red;
			if (color.green > max) max = color.green;
			if (color.blue > max) max = color.blue;
			
			if (max > 1.0)
			{
				color = color.scale(1.0 / max);
			}
			
			pixelMake(i, j, color);
			
			counter++;
			if (counter == pixelEvery)
			{
				if (loadingText)
				{
					std::cout << "*" << std::flush;
				}
				counter = 0;
			}
		}
	}
	if (loadingText)
	{
		std::cout << std::endl;
	}
}

RGB Viewport::calculatePixelColor(int i, int j)
{
	std::vector<bool> inShape = std::vector<bool>();
	for (int i = 0; i < shapes->numShapes(); i++)
	{
		inShape.push_back(false);
	}
	return calculatePhongColor(fromPoint, getRayDir(i, j), 0, inShape, 1.0);
}

RGB Viewport::calculatePhongColor(FCoord3D ff, FCoord3D rayDir, int rLayer, std::vector<bool> inShape, float recursiveScaling)
{
	const float SURFACE_EPSILON = 0.01;
	const float MIN_RECURSIVE_SCALING = 0.01;
	
	float t = 0.0;
	FCoord3D normal = FCoord3D();
	int shapeIndex = -1;
	if (shapes->rayIntersects(ff, rayDir, t, normal, shapeIndex))
	{
		assert(normal.length() != 0.0);
		
		normal = normal.makeUnit();
		
		Shape* shape = shapes->get(shapeIndex);
		FCoord3D point = ff.plus(rayDir.multiply(t));
		FCoord3D viewVector = ff.minus(point).makeUnit();
		if (normal.dotProduct(viewVector) < 0)
		{ // Ensures that the normal points towards the view vector.
			normal = normal.negate();
		}
		
		// Use the average of the light sources as the ambient color.
		RGB pointColor = RGB(0, 0, 0);
		if (lightSources->size() == 0)
		{
			pointColor = RGB(0.1, 0.1, 0.1);
		}
		else
		{
			for (int i = 0; i < (int)lightSources->size(); i++)
			{
				pointColor = pointColor.add(lightSources->at(i)->color);
			}
			pointColor = pointColor.scale(ambientIntensity / (int)lightSources->size());
		}
		
		for (int i = 0; i < (int)lightSources->size(); i++)
		{
			PhongLightSource* light = lightSources->at(i);
			
			FCoord3D lightVector = light->position.minus(point).makeUnit();
			if (!shapes->lineSegmentIntersects(point.plus(lightVector.multiply(SURFACE_EPSILON)), light->position))
			{
				FCoord3D reflectionVector = lightVector.negate().plus(normal.multiply(2.0 * normal.dotProduct(lightVector)));
				
				float scalar = light->intensity / (point.minus(ff).length() + point.minus(light->position).length());
				if (normal.dotProduct(lightVector) > 0)
				{ // If light and viewer on same side, add diffuse color.
					pointColor = pointColor.add(shape->getColor().scale(scalar * normal.dotProduct(lightVector)));
					if (viewVector.dotProduct(reflectionVector) > 0)
					{ // If view vector is within 90 degrees of reflection vector, add specular color.
						pointColor = pointColor.add(light->color.scale(scalar * pow(viewVector.dotProduct(reflectionVector), shape->getPhongExponent())));
					}
				}
			}
			
		}
		
		if (rLayer == rayTracingRecursionLayers)
		{
			return pointColor;
		}
		else
		{
			float reflWeight = shape->getRefl();
			float refrWeight = shape->getRefr();
			float pointWeight = 1.0 - (reflWeight + refrWeight);
			
			RGB reflColor = RGB(0, 0, 0);
			if (reflWeight != 0.0 && recursiveScaling > MIN_RECURSIVE_SCALING)
			{ // If the object is not reflective, or the multiplyer on this color is so low it will make no difference, don't recurse.
				FCoord3D refl = viewVector.negate().plus(normal.multiply(2.0 * normal.dotProduct(viewVector)));
				// Calculate the color of the reflected ray recursively.
				// Note: Shifts the point out of the object to ensure that the same surface is not intersected immediately.
				reflColor = calculatePhongColor(point.plus(normal.multiply(SURFACE_EPSILON)), refl, rLayer + 1, inShape, recursiveScaling * reflWeight);
			}
			
			RGB refrColor = RGB(0, 0, 0);
			if (refrWeight != 0.0 && recursiveScaling > MIN_RECURSIVE_SCALING)
			{ // If the object is not refractive, or the multiplyer on this color is so low it will make no difference, don't recurse.
				float n1 = getRefractiveIndex(inShape);
				inShape.at(shapeIndex) = !inShape.at(shapeIndex);
				float n2 = getRefractiveIndex(inShape);
				
				float alpha = acos(viewVector.dotProduct(normal));
				float beta = asin((n1 / n2) * sin(alpha));
				float l1 = sin(beta);
				float l2 = cos(beta);
				FCoord3D h = viewVector.negate().plus(normal.multiply(normal.dotProduct(viewVector))).makeUnit();
				
				FCoord3D refr = h.multiply(l1).minus(normal.multiply(l2));
				// Calculate the color of the refracted ray recursively.
				// Note: Shifts the point into the object to ensure that the same surface is not intersected immediately.
				refrColor = calculatePhongColor(point.plus(normal.multiply(-SURFACE_EPSILON)), refr, rLayer + 1, inShape, recursiveScaling * refrWeight);
			}
			
			return pointColor.scale(pointWeight).add(reflColor.scale(reflWeight)).add(refrColor.scale(refrWeight));
		}
		
		return pointColor;
	}
	else
	{
		return backgroundColor;
	}
}

FCoord3D Viewport::getRayDir(int i, int j)
{
	assert(atPoint.minus(fromPoint).length() != 0.0);
	
	FCoord3D b3 = atPoint.minus(fromPoint).makeUnit();
	FCoord3D b1 = b3.crossProduct(upVector).makeUnit();
	FCoord3D b2 = b1.crossProduct(b3).makeUnit();
	
	FCoord3D ptEye = FCoord3D(
		((float)i / (float)(size - 1)) - 0.5,
		((float)j / (float)(size - 1)) - 0.5,
		1.0 / (2.0 * tan(viewingAngleDeg * M_PI / 180.0))
	);
	
	FCoord3D ptWorld = FCoord3D(
		fromPoint.x + (ptEye.x * b1.x) + (ptEye.y * b2.x) + (ptEye.z * b3.x),
		fromPoint.y + (ptEye.x * b1.y) + (ptEye.y * b2.y) + (ptEye.z * b3.y),
		fromPoint.z + (ptEye.x * b1.z) + (ptEye.y * b2.z) + (ptEye.z * b3.z)
	);
	
	return ptWorld.minus(fromPoint).makeUnit();
}


float Viewport::getRefractiveIndex(std::vector<bool> inShape)
{
	assert((int)inShape.size() == shapes->numShapes());
	
	float result = 0.0;
	int num = 0;
	for (int i = 0; i < (int)inShape.size(); i++)
	{
		if (inShape.at(i))
		{
			result += shapes->get(i)->getRefractiveIndex();
			num++;
		}
	}
	
	if (num == 0)
	{
		return 1.0;
	}
	else
	{
		return (result / (float)num);
	}
}



/*** Private ***/
