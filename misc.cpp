#include "misc.h"

#include <math.h>

RGB::RGB()
{
	red = green = blue = 0.0;
}

RGB::RGB(float r, float g, float b)
{
	red = r;
	green = g;
	blue = b;
}

RGB RGB::scale(float f)
{
	return RGB(red * f, green * f, blue * f);
}

RGB RGB::add(RGB other)
{
	return RGB(red + other.red, green + other.green, blue + other.blue);
}

int RGB::intensity(int numLevels)
{
	assert(numLevels > 0);
	
	float maximum = red;
	if (green > maximum) maximum = green;
	if (blue > maximum) maximum = blue;
	
	//return std::round(maximum * (numLevels - 1));
	int result = std::floor(maximum * numLevels);
	if (result > 9) result = 9;
	return result;
}

void RGB::read(std::istream& s)
{
	s >> red;
	s >> green;
	s >> blue;
}

void RGB::write(std::ostream& s)
{
	s << red << " " << green << " " << blue << std::endl;
}

bool RGB::isDark()
{
	return ((red + green + blue) / 3.0) < 0.5;
}

RGB RGB::intRGB(int r, int g, int b)
{
	return RGB((float)r / 255.0, (float)g / 255.0, (float)b / 255.0);
}

RGB RGB::random()
{
	return intRGB(rand() % 256, rand() % 256, rand() % 256);
}

RegionCode::RegionCode()
{
	for (int i = 0; i < 4; i++)
	{
		code[i] = 0;
	}
}

RegionCode::RegionCode(bool left, bool right, bool bottom, bool top)
{
	code[0] = left;
	code[1] = right;
	code[2] = bottom;
	code[3] = top;
}

RegionCode::RegionCode(FCoord coord, float xMin, float xMax, float yMin, float yMax)
{
	code[0] = coord.x < xMin;
	code[1] = coord.x > xMax;
	code[2] = coord.y < yMin;
	code[3] = coord.y > yMax;
}

bool RegionCode::getValue(int dir)
{
	if (dir < 0 || dir >= 4) return false;
	return code[dir];
}

bool RegionCode::isZero()
{
	return (code[0] == 0 && code[1] == 0 && code[2] == 0 && code[3] == 0);
}

int RegionCode::diffBit(RegionCode other)
{
	for (int i = 0; i < 4; i++)
	{
		if (code[i] != other.code[i]) return i;
	}
	return -1;
}

RegionCode RegionCode::bitAnd(RegionCode other)
{
	return RegionCode(code[0] && other.code[0],
		code[1] && other.code[1],
		code[2] && other.code[2], 
		code[3] && other.code[3]
	);
}

RegionCode RegionCode::bitOr(RegionCode other)
{
	return RegionCode(code[0] || other.code[0],
		code[1] || other.code[1],
		code[2] || other.code[2], 
		code[3] || other.code[3]
	);
}

Coord::Coord()
{
	x = 0;
	y = 0;
}

Coord::Coord(int xIn, int yIn)
{
	x = xIn;
	y = yIn;
}

void Coord::print()
{
	std::cout << "(" << x << ", " << y << ")";
}

FCoord Coord::toFCoord()
{
	return FCoord((float)x, (float)y);
}


FCoord::FCoord()
{
	x = 0.0;
	y = 0.0;
}

FCoord::FCoord(float xIn, float yIn)
{
	x = xIn;
	y = yIn;
}

void FCoord::print()
{
	std::cout << "(" << x << ", " << y << ")";
}

Coord FCoord::toCoord()
{
	return Coord(std::round(x), std::round(y));
}

FCoord FCoord::multiply(float f)
{
	return FCoord(x * f, y * f);
}

FCoord FCoord::plus(FCoord other)
{
	return FCoord(x + other.x, y + other.y);
}



FCoord3D::FCoord3D()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

FCoord3D::FCoord3D(float xIn, float yIn, float zIn)
{
	x = xIn;
	y = yIn;
	z = zIn;
}

