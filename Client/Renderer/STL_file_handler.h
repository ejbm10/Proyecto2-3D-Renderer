

#ifndef STL_FILE_HANDLER_H
#define STL_FILE_HANDLER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <GL/glut.h>

#define STL_HEADER_SIZE 80
#define STL_TRIANGLE_SIZE 50


#define MAX_VERTICES 100000

typedef struct {
    float normal[3];
    float vertices[3][3];
    unsigned short attribute_byte_count;
} Triangle_;


// Load STL file in binary format
int load_stl(const char *filename, Triangle_ **triangles, unsigned int *triangle_count);

// Offset triangles for non-overlapping arrangement
void offset_triangles(Triangle_ *triangles, unsigned int count, float x_offset, float y_offset, float z_offset);

// Write the combined STL file
void write_combined_stl(const char *filename, Triangle_ *all_triangles, unsigned int total_triangle_count);





#endif //STL_FILE_HANDLER_H
