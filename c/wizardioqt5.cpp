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
#include <QActionGroup>
#include <QStyle>
#include "wizardio.h"
#include <cstdlib>
#include <QFileDialog>

extern "C" {
#include "wizards-castle.h"
#include "player.h"
#include "gamestate.h"
#include "utilities.h"
#include "combat.h"
#include "vendor.h"
#include "wizardio.h"
#include "save_load.h"
}

    Player* g_player = nullptr;
    GameState* g_game = nullptr;


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
        setColorScheme("White and Black");
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

    void clearOutput() {
        outputText->clear();
    }

    void setWaitingForSpecificInput(bool waiting, const std::string& validInputs) {
        waitingForSpecificInput = waiting;
        this->validInputs = validInputs;
    }

    bool isWaitingForSpecificInput() const {
        return waitingForSpecificInput;
    }

    void setGamePointers(Player* player, GameState* game) {
        g_player = player;
        g_game = game;
    }

signals:
    void programExit();
    void gameStateChanged();
    void newGameRequested();

protected:
    void closeEvent(QCloseEvent *event) override {
        event->accept();
        std::exit(0);
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

    void saveGame() {
        if (!g_player || !g_game) {
            QMessageBox::warning(this, tr("Save Failed"), tr("No active game to save."));
            return;
        }

        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Game"), "", tr("Wizard's Castle Save (*.wcs)"));
        if (fileName.isEmpty()) {
            return;
        }

        // Ensure the file has a .wcs extension
        if (!fileName.endsWith(".wcs", Qt::CaseInsensitive)) {
            fileName += ".wcs";
        }

        if (save_game(fileName.toStdString().c_str(), g_player, g_game)) {
            QMessageBox::information(this, tr("Game Saved"), tr("Your game has been saved successfully."));
        } else {
            QMessageBox::warning(this, tr("Save Failed"), tr("Failed to save the game. Please try again."));
        }
    }

    void loadGame() {
        if (!g_player || !g_game) {
            QMessageBox::warning(this, tr("Load Failed"), tr("Cannot load game at this time."));
            return;
        }

        QString fileName = QFileDialog::getOpenFileName(this, tr("Load Game"), "", tr("Wizard's Castle Save (*.wcs);;All Files (*)"));
        if (fileName.isEmpty()) {
            return;
        }

        if (load_game(fileName.toStdString().c_str(), g_player, g_game)) {
            QMessageBox::information(this, tr("Game Loaded"), tr("Your game has been loaded successfully."));
            // You might want to trigger an update of the UI here
            emit gameStateChanged();
        } else {
            QMessageBox::warning(this, tr("Load Failed"), tr("Failed to load the game. The file might be corrupted or incompatible."));
        }
        
        
        int playAgain = 1;
        while (playAgain) {
            playAgain = main_game_loop(g_player, g_game);
            if (playAgain) {
                // If the player wants to play again after finishing, start a new game
                emit newGameRequested();
                break;
            }
        }
        //display_map(g_game, g_player);
        /*for(int x=0;x<MAP_SIZE;x++)
        {
             print_message("%i %i\n", x, g_game->discovered_rooms[x]);
        }*/
    }
    
    void newGame() {
        // Ask for confirmation before starting a new game
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "New Game", "Are you sure you want to start a new game? Any unsaved progress will be lost.",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            emit newGameRequested();
        }
    }

    void setColorScheme(const QString &scheme) {
        QPalette palette;
    if (scheme == "Commodore 64") {
        palette.setColor(QPalette::Base, Qt::black);
        palette.setColor(QPalette::Text, QColor(0, 255, 0)); // Green text
    } else if (scheme == "Blue and White") {
        palette.setColor(QPalette::Base, QColor(0, 0, 255)); // Blue background
        palette.setColor(QPalette::Text, Qt::white);
    } else if (scheme == "Black and White") {
        palette.setColor(QPalette::Base, Qt::black);
        palette.setColor(QPalette::Text, Qt::white);
    } else if (scheme == "White and Black") {
        palette.setColor(QPalette::Base, Qt::white);
        palette.setColor(QPalette::Text, Qt::black);
    } else if (scheme == "Solarized Light") {
        palette.setColor(QPalette::Base, QColor(253, 246, 227)); // Light beige
        palette.setColor(QPalette::Text, QColor(101, 123, 131)); // Dark gray-blue
    } else if (scheme == "Solarized Dark") {
        palette.setColor(QPalette::Base, QColor(0, 43, 54));     // Dark blue-gray
        palette.setColor(QPalette::Text, QColor(131, 148, 150)); // Light gray-blue
    } else if (scheme == "Monokai") {
        palette.setColor(QPalette::Base, QColor(39, 40, 34));    // Dark gray
        palette.setColor(QPalette::Text, QColor(248, 248, 242)); // Off-white
    } else if (scheme == "Gruvbox Light") {
        palette.setColor(QPalette::Base, QColor(251, 241, 199)); // Light tan
        palette.setColor(QPalette::Text, QColor(60, 56, 54));    // Dark gray
    } else if (scheme == "Gruvbox Dark") {
        palette.setColor(QPalette::Base, QColor(40, 40, 40));    // Dark gray
        palette.setColor(QPalette::Text, QColor(235, 219, 178)); // Light tan
    } else if (scheme == "Nord") {
        palette.setColor(QPalette::Base, QColor(46, 52, 64));    // Dark blue-gray
        palette.setColor(QPalette::Text, QColor(216, 222, 233)); // Light gray-blue
    } else { // Default
        palette = QApplication::style()->standardPalette();
    }
        outputText->setPalette(palette);
        inputLine->setPalette(palette);
    }
    
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

        QAction *newGameAction = new QAction(tr("&New Game"), this);
        newGameAction->setShortcut(QKeySequence::New);
        connect(newGameAction, &QAction::triggered, this, &WizardsCastleWindow::newGame);
        fileMenu->addAction(newGameAction);


        QAction *saveAction = new QAction(tr("&Save Game"), this);
        saveAction->setShortcut(QKeySequence::Save);
        connect(saveAction, &QAction::triggered, this, &WizardsCastleWindow::saveGame);
        fileMenu->addAction(saveAction);


        QAction *loadAction = new QAction(tr("&Load Game"), this);
        loadAction->setShortcut(QKeySequence::Open);
        connect(loadAction, &QAction::triggered, this, &WizardsCastleWindow::loadGame);
        fileMenu->addAction(loadAction);

        fileMenu->addSeparator();
        QAction *quitAction = new QAction(tr("&Quit"), this);
        quitAction->setShortcuts(QKeySequence::Quit);
        connect(quitAction, &QAction::triggered, this, &WizardsCastleWindow::quit);
        fileMenu->addAction(quitAction);

        QMenu *optionsMenu = menuBar->addMenu(tr("&Options"));
        QMenu *colorSchemeMenu = optionsMenu->addMenu(tr("&Color Scheme"));

        QActionGroup *colorSchemeGroup = new QActionGroup(this);
        colorSchemeGroup->setExclusive(true);

        QStringList schemes = {"Default", "Commodore 64", "Blue and White", "Black and White", "White and Black",
                       "Solarized Light", "Solarized Dark", "Monokai", "Gruvbox Light", "Gruvbox Dark", "Nord"};

        for (const QString &scheme : schemes) {
            QAction *schemeAction = new QAction(scheme, this);
            schemeAction->setCheckable(true);
            colorSchemeMenu->addAction(schemeAction);
            colorSchemeGroup->addAction(schemeAction);
            connect(schemeAction, &QAction::triggered, [this, scheme]() {
                setColorScheme(scheme);
            });
            if (scheme == "Default") {
                schemeAction->setChecked(true);
            }
        }
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


