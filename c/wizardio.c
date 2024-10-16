#include "wizardio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

const char* get_user_input_main() {
    static char input[100];  // Static buffer to store user input
    static char dr_command[] = "DR";  // Static buffer for "DR" command

    while (1) {
        print_message("ENTER YOUR COMMAND: ");
        // Get user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            print_message_formatted("Error reading input. Please try again.\n");
            continue;
        }
        // Remove newline character if present
        input[strcspn(input, "\n")] = '\0';
        // Convert input to uppercase
        for (int i = 0; input[i]; i++) {
            input[i] = (char)toupper((unsigned char)input[i]);
        }
        // Check if input is empty
        if (input[0] == '\0') {
            return input;  // Return empty string
        }
        // Get the first character
        char firstChar = input[0];
        // Validate commands
        if (firstChar == 'D' && input[1] == 'R') {
            return dr_command;  // Return "DR" for DRINK
        } else if (strchr("ADEFGHILMNOQSTUWYZ", firstChar) != NULL) {
            return input;  // Return the single letter command
        } else {
            print_message("Invalid command. Type 'H' for help.\n");
        }
    }
}



int get_user_input_number() 
{
    int number;
    char input[100];

    while (1) {
        print_message_formatted("Enter a number: ");
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
        if (strchr("1234567890ABCDEFGHILMNOQRSTUWY", command) != NULL) {
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
