#ifndef PARSER_H
#define PARSER_H

#define NUM_MAX_QUESTION 10

typedef struct {
    char* Question;
    int Code;
}question;

typedef struct {
    question questions[NUM_MAX_QUESTION];
    int NBR_QUESTION;
}tableau_question;


tableau_question parse(char *file_content);

#endif