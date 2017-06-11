#include "shapeCollection.h"

#include <fstream>

#include "implicitShape.h"
#include "shape.h"
#include "surfaceShape.h"


/*** Public Member Functions ***/

ShapeCollection::ShapeCollection()
{
	shapes = new std::vector<Shape*>();
	viewport = nullptr;
}

void ShapeCollection::setViewport(Viewport* _viewport)
{
	viewport = _viewport;
}

void ShapeCollection::add(Shape* shape)
{
	shapes->push_back(shape);
}

Shape* ShapeCollection::get(int index)
{
	if (index < 0 || index >= numShapes()) return nullptr;
	return shapes->at(index);
}

void ShapeCollection::remove(int index)
{
	if (index < 0 || index >= numShapes()) return;
	shapes->erase(shapes->begin() + index);
}

int ShapeCollection::numShapes()
{
	return shapes->size();
}

bool ShapeCollection::rayIntersects(FCoord3D p0, FCoord3D d, float &t, FCoord3D &normal, int &shapeIndex)
{
	float bestT = 0.0;
	FCoord3D bestNormal = FCoord3D();
	int bestShape = -1;
	
	float currT = 0.0;
	FCoord3D currNormal = FCoord3D();
	
	bool intersected = false;
	
	for (int i = 0; i < numShapes(); i++)
	{
		if(get(i)->rayIntersects(p0, d, currT, currNormal))
		{
			if (intersected)
			{
				if (currT < bestT)
				{
					bestT = currT;
					bestNormal = currNormal;
					bestShape = i;
				}
			}
			else
			{
				intersected = true;
				bestT = currT;
				bestNormal = currNormal;
				bestShape = i;
			}
		}
	}
	
	if (intersected)
	{
		t = bestT;
		normal = bestNormal;
		shapeIndex = bestShape;
	}
	return intersected;
}

bool ShapeCollection::lineSegmentIntersects(FCoord3D p0, FCoord3D p1)
{
	for (int i = 0; i < numShapes(); i++)
	{
		if (get(i)->lineSegmentIntersects(p0, p1))
		{
			return true;
		}
	}
	return false;
}


/** File I/O **/
bool ShapeCollection::loadFromFile(std::string fileName)
{
	std::ifstream file(fileName.c_str());
	if (!file.is_open()) return false;
	
	readSceneAttributes(file);
	
	int n = 0;
	file >> n;
	
	std::string shapeType;
	for (int i = 0; i < n; i++)
	{
		file >> shapeType;
		
		Shape* shape;
		if (shapeType.compare("IMPLICIT_SHAPE") == 0)
		{
			shape = new ImplicitShape();
		}
		else if (shapeType.compare("SURFACE_SHAPE") == 0)
		{
			shape = new SurfaceShape();
		}
		else
		{
			file.close();
			return false;
		}
		
		shape->read(file);
		add(shape);
	}
	
	file.close();
	return true;
}

bool ShapeCollection::saveToFile(std::string fileName)
{
	std::ofstream file(fileName.c_str());
	if (!file.is_open()) return false;
	
	writeSceneAttributes(file);
	
	file << numShapes() << std::endl << std::endl;
	for (int i = 0; i < numShapes(); i++)
	{
		get(i)->write(file);
		file << std::endl;
	}
	
	file.close();
	return true;
}

void ShapeCollection::readSceneAttributes(std::istream& s)
{
	viewport->readSceneAttributes(s);
}

void ShapeCollection::writeSceneAttributes(std::ostream& s)
{
	viewport->writeSceneAttributes(s);
}