float FCoord3D::length()
{
	return sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
}

FCoord3D FCoord3D::makeUnit()
{
	return FCoord3D(x / length(), y / length(), z / length());
}

FCoord3D FCoord3D::negate()
{
	return FCoord3D(-x, -y, -z);
}

FCoord3D FCoord3D::multiply(float f)
{
	return FCoord3D(x * f, y * f, z * f);
}

FCoord3D FCoord3D::plus(FCoord3D other)
{
	return FCoord3D(
		x + other.x,
		y + other.y,
		z + other.z
	);
}

FCoord3D FCoord3D::minus(FCoord3D other)
{
	return FCoord3D(
		x - other.x,
		y - other.y,
		z - other.z
	);
}

float FCoord3D::dotProduct(FCoord3D other)
{
	return x * other.x + y * other.y + z * other.z;
}

FCoord3D FCoord3D::crossProduct(FCoord3D other)
{
	return FCoord3D(
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x
	);
}

void FCoord3D::print()
{
	// std::cout << "(" << (x == 0.0 ? 0 : x) << ", " << (y == 0.0 ? 0 : x) << ", " << (z == 0.0 ? 0 : z) << ")";
	std::cout << "(" << x << ", " << y << ", " << z << ")";
}

void FCoord3D::read(std::istream& s)
{
	s >> x;
	s >> y;
	s >> z;
}

void FCoord3D::write(std::ostream& s)
{
	s << x << " " << y << " " << z << std::endl;
}


SurfaceIndices::SurfaceIndices()
{
	a = -1;
	b = -1;
	c = -1;
}

SurfaceIndices::SurfaceIndices(int _a, int _b, int _c)
{
	a = _a;
	b = _b;
	c = _c;
}

bool SurfaceIndices::contains(int pt)
{
	return (a == pt || b == pt || c == pt);
}


Surface::Surface()
{
	a = FCoord3D();
	b = FCoord3D();
	c = FCoord3D();
}

Surface::Surface(FCoord3D _a, FCoord3D _b, FCoord3D _c)
{
	a = _a;
	b = _b;
	c = _c;
}

FCoord3D Surface::getNormal()
{
	FCoord3D v1 = a.minus(c);
	FCoord3D v2 = b.minus(c);
	return v1.crossProduct(v2).makeUnit();
}

void Surface::print()
{
	a.print();
	std::cout << " ";
	b.print();
	std::cout << " ";
	c.print();
	std::cout << std::endl;
}


PhongData::PhongData()
{
	fromPoint = FCoord3D(0.5, -5.0, 0.5);
	lightSource = FCoord3D(1.0, -10.0, 0.0);
	
	ambientIntensity = 0.2;
	ambientColorCoefficient = RGB(1.0, 1.0, 0.2);
	
	lightSourceIntensity = 4;
	specularColorCoefficient = RGB(1.0, 1.0, 1.0);
	
	C = 0.7;
	n = 3;
}



float findYIntercept(FCoord p1, FCoord p2, float x)
{
	assert(p1.x != p2.x);
	
	// Ensure that p1 is on the left.
	if (p2.x < p1.x)
	{
		FCoord temp = p1;
		p1 = p2;
		p2 = temp;
	}
	
	return p1.y + ((p2.y - p1.y) / (p2.x - p1.x)) * (x - p1.x); // yLeft + m(x - xLeft)
}

float findXIntercept(FCoord p1, FCoord p2, float y)
{
	assert(p1.y != p2.y);
	
	// Ensure that p1 is on the bottom.
	if (p2.y < p1.y)
	{
		FCoord temp = p1;
		p1 = p2;
		p2 = temp;
	}
	
	return p1.x + ((p2.x - p1.x) / (p2.y - p1.y)) * (y - p1.y); // xLeft + m(y - yLeft)
}

