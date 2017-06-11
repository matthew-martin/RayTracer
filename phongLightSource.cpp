#include "phongLightSource.h"

PhongLightSource::PhongLightSource()
{
	color = RGB(1, 1, 1);
	intensity = 10.0;
	position = FCoord3D(0, -100, 0);
}

PhongLightSource::PhongLightSource(RGB _color, float _intensity, FCoord3D _position)
{
	color = _color;
	intensity = _intensity;
	position = _position;
}

void PhongLightSource::read(std::istream& s)
{
	float f;
	
	color.read(s);
	
	s >> f;
	intensity = f;
	
	position.read(s);
}

void PhongLightSource::write(std::ostream& s)
{
	color.write(s);
	s << intensity << std::endl;
	position.write(s);
}