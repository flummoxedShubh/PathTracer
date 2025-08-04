#pragma once

#include "Hittable.h"

class Sphere : public Hittable
{
public:
	Sphere(const Point3& center, double radius, std::shared_ptr<Material> mat) : center(center), radius(std::fmax(0.0, radius)), mat(mat) {}

	bool Hit(const Ray& r, Interval rayT, HitRecord& rec) const override
	{
		Vec3 oc = center - r.Origin();
		auto a = r.Direction().LengthSquared();
		auto h = Dot(r.Direction(), oc);
		auto c = oc.LengthSquared() - radius * radius;

		auto discriminant = h * h - a * c;
		if (discriminant < 0) return false;

		auto sqrtd = std::sqrt(discriminant);

		auto root = (h - sqrtd) / a;
		if (!rayT.Surrounds(root))
		{
			root = (h + sqrtd) / a;
			if (!rayT.Surrounds(root))
			{
				return false;
			}
		}

		rec.T = root;
		rec.P = r.At(rec.T);
		Vec3 outwardNormal = (rec.P - center) / radius;
		rec.SetFaceNormal(r, outwardNormal);
		rec.Mat = mat;

		return true;
	}

private:
	Point3 center;
	double radius;
	std::shared_ptr<Material> mat;
};