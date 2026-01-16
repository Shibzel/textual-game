#include <stdio.h>
#include <stdlib.h>

#include "save_manager.h"
#include "files.h"
#include "parser.h"


int save_exists(char *save_path, char *save_fn) {
    const unsigned path_length = 512;
    char file_path[path_length];

    snprintf(file_path, path_length, "%s/%s", save_path, save_fn);
    return file_exists(file_path);
}

save load_save(char *save_path, char *save_fn) {
    save loaded_save;
    char file_path[512];
    char *file_content; 
    char location[2];

    snprintf(file_path, sizeof(file_path), "%s/%s", save_path, save_fn);


    file_content = load_asset(file_path);

    extract_text(file_content, 2, loaded_save.name, sizeof(loaded_save.name));
    extract_text(file_content, 4, location, sizeof(location));

    loaded_save.status = atoi(location);

    //TODO : Implement Time & Items

    return loaded_save;
}

char *items_parse_to_file(items items_array) {
    char *buffer = malloc(ITEMS_FILE_LENGTH);  // Items are stored under the format 00\n, a hundred times maximum
    if (buffer == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    buffer[0] = '\0';  // The array can be empty
    unsigned i = 0;
    while (
        items_array[i] != 0  // I00 does not exist, so 0 means it is the end of the array
        && i < ITEMS_MAX
    ) {
        char temp[5]; // "I00\n" + '\0'
        snprintf(temp, sizeof(temp), "I%02d\n", items_array[i]);
        strcat(buffer, temp);  // Concatenation
        i++;
    }

    return buffer;
}

void items_parse_to_array(items items_array, char *raw_items) {
    // unsigned i = 0; // Commence à 0 pour remplir tout le tableau

    // while (i < ITEMS_MAX) {
    //     char line[5]; // "I00\n" + '\0'
    //     if (get_line_content(raw_items, i + 1) == NULL) {
    //         items_array[i] = 0; // Fin du tableau
    //         break;
    //     }

    //     int value;
    //     if (sscanf(line, "I%2d", &value) != 1) {
    //         items_array[i] = 0; // Format invalide
    //     } else {
    //         items_array[i] = (items)value; // Stocke la valeur
    //     }

    //     printf("%u, %s, %u\n", i, line, items_array[i]);
    //     i++;
    // }
}

void save_save(char *save_path, char *save_fn, save current_save) {
    char buffer[TEXT_MAX_SIZE];
    char file_path[512];
    snprintf(file_path, sizeof(file_path), "%s/%s", save_path, save_fn);
    FILE *fptr = fopen(file_path, "wb");

    mkdir_if_not_exists(save_path);
    char *parsed_items = items_parse_to_file(current_save.items);
    snprintf(buffer, TEXT_MAX_SIZE, "%d\n%s\n%d\n%d\n%s",
            current_save.save_version, current_save.name,
            current_save.time_elapsed, current_save.status,
            parsed_items);
    free(parsed_items);

    fptr = fopen(file_path, "wb");
    if (fptr == NULL) {
        perror("Error while opening file");
        return;
    }
    fwrite(buffer, sizeof(char), strlen(buffer), fptr);
    fclose(fptr);
}
