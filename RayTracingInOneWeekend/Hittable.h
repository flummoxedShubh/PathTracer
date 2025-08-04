#pragma once

#include <memory>

#include "Ray.h"
#include "Interval.h"

class Material;

class HitRecord
{
public:
	Point3 P;
	Vec3 Normal;
	std::shared_ptr<Material> Mat;
	double T;
	bool FrontFace;

	void SetFaceNormal(const Ray& r, const Vec3& outwardNormal)
	{
		FrontFace = Dot(r.Direction(), outwardNormal) < 0;
		Normal = FrontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable
{
public:
	virtual ~Hittable() = default;

	virtual bool Hit(const Ray& r, Interval rayT, HitRecord& rec) const = 0;
};