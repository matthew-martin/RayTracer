#ifndef __MISC_H__
#define __MISC_H__

/* misc.h
 * 
 * A file containing small structs, enums, and helper functions that are commonly used.
 * 
 */

#include <assert.h>
#include <iostream>
#include <vector>

struct FCoord;
class Shape;

enum BasePlane {bpXY = 0, bpXZ, bpYZ};
enum Direction {dLeft, dRight, dUp, dDown, dForward, dBackward};
enum LineType {ltBresenham, ltDDA};
enum NDCType {ndctSimplified, ndctGeneralized};
enum ProjectionType {ptAxonometric, ptCabinet, ptCavalier};

struct RGB
{
	float red;
	float green;
	float blue;
	
	RGB();
	RGB(float r, float g, float b);
	
	RGB scale(float f);
	RGB add(RGB other);
	
	int intensity(int numLevels);
	
	void read(std::istream& s);
	void write(std::ostream& s);
	bool isDark();
	static RGB intRGB(int r, int g, int b);
	static RGB random();
	// Returns a similar color to the passed color, skewed by a small amount.
	static RGB skew(RGB color, float skew);
};

const RGB COLOR_RED = RGB(1, 0, 0);
const RGB COLOR_ORANGE = RGB(1, 0.4, 0);
const RGB COLOR_YELLOW = RGB(1, 1, 0);
const RGB COLOR_GREEN = RGB(0, 1, 0);
const RGB COLOR_BLUE = RGB(0, 0, 1);
const RGB COLOR_PURPLE = RGB(1, 0, 1);
const RGB COLOR_WHITE = RGB(1, 1, 1);
const RGB COLOR_GREY = RGB(0.5, 0.5, 0.5);

struct RegionCode
{
	bool code[4];
	
	RegionCode();
	RegionCode(bool left, bool right, bool bottom, bool top);
	RegionCode(FCoord coord, float xMin, float xMax, float yMin, float yMax);
	bool getValue(int dir);
	bool isZero();
	// Returns the first found position (castable to Direction enum) where the passed codes
	// differs from this code. If the codes are identical, a negative value is returned;
	int diffBit(RegionCode other);
	RegionCode bitAnd(RegionCode other);
	RegionCode bitOr(RegionCode other);
};

struct Coord
{
	int x;
	int y;
	
	Coord();
	Coord(int xIn, int yIn);
	void print();
	FCoord toFCoord();
};

struct FCoord
{
	float x;
	float y;
	
	FCoord();
	FCoord(float xIn, float yIn);
	void print();
	Coord toCoord();
	
	FCoord multiply(float f);
	FCoord plus(FCoord other);
};

struct FCoord3D
{
	float x;
	float y;
	float z;
	
	FCoord3D();
	FCoord3D(float xIn, float yIn, float zIn);
	
	float length();
	FCoord3D makeUnit();
	
	FCoord3D negate();
	FCoord3D multiply(float f);
	FCoord3D plus(FCoord3D other);
	FCoord3D minus(FCoord3D other);
	float dotProduct(FCoord3D other);
	FCoord3D crossProduct(FCoord3D other);
	
	void print();
	void read(std::istream& s);
	void write(std::ostream& s);
};


struct SurfaceIndices
{
	int a;
	int b;
	int c;
	
	SurfaceIndices();
	SurfaceIndices(int _a, int _b, int _c);
	
	// Returns true iff the surface contains the specified point (1-indexed).
	bool contains(int pt);
};

struct Surface
{
	FCoord3D a;
	FCoord3D b;
	FCoord3D c;
	
	Surface();
	Surface(FCoord3D _a, FCoord3D _b, FCoord3D _c);
	
	FCoord3D getNormal();
	
	void print();
};

struct PhongData
{
	PhongData();
	
	FCoord3D fromPoint;
	FCoord3D lightSource;
	
	float ambientIntensity; // [0, 1]
	RGB ambientColorCoefficient;
	
	float lightSourceIntensity; // [0, 1]
	RGB specularColorCoefficient;
	
	float C; // Positive (represents distance from light source to point).
	int n; // {1, 2, 3, ...}
};



// Returns the y-intercept of the line defined by the given points, and the vertical line at the given x-value.
// Will crash if the given line is vertical.
float findYIntercept(FCoord p1, FCoord p2, float x);

// Returns the x-intercept of the line defined by the given points, and the horizontal line at the given y-value.
// Will crash if the given line is horizontal.
float findXIntercept(FCoord p1, FCoord p2, float y);

// Returns the intercept of the lines defined by the given points. Will crash if the lines are parallel.
FCoord findIntercept(FCoord l1p1, FCoord l1p2, FCoord l2p1, FCoord l2p2);

// Returns true if the given points are on the same side of the line defined by the passed line points.
// Undefined behavior if a given point lies exactly on the line, or the "line" points are the same (but will not crash).
bool pointsSameSide(FCoord lp1, FCoord lp2, FCoord point1, FCoord point2);

// Returns true if the line segment defined by the coords passed through the passed y-value (defining a scanline).
bool inScanline(Coord p1, Coord p2, int scanline);

// Returns the index (1-3) of the minimum of the passed numbers (only positive numbers will be considered).
int findMinPos(int a, int b, int c);

// Returns the index (1-3) of the maximum of the passed numbers (only positive numbers will be considered).
int findMaxPos(int a, int b, int c);

// Returns the interpolated color based on the passed points and associated colors, and the y/x-value.
// Assumes the y-value(x-value) is between the points vertically(horizontally).
// Fails for horizontal(vertical) lines.
RGB interpolateColorY(Coord p1, RGB c1, Coord p2, RGB c2, int y);
RGB interpolateColorX(Coord p1, RGB c1, Coord p2, RGB c2, int x);

// Returns the value of the specified Burnstein-Bezier polynomial, ((B_i)^n)(t).
// 0.0 <= t <= 1.0.
float burnsteinBezierPolynomial(int i, int n, float t);
// Returns n choose k.
// From http://stackoverflow.com/questions/9330915/number-of-combinations-n-choose-r-in-c
int nChooseK(int n, int k);

// Returns true iff the ray defined by the point p0 and the vector d intersects the surface.
// If true is returned, t is filled with the t-value of the intersection.
bool rayIntersectsSurface(FCoord3D p0, FCoord3D d, Surface surface, float &t);
// Returns true iff the line segment defined by the two points intersects the surface.
bool lineSegmentIntersectsSurface(FCoord3D p0, FCoord3D p1, Surface surface);

// Returns a direction, interpreted from the passed string.
Direction directionStringToEnum(std::string s);

// Returns the vector, rotated about the line defined by the origin and the line points.
FCoord3D rotateVector(FCoord3D vect, FCoord3D linePt, float degAngle);
FCoord3D rotateVectorIntoZ(FCoord3D vect, FCoord3D linePt);
FCoord3D rotateVectorZ(FCoord3D vect, float degAngle);
FCoord3D rotateVectorOutOfZ(FCoord3D vect, FCoord3D linePt);
FCoord3D rotateVectorX(FCoord3D vect, float degAngle);

#endif