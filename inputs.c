#include <stdio.h>
#include "inputs.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
    #include <mmsystem.h>
    #pragma comment(lib, "winmm.lib")
    #define SLEEP(ms) Sleep(ms)
#else
    #include <unistd.h>
    #include <signal.h>
    #define SLEEP(ms) usleep(ms * 1000)
#endif

#ifndef _WIN32
    static int audio_pid = -1;
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
    if (scanf("%u", &input) != 1 || input > max_inputs) {
      unvalid = 1;
      puts(bad_input);
    }
    printf("\n");
  } while (unvalid);

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

    // Start the loop before printing
    start_sound("typing.wav");

    while (*buffer) {
        if (*buffer == '\r') {
            buffer++;
            continue;
        }

        printf("%c", *buffer);
        fflush(stdout); 
        
        SLEEP(85); 
        buffer++;
    }

    // Stop the loop immediately when text is done
    stop_sound();
    printf("\n");
}