#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "wizards-castle.h"
#include "player.h"
#include "gamestate.h"

// Global Qt window and widgets
QMainWindow *mainWindow;
QTextEdit *textArea;
QLineEdit *inputField;
QPushButton *submitButton;

// Function to handle user input
void handleUserInput() {
    QString input = inputField->text();
    inputField->clear();
    // Process the input (you'll need to implement this based on your game logic)
    // For now, just echo the input to the text area
    textArea->append("You entered: " + input);
}

// Redirect print_message to the Qt window
void print_message(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // Append the message to the text area
    textArea->append(QString::fromUtf8(buffer));
}

// Similarly, redirect print_message_formatted
void print_message_formatted(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // Capitalize sentences (you'll need to implement this function)
    capitalize_sentences(buffer);

    // Append the formatted message to the text area
    textArea->append(QString::fromUtf8(buffer));
}

// Function to get user input (you'll need to implement the logic to wait for input)
char get_user_input() {
    // This is a placeholder. You'll need to implement a way to wait for user input
    // and return the first character of the input.
    return ' ';
}

char get_user_input_yn() {
    // Similar to get_user_input, but should only accept 'Y' or 'N'
    return 'Y';
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create main window
    mainWindow = new QMainWindow();
    mainWindow->setWindowTitle("Wizard's Castle");
    mainWindow->resize(800, 600);

    // Create central widget and layout
    QWidget *centralWidget = new QWidget(mainWindow);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Create text area
    textArea = new QTextEdit(centralWidget);
    textArea->setReadOnly(true);
    layout->addWidget(textArea);

    // Create input field
    inputField = new QLineEdit(centralWidget);
    layout->addWidget(inputField);

    // Create submit button
    submitButton = new QPushButton("Submit", centralWidget);
    layout->addWidget(submitButton);

    // Connect input handling
    QObject::connect(submitButton, &QPushButton::clicked, handleUserInput);
    QObject::connect(inputField, &QLineEdit::returnPressed, handleUserInput);

    mainWindow->setCentralWidget(centralWidget);

    // Show the window
    mainWindow->show();

    // Game initialization
    Player player;
    GameState game;
    bool playagain = true;
    bool debug_mode = false;

    // Check for debug flag
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--debug") == 0) {
            debug_mode = true;
            break;
        }
    }

    // Use a QTimer to start the game loop after the event loop has started
    QTimer::singleShot(0, [&]() {
        while (playagain) 
        {
            initialize_player(&player);
            print_introduction();

            if (!debug_mode) {
                choose_race(&player);
                print_message("\n");
                choose_sex(&player);
                allocate_attributes(&player);
                buy_equipment(&player);
                buy_lamp_and_flares(&player);
            } else {
                // Debug mode setup (same as in the original main.c)
                player.race = 2;  // Elf
                player.sex = 1;   // Male
                player.strength = 18;
                player.intelligence = 18;
                player.dexterity = 18;
                player.gold = 10000;
                player.flares = 1000;
                player.lamp_flag = 1;
                player.runestaff_flag = 1;
                player.weapon_type = 3;  // Sword
                player.armor_type = 3;   // Plate (assuming this is the best armor, equivalent to "Shield")
                player.armor_points = 21; // Max armor points for Plate armor

                print_message("DEBUG MODE: You are a male elf with 18 Strength, 18 Intelligence, and 18 Dexterity.\n");
                print_message("DEBUG MODE: You have a Sword and Plate armor.\n");
                print_message("DEBUG MODE: You start with the Runestaff, 10000 gold, 1000 flares, and a lamp.\n");
                print_message("DEBUG MODE: All rooms are discovered\n");
            }

            generate_castle(&game);

            if (debug_mode) {
                print_message("DEBUG MODE: The Orb of Zot is located at (%d, %d) on level %d.\n", 
                       game.orb_location[1], game.orb_location[0], game.orb_location[2]);
                for (int q = 0; q < MAP_SIZE; q++) {
                    game.discovered_rooms[q] = 1;  // 0 means undiscovered
                }
            }

            playagain = main_game_loop(&player, &game);

            // Process Qt events to update the window
            app.processEvents();
        }
    });

    return app.exec();
}
