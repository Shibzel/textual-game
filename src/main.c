#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Pour strcpy

#include "files.h"
#include "parser.h"
#include "save_manager.h"
#include "inputs.h"

typedef unsigned items[ITEMS_MAX];


#define SAVE_PATH "./saves/"
#define ASSETS_FP "./assets/"
#define ASSET_FILE_FORMAT "%s_%d.txt"
#define SAVE_FILE_FORMAT "save_%d.txt"
#define SAVE_FILE_LENGTH 11 // meh
#define MAX_NAME_SIZE 256
#define MAX_SAVES 3
#define CURRENT_SAVE_VERSION 1
#define ITEMS_MAX 100
#define TEXT_MAX_SIZE 2048



void extract_text(const char *file_content, int line_number, char *buffer, size_t buffer_size) {
    const char *raw_line = get_line_content(file_content, line_number);
    
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

void process_save_menu(const char *lang_content, save *available_saves, save *current_save, int unsigned *do_init_save) {
    char save_fn[50];
    char text_buf_header[256];
    char text_buf_item[256];
    char text_buf_prompt[256];
    char text_buf_error[256];
    char text_buf_name_q[256];
    char text_buf_confirm[512];

    // Added buffer for formatting strings before passing to puts
    char display_buffer[512]; 
    
    int save_choice = 0;
    
  
    extract_text(lang_content, 3, text_buf_header, sizeof(text_buf_header));
    puts(text_buf_header); 

    for (int i = 1; i < MAX_SAVES + 1; i++) {
        sprintf(save_fn, SAVE_FILE_FORMAT, i);
        
        if (save_exists(SAVE_PATH, save_fn)) {
            available_saves[i - 1] = load_save(SAVE_PATH, save_fn);
            
            int total_seconds = available_saves[i - 1].time_elapsed; 
            int hours = total_seconds / 3600;
            int minutes = (total_seconds % 3600) / 60;
            int seconds = total_seconds % 60;

            extract_text(lang_content, 4, text_buf_item, sizeof(text_buf_item));
            snprintf(display_buffer, sizeof(display_buffer), text_buf_item, i, available_saves[i - 1].name, hours, minutes, seconds);
            puts(display_buffer);

        } else {
            extract_text(lang_content, 5, text_buf_item, sizeof(text_buf_item));
            snprintf(display_buffer, sizeof(display_buffer), text_buf_item, i);
            puts(display_buffer);
        }
    }

    extract_text(lang_content, 6, text_buf_prompt, sizeof(text_buf_prompt)); 

    char formatted_error[256];
    extract_text(lang_content, 1, text_buf_error, sizeof(text_buf_error));
    snprintf(formatted_error, sizeof(formatted_error), text_buf_error, MAX_SAVES);

    save_choice = get_choice(text_buf_prompt, formatted_error, MAX_SAVES);

    sprintf(save_fn, SAVE_FILE_FORMAT, save_choice);
    if (save_exists(SAVE_PATH, save_fn)) {
        *current_save = available_saves[save_choice - 1]; 
        *do_init_save = 0;
    } else {
        *do_init_save = 1;
    }



    if (*do_init_save) {
        extract_text(lang_content, 7, text_buf_prompt, sizeof(text_buf_confirm));
        extract_text(lang_content, 8, text_buf_confirm, sizeof(text_buf_confirm));

        get_name(current_save->name, text_buf_prompt, text_buf_confirm, formatted_error);
        current_save->save_version = CURRENT_SAVE_VERSION;
        current_save->status = 0;
        current_save->time_elapsed = 0;
        save_save(SAVE_PATH, save_fn, *current_save);
    }
}

void language_selection(char *language) {
    unsigned choice;


    choice = get_choice(
        "Please chose a language / Merci de choisir un langage :\n 1) English\n "
        "2) Français\nInput a value / Entrez une valeur [1-2] : ",
        "Bad input. Try again / Mauvaise entrée. Réessayez", 2);
    switch (choice) {
    case 1:
        strcpy(language, "en");
        break;
    case 2:
        strcpy(language, "fr");
        break;
    }
}


void init_game(save current_save, char language[3]) {
    tableau_question retour_question;

    char *eng_text;
    int numbr;
    char s[100];

    numbr = 0;
    snprintf(s, sizeof(s), "%s_%03d.txt", language, numbr);
    eng_text = load_asset(s);
    while (eng_text != NULL) {
        retour_question = parse_question_bloc(eng_text);
        for (int question = 1; question <= retour_question.NBR_QUESTION;
            question++) {
        printf(" %d) %s\n", question,
                retour_question.questions[question].Question);
        }
        free(eng_text);
        numbr = get_choice("What do you choose?", "not valid try again",
                        retour_question.NBR_QUESTION);

        snprintf(s, sizeof(s), "%s_%03d.txt", language, numbr);
        eng_text = load_asset(s);
    }
}

// int main(void) {
//     items current_items = {12, 3, 5, 7, 0};
//     char *text = items_parse_to_file(current_items);
//     puts(text);
//     free(text);
//     return 0;
// }

int main(void) {
    char language[3]; // 2 characters string, including "\0"
    unsigned do_init_save = 0;
    char save_fn[SAVE_FILE_LENGTH]; // Save file name
    unsigned save_choice;
    save current_save;
    save available_saves[MAX_SAVES];
    // Language selection
    language_selection(language);


    //// Save selection
    //puts("Here are the following saves :"); // TODO: Not translated
    //for (int i = 1; i < MAX_SAVES + 1; i++) {
    //    sprintf(save_fn, SAVE_FILE_FORMAT, i);
    //    if (save_exists(SAVE_PATH, save_fn)) {
    //        available_saves[i - 1] = load_save(SAVE_PATH, save_fn);
    //        printf(" %d) Username : %s - Time elapsed : %ds\n", i,
    //               available_saves[i].name,
    //               available_saves[0].time_elapsed); // TODO: Convert seconds into
    //                                                 // hours, minutes and seconds
    //    } else {
    //        printf(" %d) New save\n", i);
    //    }
    //}
    //save_choice =
    //    get_choice("Chose a save [1-%d] : ", "Bad input. Try again.", MAX_SAVES);
    //sprintf(save_fn, SAVE_FILE_FORMAT, save_choice);
    //if (save_exists(SAVE_PATH, save_fn)) {
    //    current_save = available_saves[save_choice];
    //} else {
    //    do_init_save = 1;
    //}
//
    //// Resume/start the game
    //if (do_init_save) {
    //    get_name(current_save.name,
    //             "How do you want to name yourself ?\nEnter a name : ",
    //             "Do you want to name yourself '%s' ?\nThis is not reversible "
    //             "without creating a new save.\n 1) Yes\n 2) No\nChose [1-%d] : ",
    //             "Bad input. Try again."); // TODO: Not translated
    //    current_save.save_version = CURRENT_SAVE_VERSION;
    //    current_save.status = 000;
    //    current_save.time_elapsed = 0;
//
    //    save_save(SAVE_PATH, save_fn, current_save);
    //}
    char* file_content = load_asset(!strcmp("en", language) ? "en_main.txt" : "fr_main.txt");
    process_save_menu(file_content, available_saves, &current_save, &do_init_save);
    
    init_game(current_save, language);

    return 0;
}