#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "files.h"

#include <stdio.h>
#ifdef _WIN32
#include <io.h>
#define ACCESS _access
#else
#include <unistd.h>
#define ACCESS access
#endif


int mkdir_if_not_exists(char *path) {
    int status = 0;
    int exists = 0;

    struct stat buffer;
    exists = !stat(path, &buffer);

    if (!exists) {
        #ifdef _WIN32 
        status = mkdir(path);
        #else 
        status = mkdir(path, 0755);
        #endif
    }
    return status;
}


int file_exists(const char *path) { return ACCESS(path, 0) == 0; }

char* load_asset(const char *filename) {
    // Variables 
    char path[256];
    long file_size;
    struct stat st;
    char *buffer;
    
    // Construct path to assets subfolder
    snprintf(path, sizeof(path), "%s", filename);
    
    // Open the file
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return NULL;
    }
    
    // Get file size 
    if (stat(path, &st) != 0) {
        perror("Error getting file size");
        fclose(fp);
        return NULL;
    }
    file_size = st.st_size;
    
    // Allocate memory
    buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        fclose(fp);
        return NULL;
    }
    
    // Read content into buffer
    size_t bytes_read = fread(buffer, 1, file_size, fp);
    buffer[bytes_read] = '\0';
    fclose(fp);
    
    // ---------------------------------------------------------
    //  COLOR DECODER: Convert literal "\033" text to ASCII 27
    // ---------------------------------------------------------
    char *read_ptr = buffer;
    char *write_ptr = buffer;
    
    while (*read_ptr) {
        // Check if we found the literal sequence "\033"
        if (read_ptr[0] == '\\' && read_ptr[1] == '0' && 
            read_ptr[2] == '3' && read_ptr[3] == '3') {
            
            *write_ptr = '\033'; // Insert the REAL Escape character
            write_ptr++;
            read_ptr += 4;       // Skip the 4 text characters
        } 
        else {
            *write_ptr = *read_ptr;
            write_ptr++;
            read_ptr++;
        }
    }
    *write_ptr = '\0'; // Null-terminate at the new end position
    
    // ---------------------------------------------------------
    // CRITICAL FIX: Reallocate to actual size to avoid garbage data
    // ---------------------------------------------------------
    size_t new_size = write_ptr - buffer;
    char *trimmed = realloc(buffer, new_size + 1);
    if (trimmed != NULL) {
        buffer = trimmed;
    }
    // If realloc fails, buffer is still valid with correct null terminator
    
    return buffer;
}