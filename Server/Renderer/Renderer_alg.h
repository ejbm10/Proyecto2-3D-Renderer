

#ifndef RENDERER_ALG_H
#define RENDERER_ALG_H

#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void parseInput(const char* input);
void drawSphere(GLfloat radius, GLint slices, GLint stacks);
void drawCone(GLfloat radius, GLfloat height, GLint slices);
void drawCube(GLfloat sideLength);
void drawPyramid(GLfloat height);
void drawCylinder(GLfloat radius, GLfloat height, GLint slices);
void drawPrism(float radius, float length, int n);
void display();
void timer(int value);
void reshape(int w, int h);

#endif //RENDERER_ALG_H