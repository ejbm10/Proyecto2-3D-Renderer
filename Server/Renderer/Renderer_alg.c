#include "Renderer_alg.h"

#include <GL/glut.h>
#include <math.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "Renderer_alg.h"
#include "STL_file_handler.h"
#include "STL_to_h_converter.h"

#define MAX_SHAPES 10
#define M_PI 3.14159265358979323846

// Angles for rotation
GLfloat cubeAngle = 0.0f;
GLfloat pyramidAngle = 0.0f;
GLfloat cylinderAngle = 0.0f;
GLfloat sphereAngle = 0.0f;
GLfloat coneAngle = 0.0f;
GLfloat prismAngle = 0.0f;

typedef struct {
    char shapeType[10];  // e.g., "sphere", "cube", "cylinder", "cone", "pyramid", "prism"
    GLfloat param1;      // e.g., radius for sphere or side length for cube
    GLfloat param2;      // e.g., height for cylinder or cone
    GLint slices;        // Number of slices for cylinder, cone, or sphere
    GLint stacks;        // Number of stacks for sphere
    GLint n;             // Number of sides for prism

    GLfloat position[3]; // x, y, z position
    GLfloat rotation[3]; // rotation angles (x, y, z)
} Shape;

Shape shapes[MAX_SHAPES];
int shapeCount = 0;

void parseInput(const char* input) {
    //printf("%s\n", input);
    const char* ptr = input;

    while (ptr != NULL) {
        char shapeType[20];
        GLfloat param1 = 0, param2 = 0;
        GLint n = 0;
        char paramBuffer[100];

        // Skip any leading spaces
        while (*ptr == ' ' || *ptr == '\n') ptr++;

        // Extract the shape name (e.g., sphere, cube, etc.)
        if (sscanf(ptr, "%s", shapeType) != 1) break;  // Stop if no more shapes

        // Skip the shape name and move to the parameters
        ptr = strchr(ptr, ' ') + 1;

        // Handle Sphere
        if (strcmp(shapeType, "sphere") == 0) {
            // Extract -radius value
            if (sscanf(ptr, "-radius=%f", &param1) == 1) {
                strcpy(shapes[shapeCount].shapeType, "sphere");
                shapes[shapeCount].param1 = param1;
                shapes[shapeCount].slices = 30;
                shapes[shapeCount].stacks = 30;
                shapes[shapeCount].position[0] = 0.0f;
                shapes[shapeCount].position[1] = 1.5f;
                shapes[shapeCount].position[2] = -8.0f;
                shapeCount++;
            }
        }

        // Handle Cube
        else if (strcmp(shapeType, "cube") == 0) {
            // Extract -side value
            if (sscanf(ptr, "-side=%f", &param1) == 1) {
                strcpy(shapes[shapeCount].shapeType, "cube");
                shapes[shapeCount].param1 = param1;
                shapes[shapeCount].position[0] = -2.5f;
                shapes[shapeCount].position[1] = 0.0f;
                shapes[shapeCount].position[2] = -8.0f;
                shapeCount++;
            }
        }
        // Handle Cylinder
        else if (strcmp(shapeType, "cylinder") == 0) {
            // Extract -radius and -height values
            GLfloat radius = 0, height = 0;
            if (sscanf(ptr, "-radius=%f -height=%f", &radius, &height) == 2) {
                strcpy(shapes[shapeCount].shapeType, "cylinder");
                shapes[shapeCount].param1 = radius;
                shapes[shapeCount].param2 = height;
                shapes[shapeCount].slices = 30;
                shapes[shapeCount].position[0] = 2.5f;
                shapes[shapeCount].position[1] = 0.0f;
                shapes[shapeCount].position[2] = -8.0f;
                shapeCount++;
            }
        }
        // Handle Cone
        else if (strcmp(shapeType, "cone") == 0) {
            // Extract -radius and -height values
            GLfloat radius = 0, height = 0;
            if (sscanf(ptr, "-radius=%f -height=%f", &radius, &height) == 2) {
                strcpy(shapes[shapeCount].shapeType, "cone");
                shapes[shapeCount].param1 = radius;
                shapes[shapeCount].param2 = height;
                shapes[shapeCount].slices = 30;
                shapes[shapeCount].position[0] = 0.0f;
                shapes[shapeCount].position[1] = -2.5f;
                shapes[shapeCount].position[2] = -8.0f;
                shapeCount++;
            }
        }
        // Handle Prism
        else if (strcmp(shapeType, "prism") == 0) {
            GLfloat radius = 0, length = 0;
            int sides = 0;
            if (sscanf(ptr, "-radius=%f -length=%f -sides=%d", &radius, &length, &sides) == 3) {
                strcpy(shapes[shapeCount].shapeType, "prism");
                shapes[shapeCount].param1 = radius;
                shapes[shapeCount].param2 = length;
                shapes[shapeCount].n = sides;
                shapes[shapeCount].position[0] = 0.0f;
                shapes[shapeCount].position[1] = 0.0f;
                shapes[shapeCount].position[2] = -8.0f;
                shapeCount++;
            }
        }
        // Handle Pyramid
        else if (strcmp(shapeType, "pyramid") == 0) {
            GLfloat height = 0;
            if (sscanf(ptr, "-height=%f", &height) == 1) {
                // Since the pyramid only has height, we'll assume the base is 1
                strcpy(shapes[shapeCount].shapeType, "pyramid");
                shapes[shapeCount].param1 = height;
                shapes[shapeCount].position[0] = 3.0f;
                shapes[shapeCount].position[1] = 1.0f;
                shapes[shapeCount].position[2] = -8.0f;
                shapeCount++;
            }
        }
        // Handle any unrecognized shape (error handling)
        else {
            printf("Error: Unrecognized shape '%s'. Skipping.\n", shapeType);
        }

        // Move to the next shape in the input string
        ptr = strchr(ptr, '&');
        if (ptr != NULL) {
            ptr++; // Skip '&'
        } else {
            break; // End of input
        }
    }
}

