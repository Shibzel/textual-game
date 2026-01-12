#include <stdio.h>
#include "file_loader.h"

int main() {
    printf("Hello world \n");
    char *eng_text = load_asset("english.txt");
}