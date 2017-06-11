#ifndef __SURFACESHAPE_H__
#define __SURFACESHAPE_H__

/* surfaceShape.h
 * 
 * A shape defined by a number of flat triangular surfaces (inherits from shape).
 * A surface is defined in terms of the indicies of the points it contains.
 * 
 */

#include "misc.h"
#include "shape.h"

class SurfaceShape: public Shape
{
	public:
		/*** Public Member Functions ***/
		// Default constructor.
		SurfaceShape();
		// Destructor.
		virtual ~SurfaceShape();
		
		/** Implemented for Shape **/
		bool rayIntersects(FCoord3D p0, FCoord3D d, float &t, FCoord3D &normal);
		bool lineSegmentIntersects(FCoord3D p0, FCoord3D p1);
		void read(std::istream& s);
		void write(std::ostream& s);
		
		/** Points **/
		// Adds a point to the list of shape points.
		void addPoint(FCoord3D coord);
		void addPoint(float x, float y, float z);
		// Returns a point from the list of shape points.
		FCoord3D getPoint(int index);
		// Returns the number of shape points in the list.
		int numPoints();
		// Returns the outward unit normal vertor at the point.
		FCoord3D getPointNormal(int pointIndex);
		
		/** Surfaces **/
		// Adds a surface to the surface list.
		void addSurfaceByIndices(SurfaceIndices s);
		void addSurfaceByIndices(int a, int b, int c);
		// Returns the points (by index) that this surface contains.
		SurfaceIndices getSurfaceByIndices(int index);
		// Returns the points that this surface contains.
		Surface getSurface(int index);
		// Returns the number of surfaces this object contains.
		int numSurfaces();
		// Returns the outward unit normal vector for the surface.
		FCoord3D getSurfaceNormal(int index);
		
		/** Misc. **/
		// Returns true iff all defined surfaces are valid.
		bool isValid();
		// Returns the centroid of this shape.
		FCoord3D centroid();
		
		/** Transformations **/
		// 3D transformations used to manipulate the shape.
		void translate(float x, float y, float z);
		void scale(float a, float b, float c);
		void scaleOrigin(float a, float b, float c);
		void rotate(FCoord3D p1, FCoord3D p2, float angle);
		void rotateDIntoZ(FCoord3D d);
		void rotateDOutOfZ(FCoord3D d);
		void rotateDegX(float angle);
		void rotateRadX(float angle);
		void rotateDegZ(float angle);
		void rotateRadZ(float angle);
		
	private:
		/*** Private Member Variables ***/
		// The points this shape contains.
		std::vector<FCoord3D>* points;
		// The surfaces this shape contains. SurfaceIndices holds the indices of 3 points in the shape, which defines the surface.
		std::vector<SurfaceIndices>* surfaceIndices;
};

#endif