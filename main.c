#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Pour strcpy

#include "file_loader.h"
#include "parser.h"

#define SAVE_FP "./saves/"
#define ASSETS_FP "./assets/"
#define ASSET_FILE_FORMAT "%s_%d.txt"
#define SAVE_FILE_FORMAT "save_%d.txt"
#define MAX_NAME_SIZE 32

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

int save_exists(char *assets_path) {
  // TODO: Implement
}

void init_save(char *save_path, char *name) {
  mkdir_if_not_exists(save_path);

  // TODO: Implement
}

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

char *get_name(char *name, char *display, char *bad_input) {
  unsigned unvalid;

  do {
    unvalid = 0;

    puts(display);
    while (getchar() == '\n')
      ;
    fgets(name, MAX_NAME_SIZE, stdin);
    printf("Do you want to name yourself '%s' ?",
           name); // TODO: Untranslated text
    if (get_choice("This is not reversible without creating a new save.\n 1) "
                   "Yes\n2) No\nChose [1-%d] : ",
                   "Bad input. Try again.", 2) == 2) {
      unvalid = 1;
    }
  } while (unvalid);

  return name;
}

int main(void) {
  char language[3];             // 2 characters string, including "\0"
  char name[MAX_NAME_SIZE + 1]; // 32 char max
  tableau_question retour_question;
  // Language selection
  language_selection(language);

  // Save selection
  get_name(name, "How do you want to name yourself ?", "");
  // TODO: Implement

  // Resume/start the game
  // TODO: Implement

  char *eng_text = load_asset("en_000.txt");
  int numbr;
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
    char s[100];
    snprintf(s, sizeof(s), "en_%03d.txt", numbr);
    eng_text = load_asset(s);
  }
  return 0;
}