// Function to write the sphere's geometry to a binary STL file
void writeSphereToBinarySTL(int size, GLint slices, GLint stacks, const char *filename) {
    // Open the file for binary writing
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    // Write the 80-byte header (can be any data, here we use zeros)
    char header[80] = {0};
    fwrite(header, sizeof(char), 80, file);

    // Write the number of triangles (4 bytes, little-endian)
    uint32_t numTriangles = slices * stacks * 2;  // Each stack creates two triangles per slice
    fwrite(&numTriangles, sizeof(uint32_t), 1, file);

    for (int partials = 0; partials < size; partials++) {
        char p_filename[50];
        snprintf(p_filename, sizeof(p_filename), "partial_%d.stl", partials);

        FILE *pfile = fopen(p_filename, "rb");
        if (!pfile) {
            printf("Error: Unable to open file for writing.\n");
            return;
        }

        char buff[1024];
        size_t bytesRead;
        while ((bytesRead = fread(buff, 1, sizeof(buff), pfile))) {
            fwrite(buff, 1, bytesRead, file);
        }

        fclose(pfile);
    }
    fclose(file);

    printf("Partial Sphere Binary STL file has been written to %s\n", filename);
}

void partialSphere(GLfloat radius, GLint slices, GLint stackStart, GLint stackEnd, const char *filename) {
    FILE *file = fopen(filename, "wb");
    GLint stacks = stackEnd - stackStart;

    // Iterate over the stacks and slices and create triangles
    for (int stack = stackStart; stack < stackEnd; ++stack) {
        for (int slice = 0; slice < slices; ++slice) {
            // Calculate the spherical coordinates for the four vertices of the current quadrilateral
            GLfloat theta1 = 2.0f * M_PI * slice / slices;
            GLfloat theta2 = 2.0f * M_PI * (slice + 1) / slices;
            GLfloat phi1 = M_PI * stack / stacks;
            GLfloat phi2 = M_PI * (stack + 1) / stacks;

            // Calculate the vertex positions for the first triangle
            GLfloat x1 = radius * sinf(phi1) * cosf(theta1);
            GLfloat y1 = radius * cosf(phi1);
            GLfloat z1 = radius * sinf(phi1) * sinf(theta1);

            GLfloat x2 = radius * sinf(phi1) * cosf(theta2);
            GLfloat y2 = radius * cosf(phi1);
            GLfloat z2 = radius * sinf(phi1) * sinf(theta2);

            GLfloat x3 = radius * sinf(phi2) * cosf(theta1);
            GLfloat y3 = radius * cosf(phi2);
            GLfloat z3 = radius * sinf(phi2) * sinf(theta1);

            // Calculate the normal for the first triangle using cross product
            GLfloat ux = x2 - x1, uy = y2 - y1, uz = z2 - z1;
            GLfloat vx = x3 - x1, vy = y3 - y1, vz = z3 - z1;
            GLfloat nx = uy * vz - uz * vy;
            GLfloat ny = uz * vx - ux * vz;
            GLfloat nz = ux * vy - uy * vx;

            GLfloat len = sqrtf(nx * nx + ny * ny + nz * nz);
            if (len != 0) {
                nx /= len;
                ny /= len;
                nz /= len;
            }

            // Write normal and vertices for the first triangle
            fwrite(&nx, sizeof(GLfloat), 1, file);
            fwrite(&ny, sizeof(GLfloat), 1, file);
            fwrite(&nz, sizeof(GLfloat), 1, file);
            fwrite(&x1, sizeof(GLfloat), 1, file);
            fwrite(&y1, sizeof(GLfloat), 1, file);
            fwrite(&z1, sizeof(GLfloat), 1, file);
            fwrite(&x2, sizeof(GLfloat), 1, file);
            fwrite(&y2, sizeof(GLfloat), 1, file);
            fwrite(&z2, sizeof(GLfloat), 1, file);
            fwrite(&x3, sizeof(GLfloat), 1, file);
            fwrite(&y3, sizeof(GLfloat), 1, file);
            fwrite(&z3, sizeof(GLfloat), 1, file);

            // Attribute byte count (2 bytes, typically 0)
            uint16_t attributeByteCount = 0;
            fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);

            // Second triangle (similarly calculated and written)
            GLfloat x4 = radius * sinf(phi2) * cosf(theta2);
            GLfloat y4 = radius * cosf(phi2);
            GLfloat z4 = radius * sinf(phi2) * sinf(theta2);

            // Calculate the normal for the second triangle
            GLfloat ux2 = x3 - x2, uy2 = y3 - y2, uz2 = z3 - z2;
            GLfloat vx2 = x4 - x2, vy2 = y4 - y2, vz2 = z4 - z2;
            GLfloat nx2 = uy2 * vz2 - uz2 * vy2;
            GLfloat ny2 = uz2 * vx2 - ux2 * vz2;
            GLfloat nz2 = ux2 * vy2 - uy2 * vx2;

            len = sqrtf(nx2 * nx2 + ny2 * ny2 + nz2 * nz2);
            if (len != 0) {
                nx2 /= len;
                ny2 /= len;
                nz2 /= len;
            }

            // Write normal and vertices for the second triangle
            fwrite(&nx2, sizeof(GLfloat), 1, file);
            fwrite(&ny2, sizeof(GLfloat), 1, file);
            fwrite(&nz2, sizeof(GLfloat), 1, file);
            fwrite(&x2, sizeof(GLfloat), 1, file);
            fwrite(&y2, sizeof(GLfloat), 1, file);
            fwrite(&z2, sizeof(GLfloat), 1, file);
            fwrite(&x3, sizeof(GLfloat), 1, file);
            fwrite(&y3, sizeof(GLfloat), 1, file);
            fwrite(&z3, sizeof(GLfloat), 1, file);
            fwrite(&x4, sizeof(GLfloat), 1, file);
            fwrite(&y4, sizeof(GLfloat), 1, file);
            fwrite(&z4, sizeof(GLfloat), 1, file);

            // Attribute byte count (2 bytes, typically 0)
            fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);
        }
    }

    // Close the file
    fclose(file);

    printf("Partial Sphere Binary STL file has been written to %s\n", filename);
}

