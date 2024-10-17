#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QThread>
#include <QTimer>
#include <QScrollBar>
#include <QCloseEvent>
#include <QMessageBox>
#include <atomic>
#include <cstdlib>
#include <cstdarg>

extern "C" {
#include "wizards-castle.h"
#include "player.h"
#include "gamestate.h"
#include "utilities.h"
#include "combat.h"
#include "vendor.h"
#include "wizardio.h"
}

std::atomic<bool> g_gameFinished(false);

class GameThread : public QThread {
public:
    void run() override {
        Player player;
        GameState game;
        bool playagain = true;
        bool debug_mode = false;
        int q;

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
                // Debug mode setup
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
                player.armor_type = 3;   // Plate
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
                for (q = 0; q < MAP_SIZE; q++) {
                    game.discovered_rooms[q] = 1;  // 0 means undiscovered
                }
            }

            playagain = main_game_loop(&player, &game);
        }

        g_gameFinished = true;
    }
};

class WizardsCastleWindow : public QMainWindow {
    Q_OBJECT

public:
    WizardsCastleWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Wizard's Castle");
        resize(800, 600);

        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        outputText = new QTextEdit(this);
        outputText->setReadOnly(true);
        layout->addWidget(outputText);

        inputLine = new QLineEdit(this);
        layout->addWidget(inputLine);

        setCentralWidget(centralWidget);

        connect(inputLine, &QLineEdit::returnPressed, this, &WizardsCastleWindow::processInput);

        gameThread = new GameThread();
        gameThread->start();

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &WizardsCastleWindow::checkGameFinished);
        timer->start(100);  // Check every 100 ms
    }

    ~WizardsCastleWindow() {
        if (gameThread->isRunning()) {
            gameThread->terminate();
            gameThread->wait();
        }
        delete gameThread;
    }

    void appendToOutput(const QString& text) {
        outputText->moveCursor(QTextCursor::End);
        outputText->insertPlainText(text);
        outputText->verticalScrollBar()->setValue(outputText->verticalScrollBar()->maximum());
    }

    void clearInput() {
        inputLine->clear();
        lastInput.clear();
    }

    bool inputIsEmpty() const {
        return lastInput.empty();
    }

    std::string getLastInput() {
        std::string input = lastInput;
        lastInput.clear();
        return input;
    }

protected:
    void closeEvent(QCloseEvent *event) override {
        QMessageBox::information(this, "Close Event", "A close event has occurred. The program will now exit.");
        event->accept();
    }

private slots:
    void processInput() {
        lastInput = inputLine->text().toStdString();
        inputLine->clear();
    }

    void checkGameFinished() {
        if (g_gameFinished) {
            close();
        }
    }

private:
    QTextEdit *outputText;
    QLineEdit *inputLine;
    GameThread *gameThread;
    std::string lastInput;
};

WizardsCastleWindow* g_window = nullptr;

extern "C" {

bool parse_arguments(int argc, char* argv[])
{
    std::vector<std::string> args(argv + 1, argv + argc);
    return std::find(args.begin(), args.end(), "--debug") != args.end();
}

void initialize_qt(int argc, char *argv[]) {
    QApplication app(argc, argv);
    g_window = new WizardsCastleWindow();
    g_window->show();
    app.exec();
    delete g_window;
    g_window = nullptr;
}

const char* get_user_input_main() {
    static std::string input;
    static const char* dr_command = "DR";

    while (true) {
        print_message("ENTER YOUR COMMAND: ");
        
        g_window->clearInput();
        while (g_window->inputIsEmpty()) {
            QCoreApplication::processEvents();
        }
        
        input = g_window->getLastInput();
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        
        if (input.empty()) {
            print_message("\n");
            return input.c_str();
        }
        
        char firstChar = input[0];

        if (firstChar == 'D' && input.length() > 1 && input[1] == 'R') {
            print_message("DR\n");
            return dr_command;
        } else if (strchr("ADEFGHILMNOQSTUWYZ", firstChar) != NULL) {
            print_message("%c\n", firstChar);
            return input.c_str();
        } else {
            print_message("Invalid command. Type 'H' for help.\n");
        }
    }
}

int get_user_input_number() {
    int data;
    while (true) {
        g_window->clearInput();
        while (g_window->inputIsEmpty()) {
            QCoreApplication::processEvents();
        }
        
        std::string input = g_window->getLastInput();
        
        try {
           data = std::stoi(input);
           print_message("%d\n", data);
           return data;
        } catch (const std::invalid_argument&) {
            print_message_formatted("Invalid input. Please enter a valid integer.\n");
        } catch (const std::out_of_range&) {
            print_message_formatted("Input out of range. Please enter a smaller number.\n");
        }
    }
}

char get_user_input() {
    while (true) {
        print_message_formatted("ENTER YOUR COMMAND: ");
        
        g_window->clearInput();
        while (g_window->inputIsEmpty()) {
            QCoreApplication::processEvents();
        }
        
        std::string input = g_window->getLastInput();
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        
        if (input.empty()) {
            print_message_formatted("Please enter a command.\n");
            continue;
        }
        
        char command = input[0];
        print_message("%c\n", command);
        
        if (strchr("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ", command) != NULL) {
            return command;
        } else {
            print_message("Invalid command. Type 'H' for help.\n");
        }
    }
}

char get_user_input_custom_prompt(char* prompt) {
    while (true) {
        print_message("%s", prompt);
        
        g_window->clearInput();
        while (g_window->inputIsEmpty()) {
            QCoreApplication::processEvents();
        }
        
        std::string input = g_window->getLastInput();
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        
        if (input.empty()) {
            print_message_formatted("Please enter a command.\n");
            continue;
        }
        
        char command = input[0];
        print_message("%c\n", command);
        
        if (strchr("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ", command) != NULL) {
            return command;
        } else {
            print_message("Invalid command. Type 'H' for help.\n");
        }
    }
}

char get_user_input_yn() {
    while (true) {
        g_window->clearInput();
        while (g_window->inputIsEmpty()) {
            QCoreApplication::processEvents();
        }
        
        std::string input = g_window->getLastInput();
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        
        if (input.empty()) {
            print_message("Please Enter Y or N.\n");
            continue;
        }
        
        char command = input[0];
        
        if (strchr("YN", command) != NULL) {
            return command;
        } else {
            print_message("Invalid command. Please enter Y or N.\n");
        }
    }
}

void print_message(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (g_window) {
        g_window->appendToOutput(QString::fromUtf8(buffer));
    }
}

void print_message_formatted(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    
    capitalize_sentences(buffer);
    
    va_end(args);
    
    if (g_window) {
        g_window->appendToOutput(QString::fromUtf8(buffer));
    }
}

} // end of extern "C"

#include "wizardioqt5.moc"
