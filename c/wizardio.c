#ifndef QT5

#include "wizardio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#define HISTORY_SIZE 100
#define MAX_COMMAND_LEN 64

#ifdef __linux__
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

// Terminal control for Linux
static struct termios orig_termios;

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

int getch() {
    int ch;
    enable_raw_mode();
    ch = getchar();
    disable_raw_mode();
    return ch;
}

#elif defined(_WIN32) || defined(__MSDOS__)
#include <conio.h>
#endif

typedef struct {
    char commands[HISTORY_SIZE][MAX_COMMAND_LEN];
    int current;
    int count;
    int browsing;
} CommandHistory;

static CommandHistory history = {{{0}}, 0, 0, 0};

void add_to_history(const char* cmd) {
    if (!cmd || !cmd[0]) return;
    
    // Don't add if same as last command
    if (history.count > 0 && 
        strcmp(history.commands[history.count - 1], cmd) == 0) {
        return;
    }
    
    if (history.count == HISTORY_SIZE) {
        for (int i = 0; i < HISTORY_SIZE - 1; i++) {
            strcpy(history.commands[i], history.commands[i + 1]);
        }
        history.count--;
    }
    
    strncpy(history.commands[history.count], cmd, MAX_COMMAND_LEN - 1);
    history.commands[history.count][MAX_COMMAND_LEN - 1] = '\0';
    history.count++;
    history.current = history.count;
}

void clear_line(int pos) {
    while (pos > 0) {
        print_message("\b \b");
        pos--;
    }
}

char* get_command_with_history(const char* prompt) {
    static char buffer[MAX_COMMAND_LEN];
    int pos = 0;
    
    print_message("%s", prompt);
    memset(buffer, 0, sizeof(buffer));
    history.browsing = 0;

    while (1) {
        int ch = getch();
        
        #ifdef __linux__
        if (ch == 27) {  // ESC sequence
            ch = getch();
            if (ch == '[') {
                ch = getch();
                if (ch == 'A') {  // Up arrow
                    ch = -2;  // Custom code for up
                } else if (ch == 'B') {  // Down arrow
                    ch = -3;  // Custom code for down
                }
            }
        }
        #else
        if (ch == 0 || ch == 224) {  // Extended key for DOS
            ch = getch();
            if (ch == 72) {  // Up arrow
                ch = -2;
            } else if (ch == 80) {  // Down arrow
                ch = -3;
            }
        }
        #endif

        // Handle up arrow
        if (ch == -2) {
            if (!history.browsing) {
                history.current = history.count - 1;
            } else if (history.current > 0) {
                history.current--;
            }
            
            if (history.count > 0) {
                clear_line(pos);
                strcpy(buffer, history.commands[history.current]);
                pos = strlen(buffer);
                print_message("%s", buffer);
                history.browsing = 1;
            }
            continue;
        }
        
        // Handle down arrow
        if (ch == -3) {
            if (history.browsing) {
                if (history.current < history.count - 1) {
                    history.current++;
                    clear_line(pos);
                    strcpy(buffer, history.commands[history.current]);
                    pos = strlen(buffer);
                    print_message("%s", buffer);
                } else {
                    clear_line(pos);
                    buffer[0] = '\0';
                    pos = 0;
                    history.browsing = 0;
                }
            }
            continue;
        }

        // Handle enter
        if (ch == '\r' || ch == '\n') {
            buffer[pos] = '\0';
            print_message("\n");
            add_to_history(buffer);
            return buffer;
        }
        
        // Handle backspace
        if ((ch == 127 || ch == 8) && pos > 0) {
            print_message("\b \b");
            pos--;
            buffer[pos] = '\0';
            continue;
        }
        
        // Handle regular character
        if (isprint(ch) && pos < MAX_COMMAND_LEN - 1) {
            buffer[pos++] = ch;
            print_message("%c", ch);
        }
    }
}

