#ifndef PARSER_H
#define PARSER_H

#define NUM_MAX_QUESTION 10

typedef struct {
    char* Question;
    int Code;
} question;

typedef struct {
    question questions[NUM_MAX_QUESTION];
    int NBR_QUESTION;
} tableau_question;


tableau_question parse_question_bloc(char *file_content);


char* get_line_content(char* file_content, int line_number);

#endif