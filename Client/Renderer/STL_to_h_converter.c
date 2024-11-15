//
// Created by michael-v on 13/11/24.
//


#include "STL_to_h_converter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>


// Function to read the STL file
Mesh readStl(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open STL file");
        exit(1);
    }

    Mesh mesh = {NULL, NULL, NULL, 0, 0};
    fseek(file, 80, SEEK_SET); // Skip STL header
    fread(&mesh.triangleCount, sizeof(uint32_t), 1, file);

    mesh.triangles = malloc(sizeof(Triangle) * mesh.triangleCount);
    mesh.normals = malloc(sizeof(Normal) * mesh.triangleCount);
    mesh.vertices = malloc(sizeof(Vertex) * mesh.triangleCount * 3);

    int vertexIndex = 0;
    for (int i = 0; i < mesh.triangleCount; i++) {
        Normal normal;
        fread(&normal, sizeof(float), 3, file); // Read normal vector
        mesh.normals[i] = normal;

        for (int j = 0; j < 3; j++) {
            Vertex v;
            fread(&v, sizeof(float), 3, file); // Read vertex
            mesh.vertices[vertexIndex] = v;

            if (j == 0) mesh.triangles[i].v1 = vertexIndex;
            if (j == 1) mesh.triangles[i].v2 = vertexIndex;
            if (j == 2) mesh.triangles[i].v3 = vertexIndex;
            vertexIndex++;
        }
        fseek(file, 2, SEEK_CUR); // Skip attribute byte count
    }

    mesh.vertexCount = vertexIndex;
    fclose(file);
    return mesh;
}

// Function to scale the mesh
void scaleMesh(Mesh* mesh) {
    if (mesh->vertexCount == 0) return;

    Vertex min = mesh->vertices[0];
    Vertex max = mesh->vertices[0];

    for (int i = 1; i < mesh->vertexCount; i++) {
        Vertex v = mesh->vertices[i];
        min.x = fmin(min.x, v.x);
        min.y = fmin(min.y, v.y);
        min.z = fmin(min.z, v.z);
        max.x = fmax(max.x, v.x);
        max.y = fmax(max.y, v.y);
        max.z = fmax(max.z, v.z);
    }

    float size = fmaxf(fmaxf(max.x - min.x, max.y - min.y), max.z - min.z);
    for (int i = 0; i < mesh->vertexCount; i++) {
        mesh->vertices[i].x = (mesh->vertices[i].x - (max.x + min.x) * 0.5f) / size;
        mesh->vertices[i].y = (mesh->vertices[i].y - (max.y + min.y) * 0.5f) / size;
        mesh->vertices[i].z = (mesh->vertices[i].z - (max.z + min.z) * 0.5f) / size;
    }
}


// Function to write the .h file
void writeHeaderFile(const Mesh* mesh, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open output file");
        exit(1);
    }

    //fprintf(file, "#ifndef %s_H\n#define %s_H\n\n", NAMESPACE, NAMESPACE);
    fprintf(file, "namespace %s {\n\n", "shapes");
    fprintf(file, "const int vertexCount = %d;\n", mesh->vertexCount);
    fprintf(file, "const int triangleCount = %d;\n\n", mesh->triangleCount);

    // Write vertices
    fprintf(file, "const float vertices[][3] = {\n");
    for (int i = 0; i < mesh->vertexCount; i++) {
        fprintf(file, "%.4f, %.4f, %.4f",
                mesh->vertices[i].x,
                mesh->vertices[i].y,
                mesh->vertices[i].z);
        if (i < mesh->vertexCount - 1) {
            fprintf(file, ",\n");
        }
    }
    fprintf(file, "\n};\n\n");

    // Write triangles
    fprintf(file, "const unsigned short triangles[][3] = {\n");
    for (int i = 0; i < mesh->triangleCount; i++) {
        fprintf(file, "%d, %d, %d",
                mesh->triangles[i].v1,
                mesh->triangles[i].v2,
                mesh->triangles[i].v3);
        if (i < mesh->triangleCount - 1) {
            fprintf(file, ",\n");
        }
    }
    fprintf(file, "\n};\n\n");

    // Write triangle normals
    fprintf(file, "const float triangleNormals[][3] = {\n");
    for (int i = 0; i < mesh->triangleCount; i++) {
        fprintf(file, "%.4f, %.4f, %.4f",
                mesh->normals[i].x,
                mesh->normals[i].y,
                mesh->normals[i].z);
        if (i < mesh->triangleCount - 1) {
            fprintf(file, ",\n");
        }
    }
    fprintf(file, "\n};\n\n");

    fprintf(file, "} // namespace %s\n\n", "shapes");
    //fprintf(file, "#endif // %s_H\n", NAMESPACE);

    fclose(file);
}