void writeConeToBinarySTL(GLfloat radius, GLfloat height, GLint slices, const char *filename) {
    // Open the file for binary writing
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    // Write the 80-byte header (can be any data, here we use zeros)
    char header[80] = {0};
    fwrite(header, sizeof(char), 80, file);

    // Write the number of triangles (4 bytes, little-endian)
    uint32_t numTriangles = slices + slices;  // base + side triangles
    fwrite(&numTriangles, sizeof(uint32_t), 1, file);

    GLfloat halfHeight = height / 2.0f;

    // 1. Write base triangles
    for (int i = 0; i < slices; ++i) {
        GLfloat angle1 = 2.0f * M_PI * i / slices;
        GLfloat angle2 = 2.0f * M_PI * (i + 1) / slices;

        // Vertices of the triangle on the base
        GLfloat x1 = radius * cos(angle1);
        GLfloat z1 = radius * sin(angle1);
        GLfloat x2 = radius * cos(angle2);
        GLfloat z2 = radius * sin(angle2);

        // Calculate normal for the base (pointing up along the y-axis)
        GLfloat nx = 0.0f, ny = -1.0f, nz = 0.0f;

        // Write the normal and the vertices for the base triangle
        fwrite(&nx, sizeof(GLfloat), 1, file);
        fwrite(&ny, sizeof(GLfloat), 1, file);
        fwrite(&nz, sizeof(GLfloat), 1, file);

        // Center of the base (0, -halfHeight, 0)
        fwrite(&(GLfloat){0.0f}, sizeof(GLfloat), 1, file);
        fwrite(&(GLfloat){-halfHeight}, sizeof(GLfloat), 1, file);
        fwrite(&(GLfloat){0.0f}, sizeof(GLfloat), 1, file);

        // First base vertex (x1, -halfHeight, z1)
        fwrite(&x1, sizeof(GLfloat), 1, file);
        fwrite(&(GLfloat){-halfHeight}, sizeof(GLfloat), 1, file);
        fwrite(&z1, sizeof(GLfloat), 1, file);

        // Second base vertex (x2, -halfHeight, z2)
        fwrite(&x2, sizeof(GLfloat), 1, file);
        fwrite(&(GLfloat){-halfHeight}, sizeof(GLfloat), 1, file);
        fwrite(&z2, sizeof(GLfloat), 1, file);

        // Attribute byte count (2 bytes, typically 0)
        uint16_t attributeByteCount = 0;
        fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);
    }

    // 2. Write side triangles
    for (int i = 0; i < slices; ++i) {
        GLfloat angle1 = 2.0f * M_PI * i / slices;
        GLfloat angle2 = 2.0f * M_PI * (i + 1) / slices;

        // Vertices of the triangle on the side
        GLfloat x1 = radius * cos(angle1);
        GLfloat z1 = radius * sin(angle1);
        GLfloat x2 = radius * cos(angle2);
        GLfloat z2 = radius * sin(angle2);

        // Calculate the normal for the side triangle (using cross product of two edge vectors)
        GLfloat ux = x2 - x1, uy = halfHeight - (-halfHeight), uz = z2 - z1;
        GLfloat vx = 0.0f - x1, vy = halfHeight - (-halfHeight), vz = 0.0f - z1;
        GLfloat nx = uy * vz - uz * vy;
        GLfloat ny = uz * vx - ux * vz;
        GLfloat nz = ux * vy - uy * vx;

        GLfloat len = sqrtf(nx * nx + ny * ny + nz * nz);
        if (len != 0) {
            nx /= len;
            ny /= len;
            nz /= len;
        }

        // Write normal and vertices for the side triangle
        fwrite(&nx, sizeof(GLfloat), 1, file);
        fwrite(&ny, sizeof(GLfloat), 1, file);
        fwrite(&nz, sizeof(GLfloat), 1, file);

        // Tip of the cone (0, halfHeight, 0)
        fwrite(&(GLfloat){0.0f}, sizeof(GLfloat), 1, file);
        fwrite(&(GLfloat){halfHeight}, sizeof(GLfloat), 1, file);
        fwrite(&(GLfloat){0.0f}, sizeof(GLfloat), 1, file);

        // First base vertex (x1, -halfHeight, z1)
        fwrite(&x1, sizeof(GLfloat), 1, file);
        fwrite(&(GLfloat){-halfHeight}, sizeof(GLfloat), 1, file);
        fwrite(&z1, sizeof(GLfloat), 1, file);

        // Second base vertex (x2, -halfHeight, z2)
        fwrite(&x2, sizeof(GLfloat), 1, file);
        fwrite(&(GLfloat){-halfHeight}, sizeof(GLfloat), 1, file);
        fwrite(&z2, sizeof(GLfloat), 1, file);

        // Attribute byte count (2 bytes, typically 0)
        uint16_t attributeByteCount = 0;
        fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);
    }

    // Close the file
    fclose(file);

    printf("Cone Binary STL file has been written to %s\n", filename);
    stl_to_h_file(filename);
    //mergeSTLFiles();
}

