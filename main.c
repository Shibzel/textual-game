#include <stdio.h>
#include <stdlib.h>
#include "file_loader.h"
#include "file_printer.h"

int main() {
    printf("Hello world \n");
    char *eng_text = load_asset("english.txt");
    if (eng_text != NULL) {
        printf("yes loaded, mtn test de print \n");
        print_lines(eng_text);
        free(eng_text);
    }
}