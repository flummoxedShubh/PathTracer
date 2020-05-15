#include <stdio.h>
#include <stdlib.h>
#include "vec3.h"
#include "ray.h"

#define MAX_VAL 255

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return -1.0;
    } 
    else {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

color ray_color(const ray&r) {
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    if(t > 0.0){
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5*(unit_direction.y() + 1.0); // Scaling -1 <= y <= 1 to (0 to 1)
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0); // Linear Interpolation (White when 0) (Blue when 1)
}

int main()
{
    const int image_width = 1920;
    const int image_height = 1080;

    FILE* file_ptr = fopen("img.ppm", "w");

    fprintf(file_ptr, "P3\n%i %i\n%i\n", image_width, image_height, MAX_VAL);

    point3 origin(0.0, 0.0, 0.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.25, 0.0);
    point3 lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0,0,1);

    for(int rows = image_height-1; rows >= 0; --rows){
        for(int columns = 0; columns < image_width; ++columns){
            auto u = double(columns)/(image_width-1);    
            auto v = double(rows)/(image_height-1); 
            ray r(origin, lower_left_corner + u*horizontal + v*vertical);
            color pixel_color = ray_color(r);
            //write color
            int ir = static_cast<int>(255.999 * pixel_color.x());
            int ig = static_cast<int>(255.999 * pixel_color.y());
            int ib = static_cast<int>(255.999 * pixel_color.z());
            fprintf(file_ptr, "%i %i %i ", ir, ig, ib);
        }
        fprintf(file_ptr, "\n");
    }

    fclose(file_ptr);

    return 0;
}