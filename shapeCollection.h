#ifndef __SHAPECOLLECTION_H__
#define __SHAPECOLLECTION_H__

/* shapeCollection.h
 * 
 * Defines a collection of shapes that can be attached to a viewport for displaying.
 * Has functions to determine if any shape in the collection is intersected by a ray.
 * 
 */

#include <string>
#include <vector>

#include "misc.h"
#include "viewport.h"

class Shape;

class ShapeCollection
{
	public:
		/*** Public Member Functions ***/
		ShapeCollection();
		
		// Sets the attached viewport.
		void setViewport(Viewport* _viewport);
		
		// Adds a shape to the collection.
		void add(Shape* shape);
		// Returns a shape from the collection.
		Shape* get(int index);
		// Removes a shape from the collection (the shape is destroyed).
		void remove(int index);
		// Returns the number of shapes in the collection.
		int numShapes();
		
		// Returns true iff the ray defined by the point and dirction vector intersects a shape in the collection.
		// If it does, the t-value, surface normal, and the shape index of the first intersection are returned.
		bool rayIntersects(FCoord3D p0, FCoord3D d, float &t, FCoord3D &normal, int &shapeIndex);
		// Returns true iff the line segment defined by the points intersects a shape.
		bool lineSegmentIntersects(FCoord3D p0, FCoord3D p1);
		
		/** File I/O **/
		// Load/save the collection to/from a file.
		bool loadFromFile(std::string fileName);
		bool saveToFile(std::string fileName);
		// Read/write the scene attributes to/from a stream.
		void readSceneAttributes(std::istream& s);
		void writeSceneAttributes(std::ostream& s);
		
	private:
		/*** Private Member Variables ***/
		std::vector<Shape*>* shapes;
		Viewport* viewport;
};


#endif