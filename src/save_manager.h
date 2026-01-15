#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAVE_PATH "./saves/"
#define ASSETS_FP "./assets/"
#define ASSET_FILE_FORMAT "%s_%d.txt"
#define SAVE_FILE_FORMAT "save_%d.txt"
#define SAVE_FILE_LENGTH 11
#define MAX_NAME_SIZE 256
#define MAX_SAVES 3
#define CURRENT_SAVE_VERSION 1
#define ITEMS_MAX 100
#define TEXT_MAX_SIZE 2048

typedef unsigned items[ITEMS_MAX];
typedef struct {
    unsigned save_version; // The version of the saving system, in case migration
                          // is needed.
    char name[MAX_NAME_SIZE]; // Name of the player and of the save
    unsigned time_elapsed;    // For statistics and the speedrun counter
    unsigned status;          // Status of the game, where the player progressed
    items items;              // The list of items the player has
} save;


int save_exists(char *save_path, char *save_fn);
save load_save(char *save_path, char *save_fn);
char *items_parse(items owned_items);
void save_save(char *save_path, char *save_fn, save current_save);
char *get_name(char *name, char *display, char *confirmation_display, char *bad_input);

#endif // SAVE_MANAGER_H