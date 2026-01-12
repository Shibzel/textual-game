#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "file_loader.h"

char* load_asset(const char *filename) {
    // Variables 
    char path[256];
    long file_size;
    struct stat st;
    char *buffer;


    // 1. Construct path to assets subfolder
    snprintf(path, sizeof(path), "assets/%s", filename);

    // Open the file
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return NULL;
    }

    //  Get file size 
    if (stat(path, &st) != 0) {
        perror("Error getting file size");
        fclose(fp);
        return NULL;
    }
    file_size = st.st_size;

    // Allocate memory
    buffer = (char *)malloc(file_size + 1); // +1 for null terminator
    if (buffer == NULL) {
        perror("Memory allocation failed");
        fclose(fp);
        return NULL;
    }

    // Read content into buffer
    fread(buffer, 1, file_size, fp);
    buffer[file_size] = '\0'; // Null-terminate
    fclose(fp);

    return buffer;
}