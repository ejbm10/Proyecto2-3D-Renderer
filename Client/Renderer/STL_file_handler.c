

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

// Function to list all STL files in a directory
int listSTLFiles(const char *directory, char ***fileList) {
    DIR *dir = opendir(directory);
    if (!dir) {
        printf("Error opening directory %s\n", directory);
        return 0;
    }

    struct dirent *entry;
    int fileCount = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".stl")) {
            printf("file name: ", entry->d_name);
            (*fileList) = realloc(*fileList, sizeof(char *) * (fileCount + 1));
            (*fileList)[fileCount] = malloc(strlen(directory) + strlen(entry->d_name) + 2);
            sprintf((*fileList)[fileCount], "%s/%s", directory, entry->d_name);
            fileCount++;
        }
    }
    closedir(dir);
    return fileCount;
}



// Function to combine multiple binary STL files
void combineMultipleBinarySTL(const char *outputFile, char **inputFiles, int numFiles) {
    FILE *out = fopen(outputFile, "wb");
    if (!out) {
        printf("Error opening output file.\n");
        return;
    }

    uint32_t totalTriangles = 0;
    uint8_t triangleBuffer[50];  // Each triangle is 50 bytes

    // Iterate over all input files to count the triangles and read their data
    for (int i = 0; i < numFiles; ++i) {
        FILE *f = fopen(inputFiles[i], "rb");
        if (!f) {
            printf("Error opening file: %s\n", inputFiles[i]);
            continue;
        }

        // Read the header and number of triangles from the current file
        char header[80];
        uint32_t numTriangles;

        // Read the header (80 bytes) - we skip it for now, we will overwrite it later
        fread(header, sizeof(char), 80, f);

        // Read the number of triangles from the file
        fread(&numTriangles, sizeof(uint32_t), 1, f);

        // Update the total number of triangles
        totalTriangles += numTriangles;

        // Skip the triangles to move to the next file
        fseek(f, numTriangles * 50, SEEK_CUR);

        fclose(f);
    }

    // Write the header to the output file (we will use an empty header or a constant one)
    char newHeader[80] = {0};  // Empty header or set a custom one
    fwrite(newHeader, sizeof(char), 80, out);

    // Write the total number of triangles to the output file
    fwrite(&totalTriangles, sizeof(uint32_t), 1, out);

    // Now write the triangles from each file
    for (int i = 0; i < numFiles; ++i) {
        FILE *f = fopen(inputFiles[i], "rb");
        if (!f) {
            continue;
        }

        // Skip the header (80 bytes) and the number of triangles (4 bytes)
        fseek(f, 84, SEEK_SET);

        // Read and write the triangles
        uint32_t numTriangles;
        fread(&numTriangles, sizeof(uint32_t), 1, f);

        // Write each triangle to the combined file
        for (uint32_t j = 0; j < numTriangles; ++j) {
            fread(triangleBuffer, sizeof(uint8_t), 50, f);  // Read 50 bytes per triangle
            fwrite(triangleBuffer, sizeof(uint8_t), 50, out);  // Write to the combined file
        }

        fclose(f);
    }

    // Close the output file
    fclose(out);

    printf("Successfully combined %d binary STL files into %s\n", numFiles, outputFile);
}


