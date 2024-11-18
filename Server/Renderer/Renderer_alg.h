

#ifndef RENDERER_ALG_H
#define RENDERER_ALG_H

#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct {
    float normal[3];
    float vertices[3][3];
    uint16_t attribute;
} STLTriangle;

void parseInput(const char* input);
void writeCubeToBinarySTL(GLfloat sideLength, const char *filename);
void writeSphereToBinarySTL(GLfloat radius, GLint slices, GLint stacks, const char *filename);
void writeConeToBinarySTL(GLfloat radius, GLfloat height, GLint slices, const char *filename);
void writePyramidToBinarySTL(GLfloat height, const char *filename);
void writeCylinderToBinarySTL(float radius, float length, int n, const char *filename);
void calculateNormalPrism(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], GLfloat normal[3]);
void writePrismToBinarySTL(float radius, float length, int n, const char *filename);
void stl_to_h_file(const char *filePath);
void process_partial_STL(int rank, int size, const char* input);

#endif //RENDERER_ALG_H
