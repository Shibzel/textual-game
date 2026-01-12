#include <stdio.h>

#include "file_loader.h"


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
    puts("Hello world!");

    get_choice("What do you do ? [1-%d] : ", "Bad input. Try again.", 5);
}