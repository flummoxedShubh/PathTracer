#pragma once

#include <fstream>
#include <thread>
#include <vector>
#include <sstream>
#include <atomic>
#include <cstdlib>
#include "Hittable.h"
#include "Material.h"
#include "RTCommon.h"

class Camera
{
public:
	double AspectRatio = 16.0 / 9.0;
	int ImageWidth = 512;
	int SamplesPerPixel = 10;
	int MaxDepth = 10;

	double VFov = 90;
	Point3 LookFrom = Point3(0, 0, 0);
	Point3 LookAt = Point3(0, 0, -1);
	Point3 VUp = Point3(0, 1, 0);

	double DefocusAngle = 0;
	double FocusDist = 10;

	void Render(const Hittable& world)
	{
		Initialize();

		unsigned int threadCount = std::thread::hardware_concurrency();
		if (threadCount == 0) threadCount = 2;

		std::vector<std::thread> threads;
		int chunkSize = imageHeight / threadCount;

		std::vector<std::stringstream> streams(threadCount);

		for (int i = 0; i < threadCount; i++)
		{
			int start = i * chunkSize;
			int end = (i == threadCount - 1) ? imageHeight : start + chunkSize;
			threads.emplace_back([this, start, end, &stream = streams[i], &world]() { this->WritePixel(start, end, stream, world); });
		}

		for (auto& t : threads) t.join();

		std::ofstream file("Render.ppm");
		file << "P3\n" << ImageWidth << " " << imageHeight << "\n255\n";

		for (const auto& ss : streams)
		{
			file << ss.str();
		}
			
		std::cout << "\rdone...                          " << std::flush;

		file.close();
		
		std::system("start Render.ppm");
	}

private:
	int imageHeight;
	double pixelSampleScale;
	Point3 center;
	Point3 pixel00Loc;
	Vec3 pixelDeltaU;
	Vec3 pixelDeltaV;
	Vec3 u, v, w;
	Vec3 defocusDiskU;
	Vec3 defocusDiskV;

	std::atomic<int> remainingScanlines;

	void Initialize()
	{
		imageHeight = int(ImageWidth / AspectRatio);
		imageHeight = (imageHeight < 1) ? 1 : imageHeight;

		pixelSampleScale = 1.0 / SamplesPerPixel;

		center = LookFrom;

		// auto focalLength = (LookFrom - LookAt).Length();
		auto theta = DegreesToRadians(VFov);
		auto h = std::tan(theta / 2);
		auto viewportHeight = 2 * h * FocusDist;
		auto viewportWidth = viewportHeight * (double(ImageWidth) / imageHeight);

		w = UnitVector(LookFrom - LookAt);
		u = UnitVector(Cross(VUp, w));
		v = Cross(w, u);

		auto viewportU = viewportWidth * u;
		auto viewportV = viewportHeight * -v;

		pixelDeltaU = viewportU / ImageWidth;
		pixelDeltaV = viewportV / imageHeight;

		auto viewportUpperLeft = center - (FocusDist * w) - viewportU / 2 - viewportV / 2;
		pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

		auto defocusRadius = FocusDist * std::tan(DegreesToRadians(DefocusAngle / 2));
		defocusDiskU = u * defocusRadius;
		defocusDiskV = v * defocusRadius;

		remainingScanlines = imageHeight;
	}

	Ray GetRay(int i, int j)
	{
		auto offset = SampleSquare();
		auto pixelSample = pixel00Loc
			+ ((i + offset.x()) * pixelDeltaU)
			+ ((j + offset.y()) * pixelDeltaV);

		auto rayOrigin = (DefocusAngle <= 0) ? center : DefocusDiskSample();
		auto rayDirection = pixelSample - rayOrigin;

		return Ray(rayOrigin, rayDirection);
	}

	Vec3 SampleSquare() const
	{
		return Vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0);
	}

	Point3 DefocusDiskSample() const
	{
		auto p = RandomInUnitDisk();
		return center + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
	}

	Color RayColor(const Ray& r, int depth, const Hittable& world) const
	{
		if (depth <= 0)
			return Color(0, 0, 0);

		HitRecord rec;
		if (world.Hit(r, Interval(0.001, Infinity), rec))
		{
			Ray scattered;
			Color attenuation;
			if (rec.Mat->Scatter(r, rec, attenuation, scattered))
				return attenuation * RayColor(scattered, depth - 1, world);

			return Color(0, 0, 0);
		}

		Vec3 unitDirection = UnitVector(r.Direction());
		auto a = 0.5 * (unitDirection.y() + 1.0);
		return (1 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
	}

	void WritePixel(int start, int end, std::stringstream& stream, const Hittable& world)
	{
		for (int j = start; j < end; j++)
		{
			if (j % 10 == 0) {
				std::cout << "\rScanlines Remaining: " << remainingScanlines.load() << "   " << std::flush;
			}

			for (int i = 0; i < ImageWidth; i++)
			{
				Color pixelColor(0, 0, 0);
				for (int sample = 0; sample < SamplesPerPixel; sample++)
				{
					Ray r = GetRay(i, j);
					pixelColor += RayColor(r, MaxDepth, world);
				}
				WriteColor(stream, pixelSampleScale * pixelColor);
			}

			--remainingScanlines;
		}
	}
};