bool parse_arguments(int argc, char* argv[])
{
    std::vector<std::string> args(argv + 1, argv + argc);
    return std::find(args.begin(), args.end(), "--debug") != args.end();
}

void initialize_qt(int argc, char *argv[]) {
    QApplication* app = new QApplication(argc, argv);
    g_window = new WizardsCastleWindow();
    g_window->show();

    Player player;
    GameState game;

    g_window->setGamePointers(&player, &game);

    bool debug_mode = parse_arguments(argc, argv);
    int q, playagain=1;

        auto startNewGame = [&]() {
        g_window->clearOutput();
        initialize_player(&player);
        generate_castle(&game);
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
            player.armor_points = 21;

            print_message("DEBUG MODE: You are a male elf with 18 Strength, 18 Intelligence, and 18 Dexterity.\n");
            print_message("DEBUG MODE: You have a Sword and Plate armor.\n");
            print_message("DEBUG MODE: You start with the Runestaff, 10000 gold, 1000 flares, and a lamp.\n");
            print_message("DEBUG MODE: All rooms are discovered\n");
        }

        if (debug_mode) {
            print_message("DEBUG MODE: The Orb of Zot is located at (%d, %d) on level %d.\n", 
                   game.orb_location[1], game.orb_location[0], game.orb_location[2]);
            for (q = 0; q < MAP_SIZE; q++) {
                game.discovered_rooms[q] = 1;  // 0 means undiscovered
            }
        }

        playagain = main_game_loop(&player, &game);
    };
   QObject::connect(g_window, &WizardsCastleWindow::newGameRequested, startNewGame);

    // Start the initial game
    startNewGame();

    while (playagain) {
        playagain = main_game_loop(&player, &game);
        if (playagain) {
            startNewGame();
        }
    }
    //app->exec();
    delete g_window;
    delete app;

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
            print_message("\n");
            return input.c_str();  // Return empty string
        }
        
        // Get the first character
        char firstChar = input[0];

        // Validate commands
        if (firstChar == 'D' && input.length() > 1 && input[1] == 'R') {
            print_message("DR\n", input[0]);
            return dr_command;  // Return "DR" for DRINK
        } else if (strchr("ADEFGHILMNOQSTUWYZ", firstChar) != NULL) {
            print_message("%c\n", firstChar);

            return input.c_str();  // Return the single letter command

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
           print_message("%d\n", data);  // Assuming print_message is similar to printf
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
        
        // Convert input to uppercase
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        
        // Check if input is empty
        if (input.empty()) {
            print_message_formatted("Please enter a command.\n");
            continue;
        }
        
        // Get the first character of the input
        char command = input[0];
        print_message("%c\n", command);
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
        print_message("%c\n", input[0], input);
        
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
