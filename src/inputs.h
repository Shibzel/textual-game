#ifndef INPUTS_H
#define INPUTS_H



unsigned get_choice(char *display, char *bad_input, unsigned max_inputs);
char *get_name(char *name, char *display, char *confirmation_display, char *bad_input);
void output_c_by_c(const char *buffer);


#endif