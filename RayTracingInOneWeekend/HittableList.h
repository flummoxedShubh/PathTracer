#pragma once

#include "Hittable.h"
#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable
{
public:
	std::vector<shared_ptr<Hittable>> objects;

	HittableList() {}
	HittableList(shared_ptr<Hittable> object) { Add(object); }

	void Clear() { objects.clear(); }
	void Add(shared_ptr<Hittable> object) { objects.push_back(object); }

	bool Hit(const Ray& r, Interval rayT, HitRecord& rec) const override
	{
		HitRecord tempRec;
		bool hitAnything = false;
		auto closestSoFar = rayT.Max;

		for (const auto& object : objects)
		{
			if (object->Hit(r, Interval(rayT.Min, closestSoFar), tempRec))
			{
				hitAnything = true;
				closestSoFar = tempRec.T;
				rec = tempRec;
			}
		}

		return hitAnything;
	}
};