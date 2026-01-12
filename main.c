#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // Pour strcpy

#include "file_loader.h"
#include "file_printer.h"


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

int main(void) {
    unsigned choice;
    char language[3];

    choice = get_choice(
        "Please chose a language / Merci de choisir un langage :\n 1) English\n 2) French\nInput a value / Entrez une valeur [1-2] : ",
        "Bad input. Try again",
        2
    );
    switch (choice) {
        case 1:
            strcpy(language, "en");
            break;
        case 2:
            strcpy(language, "fr");
            break;
    }

    // char *eng_text = load_asset("english.txt");
    // if (eng_text != NULL) {
    //     printf("yes loaded, mtn test de print \n");
    //     print_lines(eng_text);
    //     free(eng_text);
    // }
    // puts("Hello world!");

    // get_choice("What do you do ? [1-%d] : ", "Bad input. Try again.", 5);
}