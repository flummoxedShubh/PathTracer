#pragma once

extern const double Infinity;

class Interval
{
public:
	double Min, Max;
	static const Interval Empty, Universe;

	Interval() : Min(+Infinity), Max(-Infinity) {}
	Interval(double min, double max) : Min(min), Max(max) {}

	double Size() const 
	{ 
		return Max - Min; 
	}

	bool Contains(double x) const 
	{
		return Min <= x && x <= Max; 
	}

	bool Surrounds(double x) const 
	{
		return Min < x && x < Max; 
	}

	double Clamp(double x) const
	{
		if (x < Min) return Min;
		if (x > Max) return Max;
		return x;
	}
};

const Interval Interval::Empty = Interval();
const Interval Interval::Universe = Interval();