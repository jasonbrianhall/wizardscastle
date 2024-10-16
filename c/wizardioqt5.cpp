#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTimer>
#include <QMessageBox>
#include <QScrollBar>
#include <QString>
#include <QTextStream>
#include <QRegularExpression>
#include <QCloseEvent>
#include <QMenuBar>
#include <QWheelEvent>
#include "wizardio.h"
#include <cstdlib>

extern "C" {
#include "wizards-castle.h"
#include "player.h"
#include "gamestate.h"
#include "utilities.h"
#include "combat.h"
#include "vendor.h"
#include "wizardio.h"
}

class WizardsCastleWindow : public QMainWindow {
    Q_OBJECT

public:
    WizardsCastleWindow(QWidget *parent = nullptr)
        : QMainWindow(parent), waitingForSpecificInput(false), fontSize(10)
    {
        setWindowTitle("Wizard's Castle");
        resize(800, 600);

        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        createMenus();

        outputText = new QTextEdit(this);
        outputText->setReadOnly(true);
        
        updateFont();
        
        layout->addWidget(outputText);

        inputLine = new QLineEdit(this);
        layout->addWidget(inputLine);

        setCentralWidget(centralWidget);

        connect(inputLine, &QLineEdit::returnPressed, this, &WizardsCastleWindow::processInput);

        outputText->installEventFilter(this);
    }

    void appendToOutput(const QString& text) {
        outputText->moveCursor(QTextCursor::End);
        outputText->insertPlainText(text);
        outputText->verticalScrollBar()->setValue(outputText->verticalScrollBar()->maximum());
    }

    bool inputIsEmpty() const {
        return lastInput.empty();
    }

    std::string getLastInput() {
        std::string input = lastInput;
        lastInput.clear();
        return input;
    }

    void clearInput() {
        inputLine->clear();
        lastInput.clear();
    }

    void setWaitingForSpecificInput(bool waiting, const std::string& validInputs) {
        waitingForSpecificInput = waiting;
        this->validInputs = validInputs;
    }

    bool isWaitingForSpecificInput() const {
        return waitingForSpecificInput;
    }

protected:
    void closeEvent(QCloseEvent *event) override {
        quit();
        event->accept();
    }

    bool eventFilter(QObject *obj, QEvent *event) override {
        if (obj == outputText && event->type() == QEvent::Wheel) {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
            if (wheelEvent->modifiers() & Qt::ControlModifier) {
                const int delta = wheelEvent->angleDelta().y();
                if (delta > 0) {
                    increaseFontSize();
                } else if (delta < 0) {
                    decreaseFontSize();
                }
                return true;
            }
        }
        return QMainWindow::eventFilter(obj, event);
    }

private slots:
    void quit() {
        close();
    }
    void processInput() {
        QString input = inputLine->text().toUpper();
        inputLine->clear();

        if (waitingForSpecificInput) {
            if (validInputs.find(input[0].toLatin1()) != std::string::npos) {
                lastInput = input[0].toLatin1();
                waitingForSpecificInput = false;
            } else {
                appendToOutput(QString("Please enter one of the following: %1\n").arg(QString::fromStdString(validInputs)));
            }
        } else {
            lastInput = input.toStdString();
        }
    }

    void increaseFontSize() {
        if (fontSize < 24) {
            fontSize++;
            updateFont();
        }
    }

    void decreaseFontSize() {
        if (fontSize > 6) {
            fontSize--;
            updateFont();
        }
    }

private:
    QTextEdit *outputText;
    QLineEdit *inputLine;
    std::string lastInput;
    bool waitingForSpecificInput;
    std::string validInputs;
    int fontSize;

    void createMenus() {
        QMenuBar *menuBar = new QMenuBar(this);
        setMenuBar(menuBar);

        QMenu *fileMenu = menuBar->addMenu(tr("&File"));
        QAction *quitAction = new QAction(tr("&Quit"), this);
        quitAction->setShortcuts(QKeySequence::Quit);
        connect(quitAction, &QAction::triggered, this, &WizardsCastleWindow::quit);
        fileMenu->addAction(quitAction);
    }

    void updateFont() {
        QFont monoFont("Consolas", fontSize);
        if (!monoFont.exactMatch()) {
            monoFont.setStyleHint(QFont::Monospace);
        }
        outputText->setFont(monoFont);
    }

    void cleanup() {
        appendToOutput("Cleaning up and closing Wizard's Castle. Goodbye!\n");
    }
};

WizardsCastleWindow* g_window = nullptr;

extern "C" {

void initialize_qt(int argc, char *argv[]) {
    static QApplication app(argc, argv);
    g_window = new WizardsCastleWindow();
    g_window->show();
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
            // Debug mode: Set player as male elf with max attributes and equipment
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
            for (q = 0; q < MAP_SIZE; q++) {
                game.discovered_rooms[q] = 1;  // 0 means undiscovered
            }

        }

        playagain = main_game_loop(&player, &game);
    }
}

void close_qt(WizardsCastleWindow* window) {
    printf("Quit");
    if (window) {
        window->close();
        delete window;
    }
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
        
        // Remove newline character if present
        input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
        
        // Convert input to uppercase
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        
        // Check if input is empty
        if (input.empty()) {
            return input.c_str();  // Return empty string
        }
        
        // Get the first character
        char firstChar = input[0];
        
        // Validate commands
        if (firstChar == 'D' && input.length() > 1 && input[1] == 'R') {
            return dr_command;  // Return "DR" for DRINK
        } else if (strchr("ADEFGHILMNOQSTUWYZ", firstChar) != NULL) {
            return input.c_str();  // Return the single letter command
        } else {
            print_message("Invalid command. Type 'H' for help.\n");
        }
    }
}

int get_user_input_number() {
    while (true) {
        print_message_formatted("Enter a number: ");
        
        g_window->clearInput();
        while (g_window->inputIsEmpty()) {
            QCoreApplication::processEvents();
        }
        
        std::string input = g_window->getLastInput();
        
        try {
            return std::stoi(input);
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
        
        // Convert input to uppercase
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        
        // Check if input is empty
        if (input.empty()) {
            print_message_formatted("Please enter a command.\n");
            continue;
        }
        
        // Get the first character of the input
        char command = input[0];
        
        // Check if it's a valid command
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
        
        // Convert input to uppercase
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        
        // Check if input is empty
        if (input.empty()) {
            print_message_formatted("Please enter a command.\n");
            continue;
        }
        
        // Get the first character of the input
        char command = input[0];
        
        // Check if it's a valid command
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
        
        // Convert input to uppercase
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        
        // Check if input is empty
        if (input.empty()) {
            print_message("Please Enter Y or N.\n");
            continue;
        }
        
        // Get the first character of the input
        char command = input[0];
        
        // Check if it's a valid command
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

    QString message = QString::fromUtf8(buffer);
    g_window->appendToOutput(message);
}

void print_message_formatted(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    
    capitalize_sentences(buffer);  // Use the function from utilities.c
    
    va_end(args);
    
    QString message = QString::fromUtf8(buffer);
    g_window->appendToOutput(message);
}

} // end of extern "C"

#include "wizardioqt5.moc"
