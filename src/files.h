#ifndef FILES_H
#define FILES_H

char* load_asset(const char *filename);
int file_exists(const char *path);
int mkdir_if_not_exists(char *path);

#endif