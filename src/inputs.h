#ifndef INPUTS_H
#define INPUTS_H

// https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797 ANSI Escape Sequences
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

void appear(const char* text);
unsigned get_choice(char *display, char *bad_input, unsigned max_inputs);
char *get_name(char *name, char *display, char *confirmation_display, char *bad_input);
void output_c_by_c(const char *buffer);


#endif