FCoord findIntercept(FCoord l1p1, FCoord l1p2, FCoord l2p1, FCoord l2p2)
{
	// Special cases where either line is vertical.
	if (l1p1.x == l1p2.x)
	{ // Line 1 is vertical.
		return FCoord(l1p1.x, findYIntercept(l2p1, l2p2, l1p1.x));
	}
	else if (l2p1.x == l2p2.x)
	{ // Line 2 is vertical.
		return FCoord(l2p1.x, findYIntercept(l1p1, l1p2, l2p1.x));
	}
	
	// Find the slope of each line.
	double slope1 = ((double)l1p1.y - (double)l1p2.y) / ((double)l1p1.x - (double)l1p2.x);
	double slope2 = ((double)l2p1.y - (double)l2p2.y) / ((double)l2p1.x - (double)l2p2.x);
	assert (slope1 != slope2);
	
	// Find the y-axis intercept of each line.
	double yAxisInt1 = (double)findYIntercept(l1p1, l1p2, 0.0);
	double yAxisInt2 = (double)findYIntercept(l2p1, l2p2, 0.0);
	
	// x-intercept is (b2-b1)/(m1-m2), from solving the system y=m1*x+b1, y=m2*x+b2.
	double xInt = (yAxisInt2 - yAxisInt1) / (slope1 - slope2);
	// y-intercept is y=m*x+b for either line. Arbitrarily choose the first line.
	double yInt = (slope1 * xInt) + yAxisInt1;
	
	return FCoord((float)xInt, (float)yInt);
}

bool pointsSameSide(FCoord lp1, FCoord lp2, FCoord point1, FCoord point2)
{
	if (lp1.x == lp2.x)
	{ // Vertical line.
		return ((point1.x >= lp1.x && point2.x >= lp1.x) || (point1.x <= lp1.x && point2.x <= lp1.x));
	}
	
	double slope = (((double)lp1.y - (double)lp2.y) / ((double)lp1.x - (double)lp2.x));
	double yInt = (double)findYIntercept(lp1, lp2, 0.0);
	
	bool p1Above = (slope * (double)point1.x) + yInt < (double)point1.y;
	bool p2Above = (slope * (double)point2.x) + yInt < (double)point2.y;
	
	return (p1Above == p2Above);
}

bool inScanline(Coord p1, Coord p2, int scanline)
{
	if (p1.y > p2.y)
	{
		if (scanline <= p1.y && scanline >= p2.y) return true;
	}
	else
	{
		if (scanline <= p2.y && scanline >= p1.y) return true;
	}
	return false;
}

int findMinPos(int a, int b, int c)
{
	int result = a;
	int ind = 1;
	
	if (result < 0)
	{
		result = b;
		ind = 2;
	}
	else
	{
		if (b >= 0)
		{
			if (b < result)
			{
				result = b;
				ind = 2;
			}
		}
	}
	
	if (result < 0)
	{
		result = c;
		ind = 3;
	}
	else
	{
		if (c >= 0)
		{
			if (c < result)
			{
				result = c;
				ind = 3;
			}
		}
	}
	
	return ind;
}

int findMaxPos(int a, int b, int c)
{
	int result = a;
	int ind = 1;
	
	if (result < 0)
	{
		result = b;
		ind = 2;
	}
	else
	{
		if (b >= 0)
		{
			if (b > result)
			{
				result = b;
				ind = 2;
			}
		}
	}
	
	if (result < 0)
	{
		result = c;
		ind = 3;
	}
	else
	{
		if (c >= 0)
		{
			if (c > result)
			{
				result = c;
				ind = 3;
			}
		}
	}
	
	return ind;
}

