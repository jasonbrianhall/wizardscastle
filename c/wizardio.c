#ifndef QT5

#include "wizardio.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HISTORY_SIZE 100
#define MAX_COMMAND_LEN 64

#define NUM_THEMES (sizeof(themes) / sizeof(themes[0]))
#define NUM_THEMES_BASIC (sizeof(themesBasic) / sizeof(themesBasic[0]))

int detect_terminal_capability(void);

#define TERM_CAP_MODERN  2
#define TERM_CAP_BASIC   1
#define TERM_CAP_SIMPLE  0

int detect_terminal_capability(void) {
    const char* term = getenv("TERM");
    const char* colorterm = getenv("COLORTERM");
    
    // Check for modern terminal support
    if (colorterm && (
        strcmp(colorterm, "truecolor") == 0 ||
        strcmp(colorterm, "24bit") == 0)) {
        return TERM_CAP_MODERN;
    }
    
    // Check for basic color support
    if (term && (
        strstr(term, "256color") ||
        strstr(term, "color") ||
        strcmp(term, "xterm") == 0 ||
        strcmp(term, "rxvt") == 0)) {
        return TERM_CAP_BASIC;
    }
    
    // Default to simple
    return TERM_CAP_SIMPLE;
}


const char* themes[] = {
     // Normal Theme 
    "\033]10;#CCCCCC\007\033]11;#000000\007",

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
    "\033]10;#D8DEE9\007\033]11;#2E3440\007",
};

const char* themesBasic[] = {
        "\033[37;40m",      // White on black
        "\033[30;47m",      // Black on white
        "\033[37;44m",      // White on blue
        "\033[36;44m",      // Cyan on blue
        "\033[32;40m",      // Green on black
        "\033[33;40m",      // Yellow on black
        "\033[36;40m",      // Cyan on black
};

#ifdef __linux__
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

// Terminal control for Linux
static struct termios orig_termios;

void enable_raw_mode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

int getch() {
  int ch;
  enable_raw_mode();
  ch = getchar();
  fflush(stdout);
  disable_raw_mode();
  return ch;
}

#elif defined(_WIN32) || defined(__MSDOS__)
#include <conio.h>
#endif

int current_theme=0;

void cycle_theme(int direction) {
#if !defined(__ANDROID__) && !defined(__MSDOS__)
    int terminaltype = detect_terminal_capability();
    if (terminaltype == TERM_CAP_MODERN) {
        current_theme = (current_theme + direction + NUM_THEMES) % NUM_THEMES;
        printf("%s", themes[current_theme]);
    }
    else {
        current_theme = (current_theme + direction + NUM_THEMES_BASIC) % NUM_THEMES_BASIC;
        printf("%s", themesBasic[current_theme]);
     }
    
    fflush(stdout);
#endif
}

typedef struct {
  char commands[HISTORY_SIZE][MAX_COMMAND_LEN];
  int current;
  int count;
  int browsing;
} CommandHistory;

static CommandHistory history = {{{0}}, 0, 0, 0};

