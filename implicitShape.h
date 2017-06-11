#ifndef __IMPLICITSHAPE_H__
#define __IMPLICITSHAPE_H__

/* implicitShape.h
 * 
 * An implicitly defined shape (inherits from shape).
 * 
 */

#include "misc.h"
#include "shape.h"

class ImplicitShape: public Shape
{
	public:
		/*** Public Member Functions ***/
		// Default constructor.
		ImplicitShape();
		// General constructor.
		ImplicitShape(
			float _c200, float _c020, float _c002,
			float _c110, float _c101, float _c011,
			float _c100, float _c010, float _c001,
			float _c000
		);
		// Destructor.
		virtual ~ImplicitShape();
		
		// Returns the surface normal at the passed point. Does not ensure that the point lies on the shape.
		FCoord3D getNormal(FCoord3D point);
		// Universal setter for an implicitly defined shape.
		void setCoefficients(
			float _c200, float _c020, float _c002,
			float _c110, float _c101, float _c011,
			float _c100, float _c010, float _c001,
			float _c000
		);
		
		/** Implemented for Shape **/
		bool rayIntersects(FCoord3D p0, FCoord3D d, float &t, FCoord3D &normal);
		bool lineSegmentIntersects(FCoord3D p0, FCoord3D p1);
		void read(std::istream& s);
		void write(std::ostream& s);
		
	private:
		/*** Private Member Variables ***/
		float c200, c020, c002, c110, c101, c011, c100, c010, c001, c000;
};

#endif