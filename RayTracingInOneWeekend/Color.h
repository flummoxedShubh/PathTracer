#pragma once

#include <sstream>

#include "Vec3.h"
#include "Interval.h"

using Color = Vec3;

inline double LinearToGamma(double linearComponent)
{
	if (linearComponent > 0)
		return std::sqrt(linearComponent);

	return 0;
}

void WriteColor(std::stringstream& stream, const Color& pixelColor)
{
	auto r = pixelColor.x();
	auto g = pixelColor.y();
	auto b = pixelColor.z();

	r = LinearToGamma(r);
	g = LinearToGamma(g);
	b = LinearToGamma(b);

	static const Interval intensity(0.000, 0.999);
	int rByte = int(256 * intensity.Clamp(r));
	int gByte = int(256 * intensity.Clamp(g));
	int bByte = int(256 * intensity.Clamp(b));

	stream << rByte << " " << gByte << " " << bByte << "\n";
}