void add_to_history(const char *cmd) {
  if (!cmd || !cmd[0])
    return;

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

char *get_command_with_history(const char *prompt) {
    static char buffer[MAX_COMMAND_LEN];
    int pos = 0;

    print_message("%s", prompt);
    memset(buffer, 0, sizeof(buffer));
    history.browsing = 0;

    while (1) {
        int ch = getch();
#ifdef __ANDROID__
        fflush(stdout);
#endif

#if defined(__MSDOS__) || defined(_WIN32)
	if (ch == 0 || ch == 224) {
        ch = getch();
            if (ch == 73) {         // Page Up
		cycle_theme(1);
                continue;
            } else if (ch == 81) {  // Page Down
                cycle_theme(-1);
                continue;
            } else if (ch == 72) {  // Up arrow
                ch = -2;
            } else if (ch == 80) {  // Down arrow
                ch = -3;
            }
        }
#else
        if (ch == 27) {
            ch = getch();
#ifdef __ANDROID__
            fflush(stdout);
#endif
            if (ch == '[') {
                ch = getch();
#ifdef __ANDROID__
                fflush(stdout);
#endif
		if (ch == 91)
		{
                    ch = getch();
		    if (ch == 69 && getch()=='e')
	            {
  			cycle_theme(1);
		    }
		}
		else if (ch == '1') {
    			ch = getch();
		    if (ch == '5' && getch() == '~') {  // F5
                        cycle_theme(1);
                        continue;
                    } else if (ch == '7' && getch() == '~') {  // F6
                        cycle_theme(-1);
                        continue;
                    }
                } else if (ch == 'A') {  // Up arrow
                    ch = -2;
                } else if (ch == 'B') {  // Down arrow
                    ch = -3;
                }
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
#ifndef __ANDROID__
            print_message("%c", ch);
#endif
        }
    }
}

const char *get_user_input_main() {
  static char input_buffer[MAX_COMMAND_LEN];
  static const char *dr_command = "DR";
  static const char *qs_command = "QS";
  static const char *ql_command = "QL";

  char *input = get_command_with_history("ENTER YOUR COMMAND: ");
  strncpy(input_buffer, input, sizeof(input_buffer) - 1);
  input_buffer[sizeof(input_buffer) - 1] = '\0';

  // Convert to uppercase
  for (char *p = input_buffer; *p; ++p) {
    *p = toupper(*p);
  }

  if (strlen(input_buffer) >= 2 && input_buffer[0] == 'D' &&
      input_buffer[1] == 'R') {
    return dr_command;
  }

  if (strlen(input_buffer) >= 2 && input_buffer[0] == 'Q' &&
      input_buffer[1] == 'S') {
    return qs_command;
  }

  if (strlen(input_buffer) >= 2 && input_buffer[0] == 'Q' &&
      input_buffer[1] == 'L') {
    return ql_command;
  }

  if (strchr("ABCDEFGHIJKLMNOPQRSTUVWYZ", input_buffer[0])) {
    input_buffer[1] = '\0';
    return input_buffer;
  }

  if (input_buffer[0] != '\0') {
    print_message("Invalid command. Type 'H' for help.\n");
  }
  return "";
}

int get_user_input_number() {
  static const char *invalid_message =
      "Invalid input. Please enter a valid integer.\n";
  char *input;
  int number;

  while (1) {
    // Assuming get_command_with_history() handles the input similarly to the
    // original function
    input = get_command_with_history("Enter a number: ");

    // Attempt to parse input as an integer
    if (sscanf(input, "%d", &number) == 1) {
      return number;
    } else {
      print_message(invalid_message);
    }
  }
}

// Input/Output functions
char get_user_input() {
  static const char *invalid_message = "Invalid command. Type 'H' for help.\n";
  static const char *empty_message = "Please enter a command.\n";
  char *input;
  char command;

  while (1) {
    // Prompt and get user input with history support
    input = get_command_with_history("ENTER YOUR COMMAND: ");

    // Convert input to uppercase
    for (int i = 0; input[i]; i++) {
      input[i] = toupper((unsigned char)input[i]);
    }

    // Check if input is empty
    if (strlen(input) == 0) {
      print_message(empty_message);
      continue;
    }

    // Get the first character of the input
    command = input[0];

    // Check if the first character is a valid command
    if (strchr("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ", command) != NULL) {
      return command;
    } else {
      print_message(invalid_message); // Invalid command message
    }
  }
}

char get_user_input_custom_prompt(char *prompt) {
  static const char *invalid_message = "Invalid command. Type 'H' for help.\n";
  static const char *empty_message = "Please enter a command.\n";
  char *input;
  char command;

  while (1) {
    // Prompt and get user input with history support
    input = get_command_with_history(prompt);

    // Convert input to uppercase
    for (int i = 0; input[i]; i++) {
      input[i] = toupper((unsigned char)input[i]);
    }

    // Check if input is empty
    if (strlen(input) == 0) {
      print_message(empty_message);
      continue;
    }

    // Get the first character of the input
    command = input[0];

    // Check if the first character is a valid command
    if (strchr("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ", command) != NULL) {
      return command;
    } else {
      print_message(invalid_message); // Invalid command message
    }
  }
}

char get_user_input_yn() {
  static const char *invalid_message =
      "Invalid command. Please enter 'Y' or 'N'.\n";
  static const char *empty_message = "Please Enter Y or N.\n";
  char *input;
  char command;

  while (1) {
    // Prompt and get user input with history support
    input = get_command_with_history("");

    // Convert input to uppercase
    for (int i = 0; input[i]; i++) {
      input[i] = toupper((unsigned char)input[i]);
    }

    // Check if input is empty
    if (strlen(input) == 0) {
      print_message(empty_message);
      continue;
    }

    // Get the first character of the input
    command = input[0];

    // Check if the first character is 'Y' or 'N'
    if (command == 'Y' || command == 'N') {
      return command;
    } else {
      print_message(invalid_message); // Invalid command message
    }
  }
}

void print_message_formatted(const char *format, ...) {
  va_list args;
  va_start(args, format);

  // First, format the string
  char buffer[1024]; // Assuming a maximum length, adjust as needed
  vsnprintf(buffer, sizeof(buffer), format, args);

  // Capitalize sentences
  capitalize_sentences(buffer);

  // Print the result
  printf("%s", buffer);
#ifdef __ANDROID__
  fflush(stdout);
#endif
  va_end(args);
}

void print_message(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
#ifdef __ANDROID__
  fflush(stdout);
#endif
}

#endif
