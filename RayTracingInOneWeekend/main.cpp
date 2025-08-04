#include <fstream>
#include <thread>
#include "Camera.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"

int main()
{
    HittableList world;

    auto groundMaterial = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto chooseMat = RandomDouble();
            Point3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

            if ((center - Point3(4, 0.2, 0)).Length() > 0.9) {
                shared_ptr<Material> sphereMaterial;

                if (chooseMat < 0.8) {
                    auto albedo = Color::Random() * Color::Random();
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
                else if (chooseMat < 0.95) {
                    auto albedo = Color::Random(0.5, 1);
                    auto fuzz = RandomDouble(0, 0.5);
                    sphereMaterial = make_shared<Metal>(albedo, fuzz);
                    world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
                else {
                    sphereMaterial = make_shared<Dielectric>(1.5);
                    world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.Add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.Add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.Add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    Camera cam;

    cam.AspectRatio = 16.0 / 9.0;
    cam.ImageWidth = 1200;
    cam.SamplesPerPixel = 500;
    cam.MaxDepth = 50;

    cam.VFov = 20;
    cam.LookFrom = Point3(13, 2, 3);
    cam.LookAt = Point3(0, 0, 0);
    cam.VUp = Vec3(0, 1, 0);

    cam.DefocusAngle = 0.6;
    cam.FocusDist = 10.0;

    cam.Render(world);

	return 0;
}