void writeCubeToBinarySTL(GLfloat sideLength, const char *filename) {
    // Cube vertices are calculated based on the side length
    GLfloat halfSide = sideLength / 2.0f;

    // Cube vertices (8 points)
    GLfloat vertices[8][3] = {
        {-halfSide, -halfSide, halfSide},   // 0
        { halfSide, -halfSide, halfSide},   // 1
        { halfSide,  halfSide, halfSide},   // 2
        {-halfSide,  halfSide, halfSide},   // 3
        {-halfSide, -halfSide, -halfSide},  // 4
        { halfSide, -halfSide, -halfSide},  // 5
        { halfSide,  halfSide, -halfSide},  // 6
        {-halfSide,  halfSide, -halfSide}   // 7
    };

    // Cube faces (6 faces, each defined by 4 vertices in the original code, split into 2 triangles each)
    int faces[12][3] = {
        {0, 1, 2}, {0, 2, 3},  // Front face
        {4, 5, 6}, {4, 6, 7},  // Back face
        {0, 1, 5}, {0, 5, 4},  // Bottom face
        {2, 3, 7}, {2, 7, 6},  // Top face
        {1, 2, 6}, {1, 6, 5},  // Right face
        {0, 3, 7}, {0, 7, 4}   // Left face
    };

    // Open the file for binary writing
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    // Write the 80-byte header (can be any data, here we use zeros)
    char header[80] = {0};
    fwrite(header, sizeof(char), 80, file);

    // Write the number of triangles (4 bytes, little-endian)
    uint32_t numTriangles = 12;
    fwrite(&numTriangles, sizeof(uint32_t), 1, file);

    // Iterate over each face and write it to the binary STL file
    for (int i = 0; i < 12; i++) {
        int v0 = faces[i][0];
        int v1 = faces[i][1];
        int v2 = faces[i][2];

        // Calculate the normal vector for the triangle (simple method)
        GLfloat v1v0[3] = { vertices[v1][0] - vertices[v0][0], vertices[v1][1] - vertices[v0][1], vertices[v1][2] - vertices[v0][2] };
        GLfloat v2v0[3] = { vertices[v2][0] - vertices[v0][0], vertices[v2][1] - vertices[v0][1], vertices[v2][2] - vertices[v0][2] };

        GLfloat normal[3];
        normal[0] = v1v0[1] * v2v0[2] - v1v0[2] * v2v0[1];
        normal[1] = v1v0[2] * v2v0[0] - v1v0[0] * v2v0[2];
        normal[2] = v1v0[0] * v2v0[1] - v1v0[1] * v2v0[0];

        // Normalize the normal vector
        GLfloat length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
        normal[0] /= length;
        normal[1] /= length;
        normal[2] /= length;

        // Write the facet normal
        fwrite(normal, sizeof(GLfloat), 3, file);

        // Write the three vertices of the triangle
        fwrite(&vertices[v0], sizeof(GLfloat), 3, file);
        fwrite(&vertices[v1], sizeof(GLfloat), 3, file);
        fwrite(&vertices[v2], sizeof(GLfloat), 3, file);

        // Write the attribute byte count (2 bytes, typically 0)
        uint16_t attributeByteCount = 0;
        fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);
    }

    // Close the file
    fclose(file);

    printf("Cube Binary STL file has been written to %s\n", filename);
    stl_to_h_file(filename);
    //mergeSTLFiles();
}

