#pragma once

#include "Hittable.h"
#include "Color.h"

class Material
{
public:
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& r, const HitRecord& rec, Color& attenuation, Ray& scattered) const
	{
		return false;
	}
};

class Lambertian : public Material
{
public: 
	Lambertian(const Color& albedo) : albedo(albedo) {}

	bool Scatter(const Ray& r, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
	{
		auto scatterDirection = rec.Normal + RandomUnitVector();

		if (scatterDirection.NearZero())
			scatterDirection = rec.Normal;

		scattered = Ray(rec.P, scatterDirection);
		attenuation = albedo;
		return true;
	}

private:
	Color albedo;
};

class Metal : public Material
{
public:
	Metal(const Color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
	{
		Vec3 reflected = Reflect(rIn.Direction(), rec.Normal);
		reflected = UnitVector(reflected) + (fuzz * RandomUnitVector());
		scattered = Ray(rec.P, reflected);
		attenuation = albedo;
		return (Dot(scattered.Direction(), rec.Normal) > 0);
	}

private:
	Color albedo;
	double fuzz;
};

class Dielectric : public Material
{
public:
	Dielectric(double refractionIndex) : refractionIndex(refractionIndex) {}

	bool Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
	{
		attenuation = Color(1.0, 1.0, 1.0);
		double ri = rec.FrontFace ? (1.0 / refractionIndex) : refractionIndex;

		Vec3 unitDirection = UnitVector(rIn.Direction());

		double cosTheta = std::fmin(Dot(-unitDirection, rec.Normal), 1.0);
		double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = ri * sinTheta > 1.0;
		Vec3 direction;

		if (cannotRefract || Reflectance(cosTheta, ri) > RandomDouble())
			direction = Reflect(unitDirection, rec.Normal);
		else
			direction = Refract(unitDirection, rec.Normal, ri);

		scattered = Ray(rec.P, direction);
		return true;
	}

private:
	double refractionIndex;

	static double Reflectance(double cosine, double refractionIndex) {
		auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};