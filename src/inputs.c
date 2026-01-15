#include <stdio.h>
#include "inputs.h"


#include <stdio.h>

unsigned get_choice(char *display, char *bad_input, unsigned max_inputs) {
  unsigned input, unvalid;

  do {
    unvalid = 0;

    printf(display, max_inputs);
    if (scanf("%u", &input) != 1 || 1 > input || input > max_inputs) {
      unvalid = 1;
      puts(bad_input);
      while (getchar() != '\n');
    }
    printf("\n");
  } while (unvalid);
  while (getchar() != '\n');

  return input;
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