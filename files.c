#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  // TODO: Déplacer cette fonction dans un module
  int status = 0;
  int exists = 0;

  struct stat buffer;
  exists = !stat(path, &buffer);

  if (!exists) {
    status = mkdir(path, 0755);
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