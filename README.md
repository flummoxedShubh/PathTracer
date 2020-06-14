# Ray Tracer
A CPU based Ray Tracer (Path Tracer) written in C++, implemented following [RayTracingInOneWeekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) by Peter Shirley.

![alt text](https://github.com/flummoxedShubh/PathTracer/blob/master/raytracer.png)
**Twitter: @flummoxedShubh**
-------------
To compile, type in cmd:
```
compile
```
Code demonstration:
```cpp
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
```
