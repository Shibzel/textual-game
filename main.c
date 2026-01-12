#include <stdio.h>
#include <stdlib.h>
#include "file_loader.h"
#include "file_printer.h"


unsigned get_choice(char *display, char *bad_input, unsigned max_inputs) {
    unsigned input, unvalid;

    do {
        unvalid = 0;

        printf(display, max_inputs);
        if (scanf("%d", &input) != 1 || input > max_inputs) {
            unvalid = 1;
            puts(bad_input);
        }
        printf("\n");
    } while (unvalid);
    
    return input;
}

int main() {
    printf("Hello Let's test \n");
    char *eng_text = load_asset("en_000.txt");
    if (eng_text != NULL) {
        printf("yes loaded, mtn test de print \n");
        print_lines(eng_text);
        free(eng_text);
    }
    puts("Hello world!");

    get_choice("What do you do ? [1-%d] : ", "Bad input. Try again.", 5);
}