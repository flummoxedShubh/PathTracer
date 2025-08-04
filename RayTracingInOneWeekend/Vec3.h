#pragma once

#include <cmath>
#include <ostream>

extern double RandomDouble();
extern double RandomDouble(double min, double max);

class Vec3
{
public:
	double E[3];

	Vec3() : E{0, 0, 0} {}
	Vec3(double e0, double e1, double e2) : E{e0, e1, e2} {}

	double x() const { return E[0]; }
	double y() const { return E[1]; }
	double z() const { return E[2]; }

	Vec3 operator-() const { return Vec3{-E[0], -E[1], -E[2]}; }
	double operator[](int i) const { return E[i]; }
	double& operator[](int i) { return E[i]; }

	Vec3& operator+=(const Vec3& v)
	{
		E[0] += v.E[0];
		E[1] += v.E[1];
		E[2] += v.E[2];

		return *this;
	}

	Vec3& operator*=(double t)
	{
		E[0] *= t;
		E[1] *= t;
		E[2] *= t;

		return *this;
	}

	Vec3& operator/=(double t)
	{
		return *this *= 1 / t;
	}

	double LengthSquared() const
	{
		return E[0] * E[0] + E[1] * E[1] + E[2] * E[2];
	}

	double Length() const
	{
		return std::sqrt(LengthSquared());
	}

	bool NearZero() const
	{
		auto s = 1e-8;
		return (std::fabs(E[0] < s)) && (std::fabs(E[1] < s)) && (std::fabs(E[2] < s));
	}

	static Vec3 Random()
	{
		return Vec3(RandomDouble(), RandomDouble(), RandomDouble());
	}

	static Vec3 Random(double min, double max)
	{
		return Vec3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
	}
};

using Point3 = Vec3;

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << v.E[0] << " " << v.E[1] << " " << v.E[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return Vec3{ u.E[0] + v.E[0], u.E[1] + v.E[1], u.E[2] + v.E[2] };
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return Vec3{ u.E[0] - v.E[0], u.E[1] - v.E[1], u.E[2] - v.E[2] };
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
	return Vec3(u.E[0] * v.E[0], u.E[1] * v.E[1], u.E[2] * v.E[2]);
}

inline Vec3 operator*(double t, const Vec3& v)
{
	return Vec3{ t * v.E[0], t * v.E[1], t * v.E[2] };
}

inline Vec3 operator*(const Vec3& v, double t)
{
	return Vec3{ v.E[0] * t , v.E[1] * t, v.E[2] * t };
}

inline Vec3 operator/(const Vec3& v, double t)
{
	return (1 / t) * v;
}

inline double Dot(const Vec3& u, const Vec3& v)
{
	return u.E[0] * v.E[0]
		+ u.E[1] * v.E[1]
		+ u.E[2] * v.E[2];
}

inline Vec3 Cross(const Vec3& u, const Vec3& v) {
	return Vec3(u.E[1] * v.E[2] - u.E[2] * v.E[1],
		u.E[2] * v.E[0] - u.E[0] * v.E[2],
		u.E[0] * v.E[1] - u.E[1] * v.E[0]);
}

inline Vec3 UnitVector(const Vec3& v)
{
	return v / v.Length();	
}

inline Vec3 RandomUnitVector()
{
	while (true)
	{
		auto p = Vec3::Random(-1, 1);
		auto lensq = p.LengthSquared();
		if (1e-160 < lensq && lensq <= 1)
			return p / sqrt(lensq);
	}
}

inline Vec3 RandomOnHemisphere(const Vec3& normal)
{
	Vec3 onUnitSphere = RandomUnitVector();
	if (Dot(onUnitSphere, normal) > 0.0)
		return onUnitSphere;
	else
		return -onUnitSphere;
}

inline Vec3 RandomInUnitDisk() {
	while (true) {
		auto p = Vec3(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
		if (p.LengthSquared() < 1)
			return p;
	}
}

inline Vec3 Reflect(const Vec3& v, const Vec3& n)
{
	return v - 2 * Dot(v, n) * n;
}

inline Vec3 Refract(const Vec3& uv, const Vec3& n, double etaiOverEtat)
{
	auto CosTheta = std::fmin(Dot(-uv, n), 1.0);
	Vec3 rOutPerp = etaiOverEtat * (uv + CosTheta * n);
	Vec3 rOutParallel = -std::sqrt(std::fabs(1.0 - rOutPerp.LengthSquared())) * n;
	return rOutPerp + rOutParallel;
}
