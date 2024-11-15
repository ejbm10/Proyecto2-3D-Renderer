

#include "STL_file_handler.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>


// Function to load STL file in binary format
int load_stl(const char *filename, Triangle_ **triangles, unsigned int *triangle_count) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    fseek(file, STL_HEADER_SIZE, SEEK_SET);  // Skip the header
    fread(triangle_count, sizeof(unsigned int), 1, file);

    *triangles = malloc(*triangle_count * sizeof(Triangle_));
    fread(*triangles, STL_TRIANGLE_SIZE, *triangle_count, file);
    fclose(file);

    return 0;
}

// Function to offset triangles to avoid overlap
void offset_triangles(Triangle_ *triangles, unsigned int count, float x_offset, float y_offset, float z_offset) {
    for (unsigned int i = 0; i < count; ++i) {
        for (int v = 0; v < 3; ++v) {  // Each vertex of the triangle
            triangles[i].vertices[v][0] += x_offset;
            triangles[i].vertices[v][1] += y_offset;
            triangles[i].vertices[v][2] += z_offset;
        }
    }
}

// Function to write the merged STL file
void write_combined_stl(const char *filename, Triangle_ *all_triangles, unsigned int total_triangle_count) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open output file");
        return;
    }

    char header[STL_HEADER_SIZE] = {0};
    fwrite(header, STL_HEADER_SIZE, 1, file);  // Write header

    fwrite(&total_triangle_count, sizeof(unsigned int), 1, file);  // Number of triangles
    fwrite(all_triangles, STL_TRIANGLE_SIZE, total_triangle_count, file);

    fclose(file);
}




