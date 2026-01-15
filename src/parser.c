#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "inputs.h"

// Constants
#define DELIMITER "---"
#define NUM_MAX_QUESTION 10
#define NEW_LINE "\n"

typedef struct {
    char* Question;
    int Code;
} question;

typedef struct {
    question questions[NUM_MAX_QUESTION];
    int NBR_QUESTION;
} tableau_question;

tableau_question parse_question_bloc(char *file_content) {
    
    // Sanity Check
    if (file_content == NULL)
        return (tableau_question){0};

    // Variables
    char *ptr = file_content;
    char line[1024];
    bool is_question_mode;
    int question_nbr;
    tableau_question returned_quests;

    // Start
    is_question_mode = false;
    question_nbr = 0;

    // Loop until the end of the string
    while (*ptr != '\0') {

        // ! Parse the  line '%[^\n ]' taken from file extractionexample given in
        // SAE_jeu_mode_texte_anglais.pdf
        if (sscanf(ptr, "%[^\n]", line) != 1) {
            break; // End of content reached
        }

        //? Detect special zone
        if (strstr(line, DELIMITER) != NULL) {
            is_question_mode = !is_question_mode;
            question_nbr = 0;
        } else {
            if (is_question_mode == false) {
                output_c_by_c(line);
            } else {
                char text_part[512];
                char code_part[512];

                //  %[^;] reads everything up to ';' https://koor.fr/C/cstdio/fscanf.wp
                // ; matches the literal semicolon
                // " %[^\n]" reads the rest (skipping space after semi)
                if (sscanf(line, "%[^;]; %[^\n]", text_part, code_part) == 2) {
                    question_nbr++;
                    returned_quests.questions[question_nbr].Question = malloc(strlen(text_part) + 1);
                    strcpy(returned_quests.questions[question_nbr].Question, text_part);

                    returned_quests.questions[question_nbr].Code = atoi(code_part);
                    returned_quests.NBR_QUESTION = question_nbr;
                } else {
                    // In case we forgot ; we just print the line anyway
                    printf("%s\n", line);
                }
            }
        }

        // Next line
        // We find the newline character starting from our current position
        char *newline_pos = strchr(ptr, '\n');

        if (newline_pos != NULL) {
            // Move pointer past the current newline
            ptr = newline_pos + 1;
        } else {
            // No more newlines found, we are done
            break;
        }
    }

    return returned_quests;
}


char* get_line_content(char* file_content, int line_number) {
    if (file_content == NULL || line_number <= 0) return NULL;

    char* current = file_content;
    char* line_start = current;
    int current_line = 1;

    while (*current != '\0') {
        // Assuming NEW_LINE is defined, otherwise just use '\n'
        if (*current == '\n') { 
            if (current_line == line_number) {
                return line_start;
            }
            line_start = current + 1;
            current_line++;
        }
        current++;
    }

    // FIX: Removed the "&& line_start != file_content" check.
    // That check made it fail if the file only had 1 line.
    if (current_line == line_number) {
        return line_start;
    }

    return NULL;
}


void extract_text(char *file_content, int line_number, char *buffer, size_t buffer_size) {
    char *raw_line = get_line_content(file_content, line_number);
    
    if (!raw_line) {
        snprintf(buffer, buffer_size, "[MISSING TEXT LINE %d]", line_number);
        return;
    }

    size_t i = 0; // Index for buffer
    size_t j = 0; // Index for raw_line

    while (i < buffer_size - 1 && raw_line[j] != '\0' && raw_line[j] != '\n') {
        
        // Skip Carriage Returns (\r) so the cursor doesn't reset to the left
        if (raw_line[j] == '\r') {
            j++;
            continue;
        }

        // Handle literal "\n" (backslash + n)
        if (raw_line[j] == '\\' && raw_line[j + 1] == 'n') {
            buffer[i++] = '\n';
            j += 2; 
        } else {
            buffer[i++] = raw_line[j++];
        }
    }
    
    buffer[i] = '\0';
}