RGB interpolateColorY(Coord p1, RGB c1, Coord p2, RGB c2, int y)
{
	// Ensure that p1 is on top (higher y-value).
	if (p1.y < p2.y)
	{
		Coord tempP;
		RGB tempC;
		
		tempP = p1;
		tempC = c1;
		p1 = p2;
		c1 = c2;
		p2 = tempP;
		c2 = tempC;
	}
	assert(p1.y != p2.y);
	float c1Weight = ((float)(y - p2.y)) / ((float)(p1.y - p2.y));
	float c2Weight = ((float)(p1.y - y)) / ((float)(p1.y - p2.y));
	return RGB(
		c1.red * c1Weight + c2.red * c2Weight,
		c1.green * c1Weight + c2.green * c2Weight,
		c1.blue * c1Weight + c2.blue * c2Weight
	);
}

RGB interpolateColorX(Coord p1, RGB c1, Coord p2, RGB c2, int x)
{
	// Ensure that p1 is to the right (higher x-value).
	if (p1.x < p2.x)
	{
		Coord tempP;
		RGB tempC;
		
		tempP = p1;
		tempC = c1;
		p1 = p2;
		c1 = c2;
		p2 = tempP;
		c2 = tempC;
	}
	assert(p1.x != p2.x);
	float c1Weight = ((float)(x - p2.x)) / ((float)(p1.x - p2.x));
	float c2Weight = ((float)(p1.x - x)) / ((float)(p1.x - p2.x));
	return RGB(
		c1.red * c1Weight + c2.red * c2Weight,
		c1.green * c1Weight + c2.green * c2Weight,
		c1.blue * c1Weight + c2.blue * c2Weight
	);
}

float burnsteinBezierPolynomial(int i, int n, float t)
{
	return (float)nChooseK(n, i) * pow(1.0 - t, n - i) * pow(t, i);
}

