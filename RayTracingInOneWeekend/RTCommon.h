#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

using std::make_shared;
using std::shared_ptr;

const double Infinity = std::numeric_limits<double>::infinity();
const double Pi = 3.1415926535897932385;

inline double DegreesToRadians(double degrees)
{
	return degrees * Pi / 180.0;
}

inline double RandomDouble()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double RandomDouble(double min, double max)
{
	return min + (max - min) * RandomDouble();
}

#include "Color.h"
#include "Ray.h"
#include "Vec3.h"
#include "Interval.h"