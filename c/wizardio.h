#ifndef WIZARDIO_H
#define WIZARDIO_H

#include <stdbool.h>

// Input functions
const char* get_user_input_main(void);
char get_user_input(void);
char get_user_input_yn(void);
int get_user_input_number(void);

// Output functions
void print_message(const char *format, ...);
void print_message_formatted(const char *format, ...);
void printStars(void);

void capitalize_sentences(char *str);

char get_user_input_custom_prompt(char *prompt);

#endif // WIZARDIO_H
