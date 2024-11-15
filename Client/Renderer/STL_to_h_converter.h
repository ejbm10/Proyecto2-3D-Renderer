

#ifndef STL_TO_H_CONVERTER_H
#define STL_TO_H_CONVERTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>



typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    uint16_t v1, v2, v3;
} Triangle;

typedef struct {
    float x, y, z;
} Normal;

typedef struct {
    Vertex* vertices;
    Triangle* triangles;
    Normal* normals;
    int vertexCount;
    int triangleCount;
} Mesh;

Mesh readStl(const char* filename);
void scaleMesh(Mesh* mesh);
void writeHeaderFile(const Mesh* mesh, const char* filename);




#endif //STL_TO_H_CONVERTER_H