void writePyramidToBinarySTL(GLfloat height, const char *filename) {
    // Open the file for binary writing
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    // Write the 80-byte header (can be any data, here we use zeros)
    char header[80] = {0};
    fwrite(header, sizeof(char), 80, file);

    // Write the number of triangles (4 faces, so 4 triangles)
    uint32_t numTriangles = 4;
    fwrite(&numTriangles, sizeof(uint32_t), 1, file);

    // Define the base vertices of the pyramid
    GLfloat baseVertices[4][3] = {
        {-1.0f, -1.0f, 1.0f},  // Front-left
        { 1.0f, -1.0f, 1.0f},  // Front-right
        { 1.0f, -1.0f, -1.0f}, // Back-right
        {-1.0f, -1.0f, -1.0f}  // Back-left
    };

    // Define the apex of the pyramid (tip)
    GLfloat apex[3] = {0.0f, height, 0.0f};

    // 1. Front face
    // Normal is computed by cross product of two vectors: (b - a) and (c - a)
    GLfloat normal1[3] = {0.0f, 1.0f, 0.0f}; // Upward normal for front face
    // Write normal
    fwrite(normal1, sizeof(GLfloat), 3, file);

    // Vertices for the front face (apex, front-left, front-right)
    fwrite(apex, sizeof(GLfloat), 3, file); // Apex
    fwrite(baseVertices[0], sizeof(GLfloat), 3, file); // Front-left
    fwrite(baseVertices[1], sizeof(GLfloat), 3, file); // Front-right

    // Attribute byte count (2 bytes, typically 0)
    uint16_t attributeByteCount = 0;
    fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);

    // 2. Right face
    GLfloat normal2[3] = {1.0f, 0.0f, 0.0f}; // Rightward normal for right face
    // Write normal
    fwrite(normal2, sizeof(GLfloat), 3, file);

    // Vertices for the right face (apex, front-right, back-right)
    fwrite(apex, sizeof(GLfloat), 3, file); // Apex
    fwrite(baseVertices[1], sizeof(GLfloat), 3, file); // Front-right
    fwrite(baseVertices[2], sizeof(GLfloat), 3, file); // Back-right

    // Attribute byte count (2 bytes, typically 0)
    fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);

    // 3. Back face
    GLfloat normal3[3] = {0.0f, -1.0f, 0.0f}; // Downward normal for back face
    // Write normal
    fwrite(normal3, sizeof(GLfloat), 3, file);

    // Vertices for the back face (apex, back-right, back-left)
    fwrite(apex, sizeof(GLfloat), 3, file); // Apex
    fwrite(baseVertices[2], sizeof(GLfloat), 3, file); // Back-right
    fwrite(baseVertices[3], sizeof(GLfloat), 3, file); // Back-left

    // Attribute byte count (2 bytes, typically 0)
    fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);

    // 4. Left face
    GLfloat normal4[3] = {-1.0f, 0.0f, 0.0f}; // Leftward normal for left face
    // Write normal
    fwrite(normal4, sizeof(GLfloat), 3, file);

    // Vertices for the left face (apex, back-left, front-left)
    fwrite(apex, sizeof(GLfloat), 3, file); // Apex
    fwrite(baseVertices[3], sizeof(GLfloat), 3, file); // Back-left
    fwrite(baseVertices[0], sizeof(GLfloat), 3, file); // Front-left

    // Attribute byte count (2 bytes, typically 0)
    fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);

    // Close the file
    fclose(file);

    printf("Pyramid Binary STL file has been written to %s\n", filename);
    stl_to_h_file(filename);
    //mergeSTLFiles();
}

