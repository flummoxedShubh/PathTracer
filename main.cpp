#include "common_utility.h"

#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>
#define MAX_VAL 255

color ray_color(const ray&r, const hittable& world, int depth) {
    hit_record rec;
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);
    
    if(world.hit(r, 0.001, infinity, rec)){ //if ray hits (tmin = 0.001 not working, overflows the stack)
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }
    vec3 unit_direction = unit_vector(r.direction()); //if ray doesn't hit
    auto t = 0.5*(unit_direction.y() + 1.0); // Scaling -1 <= y <= 1 to (0 to 1)
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0); // Linear Interpolation (White when 0) (Blue when 1)
}

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main()
{
    
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 1920;
    const int image_height = 1080;
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    FILE* file_ptr = fopen("img.ppm", "w");

    fprintf(file_ptr, "P3\n%i %i\n%i\n", image_width, image_height, MAX_VAL);

    auto world = random_scene();

    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    for(int rows = image_height-1; rows >= 0; --rows){
        for(int columns = 0; columns < image_width; ++columns){
        
            color pixel_color(0, 0, 0);
            for(int s = 0; s < samples_per_pixel; ++s){
                auto u = (columns + random_double()) / (image_width-1);
                auto v = (rows + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }

            //write color
            //---------------------------------------------------
            auto ir = pixel_color.x();
            auto ig = pixel_color.y();
            auto ib = pixel_color.z();
            
            // Divide the color total by the number of samples and gamma-correct for gamma=2.0.
            auto scale = 1.0 / samples_per_pixel;
            ir = sqrt(scale * ir);
            ig = sqrt(scale * ig);
            ib = sqrt(scale * ib);

            fprintf(file_ptr, "%i %i %i ", static_cast<int>(256 * clamp(ir, 0.0, 0.999)), 
            static_cast<int>(256 * clamp(ig, 0.0, 0.999)), 
            static_cast<int>(256 * clamp(ib, 0.0, 0.999)));
            //---------------------------------------------------
        }
        fprintf(file_ptr, "\n");
    }

    fclose(file_ptr);

    return 0;
}