#include <stdio.h>
#include <string.h>

#define NEW_LINE "\n"



void print_lines(char* text) {
    //Variable
    char buffer[1024];
    char *line;
    
    //Test
    if (text == NULL) return;

    
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0'; 
    line = strtok(buffer, NEW_LINE);
    while (line != NULL) {
        printf("%s \n",line);
        line = strtok ( NULL, NEW_LINE );
    }
}