void writeCylinderToBinarySTL(float radius, float length, int n, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    // Write the 80-byte header (can be any data, here we use zeros)
    char header[80] = {0};
    fwrite(header, sizeof(char), 80, file);

    // Calculate the number of triangles: 2*n (side triangles) + n (top) + n (bottom)
    uint32_t numTriangles = n * 2 + n + n;
    fwrite(&numTriangles, sizeof(uint32_t), 1, file);

    // Angle increment for each side of the polygon
    float angleIncrement = 2 * M_PI / n;

    // Vertices for the prism
    GLfloat topCenter[3] = {0.0f, 0.0f, length};   // Top center vertex
    GLfloat bottomCenter[3] = {0.0f, 0.0f, 0.0f};  // Bottom center vertex

    // Side faces (Each side face is composed of 2 triangles)
    for (int i = 0; i < n; i++) {
        // Calculate the angle for the current side
        float angle1 = i * angleIncrement;
        float angle2 = (i + 1) * angleIncrement;

        // Calculate the x, y coordinates for the vertices
        GLfloat x1 = radius * cos(angle1);
        GLfloat y1 = radius * sin(angle1);
        GLfloat x2 = radius * cos(angle2);
        GLfloat y2 = radius * sin(angle2);

        // Normal for the side faces (pointing outward)
        GLfloat normal[3] = {0.0f, 0.0f, 1.0f}; // Normal pointing outwards

        // Side face triangles
        GLfloat v1[3] = {x1, y1, 0.0f};
        GLfloat v2[3] = {x1, y1, length};
        GLfloat v3[3] = {x2, y2, length};
        calculateNormalPrism(v1, v2, v3, normal);
        fwrite(normal, sizeof(GLfloat), 3, file);
        fwrite(&v1, sizeof(GLfloat), 3, file);
        fwrite(&v2, sizeof(GLfloat), 3, file);
        fwrite(&v3, sizeof(GLfloat), 3, file);
        uint16_t attributeByteCount = 0;
        fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);

        GLfloat v4[3] = {x2, y2, 0.0f};
        calculateNormalPrism(v1, v3, v4, normal);
        fwrite(normal, sizeof(GLfloat), 3, file);
        fwrite(&v1, sizeof(GLfloat), 3, file);
        fwrite(&v3, sizeof(GLfloat), 3, file);
        fwrite(&v4, sizeof(GLfloat), 3, file);
        fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);
    }

    // Top face (n triangles)
    for (int i = 0; i < n; i++) {
        float angle1 = i * angleIncrement;
        float angle2 = (i + 1) * angleIncrement;

        // Calculate the x, y coordinates for the vertices
        GLfloat x1 = radius * cos(angle1);
        GLfloat y1 = radius * sin(angle1);
        GLfloat x2 = radius * cos(angle2);
        GLfloat y2 = radius * sin(angle2);

        // Normal for the top face (pointing upwards)
        GLfloat normal[3] = {0.0f, 0.0f, 1.0f}; // Normal pointing upwards

        GLfloat v1[3] = {x1, y1, length};
        GLfloat v2[3] = {x2, y2, length};
        fwrite(normal, sizeof(GLfloat), 3, file);
        fwrite(&topCenter, sizeof(GLfloat), 3, file);
        fwrite(&v1, sizeof(GLfloat), 3, file);
        fwrite(&v2, sizeof(GLfloat), 3, file);
        uint16_t attributeByteCount = 0;
        fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);
    }

    // Bottom face (n triangles)
    for (int i = 0; i < n; i++) {
        float angle1 = i * angleIncrement;
        float angle2 = (i + 1) * angleIncrement;

        // Calculate the x, y coordinates for the vertices
        GLfloat x1 = radius * cos(angle1);
        GLfloat y1 = radius * sin(angle1);
        GLfloat x2 = radius * cos(angle2);
        GLfloat y2 = radius * sin(angle2);

        // Normal for the bottom face (pointing downwards)
        GLfloat normal[3] = {0.0f, 0.0f, -1.0f}; // Normal pointing downwards

        GLfloat v1[3] = {x1, y1, 0.0f};
        GLfloat v2[3] = {x2, y2, 0.0f};
        fwrite(normal, sizeof(GLfloat), 3, file);
        fwrite(&bottomCenter, sizeof(GLfloat), 3, file);
        fwrite(&v1, sizeof(GLfloat), 3, file);
        fwrite(&v2, sizeof(GLfloat), 3, file);
        uint16_t attributeByteCount = 0;
        fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);
    }

    // Close the file
    fclose(file);
    printf("Cylinder Binary STL file has been written to %s\n", filename);
    stl_to_h_file(filename);
    //mergeSTLFiles();
}

