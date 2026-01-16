#include <stdio.h>
#include "inputs.h"
#define MAX_TERMINAL_WIDTH 80 
#define BUF_SIZE 1024 * 1024 // 1MB Buffer

#ifdef _WIN32
    #include <windows.h>
    #include <mmsystem.h>
    #include <conio.h>
    #pragma comment(lib, "winmm.lib")
    #define SLEEP(ms) Sleep(ms)
#else
    #include <unistd.h>
    #include <signal.h>
    #include <termios.h>
    #include <fcntl.h>
    #define SLEEP(ms) usleep(ms * 1000)
    static int audio_pid = -1;
#endif

// --- CROSS-PLATFORM INPUT HELPERS ---

#ifndef _WIN32
// Linux version of kbhit: checks if a key is waiting in the buffer
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

// Linux version of getch: reads one char without waiting for Enter
int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#else
    // Windows already has _kbhit() and _getch() in conio.h
    #define kbhit _kbhit
    #define getch _getch
#endif

void start_sound(const char* filename) {
#ifdef _WIN32
    // SND_ASYNC: Don't block the code
    // SND_LOOP: Keep playing until stopped
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
#else
    // Start aplay in background and get its PID
    char command[256];
    snprintf(command, sizeof(command), "aplay -q %s & echo $!", filename);
    FILE *pipe = popen(command, "r");
    if (pipe) {
        fscanf(pipe, "%d", &audio_pid);
        pclose(pipe);
    }
#endif
}

void stop_sound() {
#ifdef _WIN32
    PlaySound(NULL, NULL, 0); // Stops any playing sound
#else
    if (audio_pid > 0) {
        char kill_cmd[32];
        sprintf(kill_cmd, "kill %d > /dev/null 2>&1", audio_pid);
        system(kill_cmd);
        audio_pid = -1;
    }
#endif
}

void appear(const char* text) {
    size_t len = strlen(text);
    if (len == 0) return;
    char *string;

#ifdef _WIN32
    // Windows: Direct System Call to the Console Handle
    // This is much faster than printf() or puts()
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    DWORD written;
    WriteFile(hOut, text, (DWORD)len, &written, NULL);
    puts("");
#else
    // Linux/Unix: Direct Write System Call
    // This bypasses the C standard library buffers entirely
    write(STDOUT_FILENO, text, len);
    puts("");
#endif
}

unsigned get_choice(char *display, char *bad_input, unsigned max_inputs) {
  unsigned input, unvalid;

  do {
    unvalid = 0;

    printf(display, max_inputs);
    if (scanf("%u", &input) != 1 || 1 > input || input > max_inputs) {
      unvalid = 1;
      puts(bad_input);
      while (getchar() != '\n');
    }
    printf("\n");
  } while (unvalid);
  while (getchar() != '\n');

  return input;
}

char *get_name(char *name, char *display, char *confirmation_display,
               char *bad_input) {
  unsigned unvalid;
  const unsigned MAX_INPUTS = 2;

  do {
    unvalid = 1;

    printf("%s", display);
    if (scanf("%s", name) == 1) {
      printf(confirmation_display, name, MAX_INPUTS);
      if (get_choice("", bad_input, MAX_INPUTS) == 1) {
        unvalid = 0;
      }
    }
  } while (unvalid);

  return name;
}


void output_c_by_c(const char *buffer) {
    if (buffer == NULL) return;

    int skip_delay = 0;
    int current_col = 0; // Track column to handle line wraps
    start_sound("assets/sounds/typing.wav");

    while (*buffer) {
        // Check for Skip Input
        if (!skip_delay && kbhit()) {
            int ch = getch();
            if (ch == 10 || ch == 13 || ch == ' ') { 
                skip_delay = 1;
                stop_sound();
            }
        }

        // UTF-8 Character Handling 
        int char_len = 1;
        if ((unsigned char)*buffer > 127) {
            if ((*buffer & 0xE0) == 0xC0) char_len = 2;
            else if ((*buffer & 0xF0) == 0xE0) char_len = 3;
            else if ((*buffer & 0xF8) == 0xF0) char_len = 4;
        }

        //  Print the character(s)
        for (int i = 0; i < char_len; i++) {
            putchar(buffer[i]);
        }

        //  Update column tracking
        if (*buffer == '\n') {
            current_col = 0;
        } else {
            current_col++;
        }

        // Retro Cursor Logic using ANSI Escape Codes
        // Only show cursor if we aren't at the very edge of the terminal
        if (!skip_delay && *buffer != '\n' && current_col < MAX_TERMINAL_WIDTH - 1) {
            // \033[s = Save cursor position
            // \033[u = Restore cursor position
            printf("\033[s_\033[u"); 
            fflush(stdout);
            SLEEP(15);
            // Overwrite the cursor with a space then restore again
            printf("\033[s \033[u"); 
            fflush(stdout);
        } else {
            fflush(stdout);
        }

        //  Dynamic Punctuation Delay
        if (!skip_delay) {
            int delay = 25 + (rand() % 15); // Faster base speed

            if (*buffer == '.' || *buffer == '!' || *buffer == '?') {
                delay = 300; 
            } else if (*buffer == ',' || *buffer == ':') {
                delay = 100; 
            }
            
            SLEEP(delay);
        }

        buffer += char_len;
    }

    if (!skip_delay) stop_sound();
    printf("\n");

    // Clear input buffer
    #ifdef _WIN32
        while (_kbhit()) _getch();
    #else
        tcflush(0, TCIFLUSH);
    #endif
}