const char* get_user_input_main() {
    static char input_buffer[MAX_COMMAND_LEN];
    static const char* dr_command = "DR";

    char* input = get_command_with_history("ENTER YOUR COMMAND: ");
    strncpy(input_buffer, input, sizeof(input_buffer) - 1);
    input_buffer[sizeof(input_buffer) - 1] = '\0';

    // Convert to uppercase
    for (char* p = input_buffer; *p; ++p) {
        *p = toupper(*p);
    }

    if (strlen(input_buffer) >= 2 && 
        input_buffer[0] == 'D' && 
        input_buffer[1] == 'R') {
        return dr_command;
    }

    if (strchr("ADEFGHILMNOQSTUWYZ", input_buffer[0])) {
        input_buffer[1] = '\0';
        return input_buffer;
    }

    if (input_buffer[0] != '\0') {
        print_message("Invalid command. Type 'H' for help.\n");
    }
    return "";
}

int get_user_input_number() 
{
    int number;
    char input[100];

    while (1) {
        print_message("Enter a number: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            print_message_formatted("Error reading input. Please try again.\n");
            continue;
        }

        if (sscanf(input, "%d", &number) == 1) {
            return number;
        } else {
            print_message_formatted("Invalid input. Please enter a valid integer.\n");
        }
    }
}

// Input/Output functions
char get_user_input()
{
    char input[100];  // Buffer to store user input
    char command;

    while (1) {
        print_message_formatted("ENTER YOUR COMMAND: ");
        
        // Get user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle input error or EOF
            print_message_formatted("Error reading input. Please try again.\n");
            continue;
        }

        // Remove newline character if present
        input[strcspn(input, "\n")] = 0;

        // Convert input to uppercase
        for (int i = 0; input[i]; i++) {
            input[i] = toupper((unsigned char)input[i]);
        }

        // Check if input is empty
        if (strlen(input) == 0) {
            print_message_formatted("Please enter a command.\n");
            continue;
        }

        // Get the first character of the input
        command = input[0];

        // Check if it's a valid command
        if (strchr("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ", command) != NULL) {
            return command;
        } else {
            print_message("Invalid command. Type 'H' for help.\n");
        }
    }
}

char get_user_input_custom_prompt(char *prompt)
{
    char input[100];  // Buffer to store user input
    char command;

    while (1) {
        print_message("%s", prompt);
        
        // Get user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle input error or EOF
            print_message_formatted("Error reading input. Please try again.\n");
            continue;
        }

        // Remove newline character if present
        input[strcspn(input, "\n")] = 0;

        // Convert input to uppercase
        for (int i = 0; input[i]; i++) {
            input[i] = toupper((unsigned char)input[i]);
        }

        // Check if input is empty
        if (strlen(input) == 0) {
            print_message_formatted("Please enter a command.\n");
            continue;
        }

        // Get the first character of the input
        command = input[0];

        // Check if it's a valid command
        if (strchr("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ", command) != NULL) {
            return command;
        } else {
            print_message("Invalid command. Type 'H' for help.\n");
        }
    }
}


char get_user_input_yn()
{
    char input[100];  // Buffer to store user input
    char command;

    while (1) {
        
        // Get user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle input error or EOF
            print_message_formatted("Error reading input. Please try again.\n");
            continue;
        }

        // Remove newline character if present
        input[strcspn(input, "\n")] = 0;

        // Convert input to uppercase
        for (int i = 0; input[i]; i++) {
            input[i] = toupper((unsigned char)input[i]);
        }

        // Check if input is empty
        if (strlen(input) == 0) {
            print_message("Please Enter Y or N.\n");
            continue;
        }

        // Get the first character of the input
        command = input[0];

        // Check if it's a valid command
        if (strchr("YN", command) != NULL) {
            return command;
        } else {
            print_message("Invalid command. Type 'H' for help.\n");
        }
    }
}

void print_message_formatted(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    // First, format the string
    char buffer[1024];  // Assuming a maximum length, adjust as needed
    vsnprintf(buffer, sizeof(buffer), format, args);
    
    // Capitalize sentences
    capitalize_sentences(buffer);
    
    // Print the result
    printf("%s", buffer);
    
    va_end(args);
}

void print_message(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

#endif
