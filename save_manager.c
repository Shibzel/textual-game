#include "save_manager.h"
#include "files.h"


unsigned get_choice(char *display, char *bad_input, unsigned max_inputs) {
  unsigned input, unvalid;

  do {
    unvalid = 0;

    printf(display, max_inputs);
    if (scanf("%u", &input) != 1 || input > max_inputs) {
      unvalid = 1;
      puts(bad_input);
    }
    printf("\n");
  } while (unvalid);

  return input;
}


int save_exists(char *save_path, char *save_fn) {
  const unsigned path_length = 512;
  char file_path[path_length];

  snprintf(file_path, path_length, "%s/%s", save_path, save_fn);
  return file_exists(file_path);
}

save load_save(char *save_path, char *save_fn) {
  // TODO: Implement
}

char *items_parse(items owned_items) {
  // TODO: Implement
  return "Hello, world!";
}

void save_save(char *save_path, char *save_fn, save current_save) {
  char buffer[TEXT_MAX_SIZE];
  char file_path[512];
  snprintf(file_path, sizeof(file_path), "%s/%s", save_path, save_fn);
  FILE *fptr = fopen(file_path, "wb");

  mkdir_if_not_exists(save_path);
  snprintf(buffer, TEXT_MAX_SIZE, "%d\n%s\n%d\n%d\n%s",
           current_save.save_version, current_save.name,
           current_save.time_elapsed, current_save.status,
           items_parse(current_save.items));

  fptr = fopen(file_path, "wb");
  // if (fptr == NULL) {
  //     perror("Erreur lors de l'ouverture du fichier");
  //     return;
  // }
  fwrite(buffer, sizeof(char), strlen(buffer), fptr);
  fclose(fptr);
}
