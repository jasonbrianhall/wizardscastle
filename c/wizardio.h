#ifndef WIZARDIO_H
#define WIZARDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

// Input functions
const char *get_user_input_main(void);
char get_user_input(void);
char get_user_input_yn(void);
int get_user_input_number(void);

// Output functions
void print_message(const char *format, ...);
void print_message_formatted(const char *format, ...);
void printStars(void);

void capitalize_sentences(char *str);

char get_user_input_custom_prompt(char *prompt);

#define NUM_THEMES 8
static const char* themes[] = {
    // Standard black on white
    "\033]10;#000000\007\033]11;#FFFFFF\007",
    // White on blue (classic IBM look)
    "\033]10;#FFFFFF\007\033]11;#0000AA\007",

    // Commodore 64 (light blue on dark blue)
    "\033]10;#6C9CF9\007\033]11;#0B0B79\007", 
    // Matrix (bright green on black)
    "\033]10;#00FF00\007\033]11;#000000\007",

    // Amber monochrome
    "\033]10;#FFB000\007\033]11;#000000\007",

    // Solarized Dark
    "\033]10;#839496\007\033]11;#002B36\007",

    // Dracula
    "\033]10;#F8F8F2\007\033]11;#282A36\007",

    // Nord
    "\033]10;#D8DEE9\007\033]11;#2E3440\007"
};

#ifdef __cplusplus
}
#endif

#endif // WIZARDIO_H
