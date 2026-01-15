#include "inputs.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    
    memset(&returned_quests, 0, sizeof(tableau_question)); // Initialize to zero
    
    // Start
    is_question_mode = false;
    question_nbr = 0;
    printf("DEBUG: Starting parse_question_bloc\n");
    
    // Loop until the end of the string
    while (*ptr != '\0') {
        if (sscanf(ptr, "%[^\n]", line) != 1) {
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\r') {
            line[len - 1] = '\0';
        }
        
        
        if (strstr(line, DELIMITER) != NULL) {
            is_question_mode = !is_question_mode;
        } else {
            if (is_question_mode == false) {
                output_c_by_c(line);
            } else {
                char text_part[512];
                char code_part[512];
                if (sscanf(line, "%[^;]; %[^\n]", text_part, code_part) == 2) {
                    question_nbr++;
                    
                    returned_quests.questions[question_nbr].Question = malloc(strlen(text_part) + 1);
                    strcpy(returned_quests.questions[question_nbr].Question, text_part);
                    returned_quests.questions[question_nbr].Code = atoi(code_part);
                } else {
                    printf("%s\n", line);
                }
            }
        }
        
        char *newline_pos = strchr(ptr, '\n');
        if (newline_pos != NULL) {
            ptr = newline_pos + 1;
        } else {
            break;
        }
    }
    
    returned_quests.NBR_QUESTION = question_nbr;
    
    return returned_quests;
}
const char* get_line_content(const char* file_content, int line_number) {
    if (file_content == NULL || line_number <= 0) return NULL;

    const char* current = file_content;
    const char* line_start = current;
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