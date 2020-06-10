#include "common_utility.h"

#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>
#define MAX_VAL 255

color ray_color(const ray&r, const hittable& world, int depth) {
    hit_record rec;
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);
    
    if(world.hit(r, 0, infinity, rec)){ //if ray hits (tmin = 0.001 not working, overflows the stack)
        point3 target = rec.p + rec.normal + random_unit_vector();
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth);
    }
    vec3 unit_direction = unit_vector(r.direction()); //if ray doesn't hit
    auto t = 0.5*(unit_direction.y() + 1.0); // Scaling -1 <= y <= 1 to (0 to 1)
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0); // Linear Interpolation (White when 0) (Blue when 1)
}

int main()
{
    //Aspect Ratio 16:9
    const int image_width = 1920;
    const int image_height = 1080;
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    FILE* file_ptr = fopen("img.ppm", "w");

    fprintf(file_ptr, "P3\n%i %i\n%i\n", image_width, image_height, MAX_VAL);

    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    camera cam;

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