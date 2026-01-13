#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Pour strcpy

#include "file_loader.h"
#include "parser.h"

#define SAVE_PATH "./saves/"
#define ASSETS_FP "./assets/"
#define ASSET_FILE_FORMAT "%s_%d.txt"
#define SAVE_FILE_FORMAT "save_%d.txt"
#define SAVE_FILE_LENGTH 11 // meh
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

#include <sys/stat.h>
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
#include <stdio.h>
#ifdef _WIN32
#include <io.h>
#define ACCESS _access
#else
#include <unistd.h>
#define ACCESS access
#endif
int file_exists(const char *path) { return ACCESS(path, 0) == 0; }

void language_selection(char *language) {
  unsigned choice;

  choice = get_choice(
      "Please chose a language / Merci de choisir un langage :\n 1) English\n "
      "2) Français\nInput a value / Entrez une valeur [1-2] : ",
      "Bad input. Try again / Mauvaise entrée. Réessayez", 2);
  switch (choice) {
  case 1:
    strcpy(language, "en");
    break;
  case 2:
    strcpy(language, "fr");
    break;
  }
}

char *get_name(char *name, char *display, char *confirmation_display,
               char *bad_input) {
  unsigned unvalid;
  const unsigned MAX_INPUTS = 2;

  do {
    unvalid = 1;

    printf("%s", display);
    if (scanf("%s", name) == 1) {
      printf(confirmation_display, name, MAX_INPUTS);
      if (get_choice("", bad_input, MAX_INPUTS) == 1) {
        unvalid = 0;
      }
    }
  } while (unvalid);

  return name;
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

void init_game(save current_save, char language[3]) {
  tableau_question retour_question;

  char *eng_text;
  int numbr;
  char s[100];

  numbr = 0;
  snprintf(s, sizeof(s), "%s_%03d.txt", language, numbr);
  eng_text = load_asset(s);
  while (eng_text != NULL) {
    retour_question = parse(eng_text);
    for (int question = 1; question <= retour_question.NBR_QUESTION;
         question++) {
      printf(" %d) %s\n", question,
             retour_question.questions[question].Question);
    }
    free(eng_text);
    numbr = get_choice("What do you choose?", "not valid try again",
                       retour_question.NBR_QUESTION);

    snprintf(s, sizeof(s), "%s_%03d.txt", language, numbr);
    eng_text = load_asset(s);
  }
}

int main(void) {
  char language[3]; // 2 characters string, including "\0"
  unsigned do_init_save = 0;
  char save_fn[SAVE_FILE_LENGTH]; // Save file name
  unsigned save_choice;
  save current_save;
  save available_saves[MAX_SAVES];
  // Language selection
  language_selection(language);

  // Save selection
  puts("Here are the following saves :"); // TODO: Not translated
  for (int i = 1; i < MAX_SAVES + 1; i++) {
    sprintf(save_fn, SAVE_FILE_FORMAT, i);
    if (save_exists(SAVE_PATH, save_fn)) {
      available_saves[i - 1] = load_save(SAVE_PATH, save_fn);
      printf(" %d) Username : %s - Time elapsed : %ds\n", i,
             available_saves[i].name,
             available_saves[0].time_elapsed); // TODO: Convert seconds into
                                               // hours, minutes and seconds
    } else {
      printf(" %d) New save\n", i);
    }
  }
  save_choice =
      get_choice("Chose a save [1-%d] : ", "Bad input. Try again.", MAX_SAVES);
  sprintf(save_fn, SAVE_FILE_FORMAT, save_choice);
  if (save_exists(SAVE_PATH, save_fn)) {
    current_save = available_saves[save_choice];
  } else {
    do_init_save = 1;
  }

  // Resume/start the game
  if (do_init_save) {
    get_name(current_save.name,
             "How do you want to name yourself ?\nEnter a name : ",
             "Do you want to name yourself '%s' ?\nThis is not reversible "
             "without creating a new save.\n 1) Yes\n 2) No\nChose [1-%d] : ",
             "Bad input. Try again."); // TODO: Not translated
    current_save.save_version = CURRENT_SAVE_VERSION;
    current_save.status = 000;
    current_save.time_elapsed = 0;

    save_save(SAVE_PATH, save_fn, current_save);
  }
  init_game(current_save, language);

  return 0;
}