void calculateNormalPrism(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], GLfloat normal[3]) {
    // Calculate vectors v1-v2 and v1-v3
    GLfloat vec1[3] = {v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2]};
    GLfloat vec2[3] = {v3[0] - v1[0], v3[1] - v1[1], v3[2] - v1[2]};

    // Cross product of v1-v2 and v1-v3 to get the normal
    normal[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
    normal[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
    normal[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];

    // Normalize the normal vector
    GLfloat length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    normal[0] /= length;
    normal[1] /= length;
    normal[2] /= length;
}


// Function to write the prism's geometry to a binary STL file
void writePrismToBinarySTL(float radius, float length, int n, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    // Write the 80-byte header (can be any data, here we use zeros)
    char header[80] = {0};
    fwrite(header, sizeof(char), 80, file);

    // Calculate the number of triangles: 2*n (side triangles) + n (top) + n (bottom)
    uint32_t numTriangles = n * 2 + n + n;
    fwrite(&numTriangles, sizeof(uint32_t), 1, file);

    // Angle increment for each side of the polygon
    float angleIncrement = 2 * M_PI / n;

    // Vertices for the prism
    GLfloat topCenter[3] = {0.0f, 0.0f, length};   // Top center vertex
    GLfloat bottomCenter[3] = {0.0f, 0.0f, 0.0f};  // Bottom center vertex

    // Side faces (Each side face is composed of 2 triangles)
    for (int i = 0; i < n; i++) {
        // Calculate the angle for the current side
        float angle1 = i * angleIncrement;
        float angle2 = (i + 1) * angleIncrement;

        // Calculate the x, y coordinates for the vertices
        GLfloat x1 = radius * cos(angle1);
        GLfloat y1 = radius * sin(angle1);
        GLfloat x2 = radius * cos(angle2);
        GLfloat y2 = radius * sin(angle2);

        // Normal for the side faces (pointing outward)
        GLfloat normal[3] = {0.0f, 0.0f, 1.0f}; // Normal pointing outwards

        // Side face triangles
        GLfloat v1[3] = {x1, y1, 0.0f};
        GLfloat v2[3] = {x1, y1, length};
        GLfloat v3[3] = {x2, y2, length};
        calculateNormalPrism(v1, v2, v3, normal);
        fwrite(normal, sizeof(GLfloat), 3, file);
        fwrite(&v1, sizeof(GLfloat), 3, file);
        fwrite(&v2, sizeof(GLfloat), 3, file);
        fwrite(&v3, sizeof(GLfloat), 3, file);
        uint16_t attributeByteCount = 0;
        fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);

        GLfloat v4[3] = {x2, y2, 0.0f};
        calculateNormalPrism(v1, v3, v4, normal);
        fwrite(normal, sizeof(GLfloat), 3, file);
        fwrite(&v1, sizeof(GLfloat), 3, file);
        fwrite(&v3, sizeof(GLfloat), 3, file);
        fwrite(&v4, sizeof(GLfloat), 3, file);
        fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);
    }

    // Top face (n triangles)
    for (int i = 0; i < n; i++) {
        float angle1 = i * angleIncrement;
        float angle2 = (i + 1) * angleIncrement;

        // Calculate the x, y coordinates for the vertices
        GLfloat x1 = radius * cos(angle1);
        GLfloat y1 = radius * sin(angle1);
        GLfloat x2 = radius * cos(angle2);
        GLfloat y2 = radius * sin(angle2);

        // Normal for the top face (pointing upwards)
        GLfloat normal[3] = {0.0f, 0.0f, 1.0f}; // Normal pointing upwards

        GLfloat v1[3] = {x1, y1, length};
        GLfloat v2[3] = {x2, y2, length};
        fwrite(normal, sizeof(GLfloat), 3, file);
        fwrite(&topCenter, sizeof(GLfloat), 3, file);
        fwrite(&v1, sizeof(GLfloat), 3, file);
        fwrite(&v2, sizeof(GLfloat), 3, file);
        uint16_t attributeByteCount = 0;
        fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);
    }

    // Bottom face (n triangles)
    for (int i = 0; i < n; i++) {
        float angle1 = i * angleIncrement;
        float angle2 = (i + 1) * angleIncrement;

        // Calculate the x, y coordinates for the vertices
        GLfloat x1 = radius * cos(angle1);
        GLfloat y1 = radius * sin(angle1);
        GLfloat x2 = radius * cos(angle2);
        GLfloat y2 = radius * sin(angle2);

        // Normal for the bottom face (pointing downwards)
        GLfloat normal[3] = {0.0f, 0.0f, -1.0f}; // Normal pointing downwards

        GLfloat v1[3] = {x1, y1, 0.0f};
        GLfloat v2[3] = {x2, y2, 0.0f};
        fwrite(normal, sizeof(GLfloat), 3, file);
        fwrite(&bottomCenter, sizeof(GLfloat), 3, file);
        fwrite(&v1, sizeof(GLfloat), 3, file);
        fwrite(&v2, sizeof(GLfloat), 3, file);
        uint16_t attributeByteCount = 0;
        fwrite(&attributeByteCount, sizeof(uint16_t), 1, file);
    }

    // Close the file
    fclose(file);
    printf("Prism Binary STL file has been written to %s\n", filename);
    stl_to_h_file(filename);

    //mergeSTLFiles();
}

