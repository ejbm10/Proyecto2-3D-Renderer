
#include "stl_merger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <float.h>


// Function to load an STL file
int load_stl(const char *filename, STLHeader *header, Triangle_ **triangles) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Unable to open STL file");
        return -1;
    }

    // Read the header
    fread(header, sizeof(STLHeader), 1, file);

    // Allocate memory for the triangles
    *triangles = (Triangle_ *)malloc(header->num_triangles * sizeof(Triangle_));
    if (!*triangles) {
        perror("Unable to allocate memory for triangles");
        fclose(file);
        return -1;
    }

    // Read the triangles
    fread(*triangles, sizeof(Triangle_), header->num_triangles, file);

    fclose(file);
    return 0;
}

// Function to write an STL file
int save_stl(const char *filename, STLHeader *header, Triangle_ *triangles) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Unable to open STL file for writing");
        return -1;
    }

    // Write the header
    fwrite(header, sizeof(STLHeader), 1, file);

    // Write the triangles
    fwrite(triangles, sizeof(Triangle_), header->num_triangles, file);

    fclose(file);
    return 0;
}


/*
// Function to combine multiple STL meshes
int combine_meshes(const char *output_filename, const char **file_list, int num_files) {
    STLHeader *headers = (STLHeader *)malloc(num_files * sizeof(STLHeader));
    Triangle_ **triangles = (Triangle_ **)malloc(num_files * sizeof(Triangle_ *));
    unsigned int total_triangles = 0;

    if (!headers || !triangles) {
        perror("Unable to allocate memory for file data");
        return -1;
    }

    // Load all STL files
    for (int i = 0; i < num_files; ++i) {
        if (load_stl(file_list[i], &headers[i], &triangles[i]) != 0) {
            for (int j = 0; j < i; ++j) {
                free(triangles[j]);
            }
            free(headers);
            free(triangles);
            return -1;
        }
        total_triangles += headers[i].num_triangles;
    }

    // Combine all meshes into a single array
    Triangle_ *combined_triangles = (Triangle_ *)malloc(total_triangles * sizeof(Triangle_));
    if (!combined_triangles) {
        perror("Unable to allocate memory for combined triangles");
        for (int i = 0; i < num_files; ++i) {
            free(triangles[i]);
        }
        free(headers);
        free(triangles);
        return -1;
    }

    unsigned int offset_triangle_count = 0;
    float offset_step = 5.0f;  // Step for offset along the x-axis
    for (int i = 0; i < num_files; ++i) {
        // Apply offset to each file except the first one
        float offset = offset_step * i;
        for (unsigned int t = 0; t < headers[i].num_triangles; ++t) {
            for (int v = 0; v < 3; ++v) {
                triangles[i][t].vertices[v][0] += offset;  // Apply x-offset
            }
        }

        // Copy triangles into the combined array
        memcpy(combined_triangles + offset_triangle_count, triangles[i], headers[i].num_triangles * sizeof(Triangle_));
        offset_triangle_count += headers[i].num_triangles;

        // Free memory for the current file
        free(triangles[i]);
    }

    // Create the combined header
    STLHeader combined_header = { .num_triangles = total_triangles };
    memset(combined_header.header, 0, sizeof(combined_header.header));  // Clear the header

    // Save the combined STL file
    int result = save_stl(output_filename, &combined_header, combined_triangles);

    // Clean up
    free(headers);
    free(triangles);
    free(combined_triangles);

    return result;
}
*/

// Function to calculate the bounding box of a mesh
void calculate_bounding_box(const Triangle_ *triangles, unsigned int num_triangles, float *min_x, float *max_x) {
    *min_x = FLT_MAX;
    *max_x = -FLT_MAX;

    for (unsigned int i = 0; i < num_triangles; ++i) {
        for (int j = 0; j < 3; ++j) {
            float x = triangles[i].vertices[j][0];
            if (x < *min_x) *min_x = x;
            if (x > *max_x) *max_x = x;
        }
    }
}


// Function to combine multiple STL meshes without overlapping
int combine_meshes(const char *output_filename, const char **file_list, int num_files) {
    STLHeader *headers = (STLHeader *)malloc(num_files * sizeof(STLHeader));
    Triangle_ **triangles = (Triangle_ **)malloc(num_files * sizeof(Triangle_ *));
    unsigned int total_triangles = 0;

    if (!headers || !triangles) {
        perror("Unable to allocate memory for file data");
        return -1;
    }

    // Load all STL files
    for (int i = 0; i < num_files; ++i) {
        if (load_stl(file_list[i], &headers[i], &triangles[i]) != 0) {
            for (int j = 0; j < i; ++j) {
                free(triangles[j]);
            }
            free(headers);
            free(triangles);
            return -1;
        }
        total_triangles += headers[i].num_triangles;
    }

    // Combine all meshes into a single array
    Triangle_ *combined_triangles = (Triangle_ *)malloc(total_triangles * sizeof(Triangle_));
    if (!combined_triangles) {
        perror("Unable to allocate memory for combined triangles");
        for (int i = 0; i < num_files; ++i) {
            free(triangles[i]);
        }
        free(headers);
        free(triangles);
        return -1;
    }

    unsigned int offset_triangle_count = 0;
    float current_offset = 0.0f;
    float gap = 2.0f;  // Add a small gap between objects

    for (int i = 0; i < num_files; ++i) {
        // Calculate the bounding box of the current mesh
        float min_x, max_x;
        calculate_bounding_box(triangles[i], headers[i].num_triangles, &min_x, &max_x);

        // Apply offset to the current mesh
        for (unsigned int t = 0; t < headers[i].num_triangles; ++t) {
            for (int v = 0; v < 3; ++v) {
                triangles[i][t].vertices[v][0] += current_offset;  // Apply x-offset
            }
        }

        // Update the offset for the next mesh
        current_offset += (max_x - min_x) + gap;

        // Copy triangles into the combined array
        memcpy(combined_triangles + offset_triangle_count, triangles[i], headers[i].num_triangles * sizeof(Triangle_));
        offset_triangle_count += headers[i].num_triangles;

        // Free memory for the current file
        free(triangles[i]);
    }

    // Create the combined header
    STLHeader combined_header = { .num_triangles = total_triangles };
    memset(combined_header.header, 0, sizeof(combined_header.header));  // Clear the header

    // Save the combined STL file
    int result = save_stl(output_filename, &combined_header, combined_triangles);

    // Clean up
    free(headers);
    free(triangles);
    free(combined_triangles);

    return result;
}




