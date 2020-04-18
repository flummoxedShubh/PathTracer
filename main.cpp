#include <stdio.h>
#include <stdlib.h>

#define MAX_VAL 255

int main()
{
    const int image_width = 1920;
    const int image_height = 1080;

    FILE* file_ptr = fopen("img.ppm", "w");

    fprintf(file_ptr, "P3\n%i %i\n%i\n", image_width, image_height, MAX_VAL);

    for(int rows = image_height-1; rows >= 0; --rows){
        for(int columns = 0; columns < image_width; ++columns){
            auto r = double(columns) / image_width;
            auto g = double(rows) / image_height;
            auto b = 0.2;
            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);
            fprintf(file_ptr, "%i %i %i ", ir, ig, ib);
        }
        fprintf(file_ptr, "\n");
    }

    fclose(file_ptr);

    return 0;
}