void stl_to_h_file(const char *filePath) {

    Mesh mesh = readStl(filePath);
    scaleMesh(&mesh);
    writeHeaderFile(&mesh, "../pruebaconect/shapes.h");

    free(mesh.vertices);
    free(mesh.triangles);
    free(mesh.normals);

    printf("Header file '%s' generated successfully.\n", "shapes.h");
}

void process_partial_STL(int rank, int size, const char* input) {
    parseInput(input);  // Parse the input string

    for (int i = 0; i < shapeCount; i++) {
        Shape currentShape = shapes[i];

        const char filename[50];
        snprintf(filename, sizeof(filename), "../Resources/partial_binary_%d.stl", rank);  // Output binary STL file
        const char create[50];
        snprintf(create, sizeof(create), "touch %s", filename);
        system(create);

        // Determine which shape to draw based on shapeType
        if (strcmp(currentShape.shapeType, "sphere") == 0) {

            GLint stacks_per_node = currentShape.stacks / size;
            GLint start = rank * stacks_per_node;
            GLint end = (rank == size - 1) ? currentShape.stacks : start + stacks_per_node;

            partialSphere(currentShape.param1, currentShape.slices, start, end, filename);
        }
        else if (strcmp(currentShape.shapeType, "cube") == 0) {
            writeCubeToBinarySTL(currentShape.param1, filename);
        }
        else if (strcmp(currentShape.shapeType, "cylinder") == 0) {
            writeCylinderToBinarySTL(currentShape.param1, currentShape.param2, currentShape.slices,filename);
        }
        else if (strcmp(currentShape.shapeType, "cone") == 0) {
            writeConeToBinarySTL(currentShape.param1, currentShape.param2, currentShape.slices,filename);
        }
        else if (strcmp(currentShape.shapeType, "pyramid") == 0) {
            writePyramidToBinarySTL(currentShape.param1,filename);
        }
        else if (strcmp(currentShape.shapeType, "prism") == 0) {
            writePrismToBinarySTL(currentShape.param1, currentShape.param2, currentShape.n,filename);
        }
    }
}

void process_STL(int size) {
    for (int i = 0; i < shapeCount; i++) {
        Shape currentShape = shapes[i];

        const char *filename = "../Resources/binary.stl";  // Output binary STL file
        const char create[50];
        snprintf(create, sizeof(create), "touch %s", filename);
        system(create);

        // Determine which shape to draw based on shapeType
        if (strcmp(currentShape.shapeType, "sphere") == 0) {
            writeSphereToBinarySTL(size, currentShape.slices, currentShape.stacks, filename);
        }
        else if (strcmp(currentShape.shapeType, "cube") == 0) {
            writeCubeToBinarySTL(currentShape.param1, filename);
        }
        else if (strcmp(currentShape.shapeType, "cylinder") == 0) {
            writeCylinderToBinarySTL(currentShape.param1, currentShape.param2, currentShape.slices,filename);
        }
        else if (strcmp(currentShape.shapeType, "cone") == 0) {
            writeConeToBinarySTL(currentShape.param1, currentShape.param2, currentShape.slices,filename);
        }
        else if (strcmp(currentShape.shapeType, "pyramid") == 0) {
            writePyramidToBinarySTL(currentShape.param1,filename);
        }
        else if (strcmp(currentShape.shapeType, "prism") == 0) {
            writePrismToBinarySTL(currentShape.param1, currentShape.param2, currentShape.n,filename);
        }
    }
}