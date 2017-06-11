#ifndef __SHAPE_H__
#define __SHAPE_H__

/* shape.h
 * 
 * An abstract class that defines what a shape should implement.
 * Includes protected members for storing values related to material properties.
 * 
 */

#include <vector>

#include "misc.h"


class Shape
{
	public:
		/*** Public Member Functions ***/
		/** Pure Virtual **/
		// Returns true iff the ray defined by the point and dirction vector intersects this shape.
		// If it does, the t-value and surface normal of the first intersection are returned by reference.
		virtual bool rayIntersects(FCoord3D p0, FCoord3D d, float &t, FCoord3D &normal) = 0;
		// Returns true iff the line segment from p0 to p1 intersects the shape.
		virtual bool lineSegmentIntersects(FCoord3D p0, FCoord3D p1) = 0;
		// Reads the shape data from the stream.
		virtual void read(std::istream& s) = 0;
		// Writes the shape data to the stream.
		virtual void write(std::ostream& s) = 0;
		
		/** Virtual **/
		// Virtual public destructor.
		virtual ~Shape();
		//
		virtual void readAttributes(std::istream& s);
		virtual void writeAttributes(std::ostream& s);
		
		// Material property setters and getters.
		virtual RGB getColor();
		virtual void setColor(RGB c);
		virtual float getRefl();
		virtual void setRefl(float f);
		virtual float getRefr();
		virtual void setRefr(float f);
		virtual float getRefractiveIndex();
		virtual void setRefractiveIndex(float f);
		virtual int getPhongExponent();
		virtual void setPhongExponent(int i);
		
		/** Static **/
		// Returns a cube of the specified dimensions and material properties.
		static Shape* createCube(FCoord3D origin, float sideLen, RGB color, float refl, float refr, float refrIndex, int phongE);
		// Returns a sphere of the specified dimensions and material properties.
		static Shape* createSphere(FCoord3D origin, float radius, RGB color, float refl, float refr, float refrIndex, int phongE);
		
		
	protected:
		/*** Protected Member Variables ***/
		// The color of this shape.
		RGB color;
		// The amount this shape reflects (0.0 to 1.0).
		float reflectionCoefficient;
		// The amount this shape refracts (0.0 to 1.0).
		float refractionCoeffieient;
		// The refractive index of this shape (determines refraction angle).
		float refractiveIndex;
		// Determines specular highlight size. Low implies a larger highlight.
		int phongExponent;
};


#endif