int nChooseK(int n, int k)
{
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int result = n;
    for( int i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}

bool rayIntersectsSurface(FCoord3D p0, FCoord3D d, Surface surface, float &t)
{
	FCoord3D surfaceNormal = surface.getNormal();
	
	float A = surfaceNormal.x;
	float B = surfaceNormal.y;
	float C = surfaceNormal.z;
	float D = -((A * surface.a.x) + (B * surface.a.y) + (C * surface.a.z));
	
	float denom = (A * d.x) + (B * d.y) + (C * d.z);
	if (denom == 0.0)
	{ // The line lies in the plane of the surface. Count this as non-intersecting.
		return false;
	}
	
	float tTemp = -((A * p0.x) + (B * p0.y) + (C * p0.z) + D) / denom;
	FCoord3D intersectPoint = p0.plus(d.multiply(tTemp));
	
	FCoord3D d0 = surface.a.minus(intersectPoint);
	FCoord3D d1 = surface.b.minus(intersectPoint);
	FCoord3D d2 = surface.c.minus(intersectPoint);
	float D0 = (d0.x * d1.y) - (d0.x * d1.z) - (d0.y * d1.x) + (d0.y * d1.z) + (d0.z * d1.x) - (d0.z * d1.y);
	float D1 = (d1.x * d2.y) - (d1.x * d2.z) - (d1.y * d2.x) + (d1.y * d2.z) + (d1.z * d2.x) - (d1.z * d2.y);
	float D2 = (d2.x * d0.y) - (d2.x * d0.z) - (d2.y * d0.x) + (d2.y * d0.z) + (d2.z * d0.x) - (d2.z * d0.y);
	
	if (((D0 <= 0.0 && D1 <= 0.0 && D2 <= 0.0) || (D0 >= 0.0 && D1 >= 0.0 && D2 >= 0.0)) && tTemp > 0)
	{
		t = tTemp;
		return true;
	}
	else
	{
		return false;
	}
}

bool lineSegmentIntersectsSurface(FCoord3D p0, FCoord3D p1, Surface surface)
{
	FCoord3D dir = p1.minus(p0).makeUnit();
	
	float dummy;
	return (rayIntersectsSurface(p0, dir, surface, dummy) && rayIntersectsSurface(p1, dir.negate(), surface, dummy));
}

Direction directionStringToEnum(std::string s)
{
	if (s.length() == 0) return dBackward;
	
	if (s.at(0) == 'l') return dLeft;
	if (s.at(0) == 'r') return dRight;
	if (s.at(0) == 'u') return dUp;
	if (s.at(0) == 'd') return dDown;
	if (s.at(0) == 'f') return dForward;
	if (s.at(0) == 'b') return dBackward;
	
	return dBackward;
}

FCoord3D rotateVector(FCoord3D vect, FCoord3D linePt, float degAngle)
{
	std::cout << "vect = ";
	vect.print();
	std::cout << std::endl;
	
	std::cout << "linePt = ";
	linePt.print();
	std::cout << std::endl;
	
	
	if (sqrt(pow(linePt.y, 2.0) + pow(linePt.z, 2.0)) == 0.0)
	{
		vect = rotateVectorX(vect, linePt.x > 0.0 ? degAngle : -degAngle);
	}
	else
	{
		vect = rotateVectorIntoZ(vect, linePt);
		vect = rotateVectorZ(vect, degAngle);
		vect = rotateVectorOutOfZ(vect, linePt);
	}
	
	return vect;
}

FCoord3D rotateVectorIntoZ(FCoord3D vect, FCoord3D linePt)
{
	float l = sqrt(pow(linePt.y, 2.0) + pow(linePt.z, 2.0));
	
	FCoord3D temp = vect;
	vect.x = (l) * temp.x + (-linePt.x) * temp.z;
	vect.y = (-linePt.x * linePt.y / l) * temp.x + (linePt.z / l) * temp.y + (-linePt.y) * temp.z;
	vect.z = (linePt.x * linePt.z / l) * temp.x + (linePt.y / l) * temp.y + (linePt.z) * temp.z;
	
	return vect;
}

FCoord3D rotateVectorZ(FCoord3D vect, float degAngle)
{
	float radAngle = degAngle * M_PI / 180;
	
	float ca = cos(radAngle);
	float sa = sin(radAngle);
	float newX, newY;
	
	float x = vect.x;
	float y = vect.y;
	newX = (x * ca) - (y * sa);
	newY = (x * sa) + (y * ca); 
	
	FCoord3D result = vect;
	result.x = newX;
	result.y = newY;
	
	return result;
}

FCoord3D rotateVectorOutOfZ(FCoord3D vect, FCoord3D linePt)
{
	float l = sqrt(pow(linePt.y, 2.0) + pow(linePt.z, 2.0));
	float lsqxsq = pow(linePt.x, 2.0) + pow(linePt.y, 2.0) + pow(linePt.z, 2.0);
	float ysqzsq = pow(linePt.y, 2.0) + pow(linePt.z, 2.0);
	
	FCoord3D temp = vect;
	
	vect.x = (l / lsqxsq) * temp.x +
		(-l * linePt.x * linePt.y / (lsqxsq * ysqzsq)) * temp.y +
		(l * linePt.x * linePt.z / (lsqxsq * ysqzsq)) * temp.z;
	vect.y = (l * linePt.z / ysqzsq) * temp.y +
		(l * linePt.y / ysqzsq) * temp.z;
	vect.z = (-linePt.x / lsqxsq) * temp.x +
		(-pow(l, 2.0) * linePt.y / (lsqxsq * ysqzsq)) * temp.y + 
		(pow(l, 2.0) * linePt.z / (lsqxsq * ysqzsq)) * temp.z;
	
	return vect;
}

FCoord3D rotateVectorX(FCoord3D vect, float degAngle)
{
	float radAngle = degAngle * M_PI / 180;
	
	float ca = cos(radAngle);
	float sa = sin(radAngle);
	float newY, newZ;
	
	float y = vect.y;
	float z = vect.z;
	newY = (y * ca) - (z * sa);
	newZ = (y * sa) + (z * ca); 
	
	FCoord3D result = vect;
	result.y = newY;
	result.z = newZ;
	
	return result;
}