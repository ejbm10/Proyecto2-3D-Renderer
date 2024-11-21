
#ifndef ASCII_STL_MERGER_H
#define ASCII_STL_MERGER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#pragma pack(push, 1)  // Ensure proper packing of the struct

// Define the structure for the binary STL header
typedef struct {
    char header[80];      // 80-byte header
    unsigned int num_triangles;  // Number of triangles in the file
} STLHeader;

// Define the structure for a single triangle in the STL file
typedef struct {
    float normal[3];      // Normal vector (not used here, but part of the file format)
    float vertices[3][3]; // 3 vertices, each with 3 float values (x, y, z)
    unsigned short attribute_byte_count;  // Usually 0 (not used here)
} Triangle_;


int load_stl(const char *filename, STLHeader *header, Triangle_ **triangles);
int save_stl(const char *filename, STLHeader *header, Triangle_ *triangles);
void calculate_bounding_box(const Triangle_ *triangles, unsigned int num_triangles, float *min_x, float *max_x);
int combine_meshes(const char *output_filename, const char **file_list, int num_files);


#endif //ASCII_STL_MERGER_H
