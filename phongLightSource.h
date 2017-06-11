#ifndef __PHONGLIGHTSOURCE_H__
#define __PHONGLIGHTSOURCE_H__

/* phongLightSource.h
 * 
 * Defines all the parameters of a phong lighting model light source.
 * 
 */

#include "misc.h"

struct PhongLightSource
{
	// Constructors.
	PhongLightSource();
	PhongLightSource(RGB _color, float _intensity, FCoord3D _position);
	
	// Reads the light source data from the stream.
	void read(std::istream& s);
	// Writes the light source data to the stream.
	void write(std::ostream& s);
	
	// The color of the light source.
	RGB color;
	// How bright the light source is.
	float intensity;
	// The world position of the light source.
	FCoord3D position;
};

#endif