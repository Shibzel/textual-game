#include <stdio.h>
#include "inputs.h"


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
    start_sound("assets/sounds/typing.wav");

    while (*buffer) {
        // Check if a key was pressed to skip
        if (!skip_delay && kbhit()) {
            int ch = getch();
            if (ch == 10 || ch == 13) { // Enter key
                skip_delay = 1;
                stop_sound(); 
            }
        }

        if (*buffer == '\r') {
            buffer++;
            continue;
        }

        printf("%c", *buffer);
        fflush(stdout); 
        
        if (!skip_delay) {
            SLEEP(85); 
        }
        buffer++;
    }

    if (!skip_delay) stop_sound();
    printf("\n");

    // CRITICAL: Clear the input buffer. 
    // This prevents the "Enter" you used to skip from triggering the next scanf.
    #ifdef _WIN32
        while (_kbhit()) _getch();
    #else
        tcflush(STDIN_FILENO, TCIFLUSH); 
    #endif
}

