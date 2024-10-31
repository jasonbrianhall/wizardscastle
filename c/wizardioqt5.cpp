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
#include "asciiart.h"
#include <cstdlib>
#include <stdexcept>
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
        resize(1280, 768);  // Increased width to accommodate the map

        QWidget *centralWidget = new QWidget(this);
        QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

        QWidget *leftWidget = new QWidget(this);
        QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);

        createMenus();

        outputText = new QTextEdit(this);
        outputText->setReadOnly(true);
        
        updateFont();
        
        leftLayout->addWidget(outputText);

        inputLine = new QLineEdit(this);
        leftLayout->addWidget(inputLine);

        mainLayout->addWidget(leftWidget, 1);  // Left side takes 1/2 of the space

        // Create the map display
        mapDisplay = new QTextEdit(this);
        mapDisplay->setReadOnly(true);
        mapDisplay->setAcceptRichText(true);
        mapDisplay->setFont(QFont("Consolas", 10));
        /*mapDisplay->document()->setDefaultStyleSheet(
            "span.emoji { font-size: 48pt; line-height: 1.5; }"
            "span.text { font-size: 12pt; }"
        );*/        
        mainLayout->addWidget(mapDisplay, 1);  // Right side (map) takes 1/2 of the space

        setCentralWidget(centralWidget);

        connect(inputLine, &QLineEdit::returnPressed, this, &WizardsCastleWindow::processInput);
        mapUpdateTimer = new QTimer(this);
        connect(mapUpdateTimer, &QTimer::timeout, this, &WizardsCastleWindow::updateMap);
        mapUpdateTimer->start(66);  // Update 15 times per second
        outputText->installEventFilter(this);
        setColorScheme("White and Black");
    }

void display_map2(GameState *game, Player *player)
{
    int currentRoom = get_room_content(game, player->x, player->y, player->level);
    int treasurecount=0;
    int red, green, blue, monster_level;
    char color_str[12];

    print_message2("");        

    if (currentRoom >= ROOM_START && currentRoom <= ROOM_END)
    {
        switch(currentRoom) {
            case ENTRANCE:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px gold;'>" + AsciiArt::ENTRANCE_AA + "</p>");
                print_message2("\nYou are at the entrance.\n");
                break;
            case STAIRS_UP:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px gold;'>" + AsciiArt::STAIRSUP_AA + "</p>");
                print_message2("\nYou found stairs going up.\n");
                break;
            case STAIRS_DOWN:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px gold;'>" + AsciiArt::STAIRSDOWN_AA + "</p>");
                print_message2("\nYou found stairs going down.\n");
                break;
            case POOL:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 0 0 10px #00ffff;'>" + AsciiArt::POOL_AA + "</p>");
                print_message2("\nYou found a pool.\n");
                break;
            case CHEST:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px gold;'>" + AsciiArt::CHEST_AA + "</p>");
                print_message2("\nYou found a chest.\n");

                break;
            case CRYSTAL_ORB:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 0 0 10px #ff00ff;'>" + AsciiArt::CRYSTALORB_AA + "</p>");
                print_message2("\nYou've discovered a mystical crystal orb! It seems to swirl with magical energy.\n");
                break;
            case BOOK:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 0 0 10px #8b4513;'>" + AsciiArt::BOOK_AA + "</p>");
                print_message2("\nYou've discovered an old book. Open it to reveal it's mysteries.\n");
                break;
            case KOBOLD:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px #ff0000;'>" + AsciiArt::KOBOLD_AA + "</p>");
                print_message2("\nYou've encountered a Kobold!\n");
                break;
            case ORC:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px #ff0000;'>" + AsciiArt::ORC_AA + "</p>");
                print_message2("\nYou've encountered an Orc!\n");
                break;
            case WOLF:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px #808080;'>" + AsciiArt::WOLF_AA + "</p>");
                print_message2("\nYou've encountered a Wolf!\n");
                break;
            case GOBLIN:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px #00ff00;'>" + AsciiArt::GOBLIN_AA + "</p>");
                print_message2("\nYou've encountered a Goblin!\n");
                break;
            case OGRE:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px #8b4513;'>" + AsciiArt::OGRE_AA + "</p>");
                print_message2("\nYou've encountered an Ogre!\n");
                break;
            case TROLL:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px #4b0082;'>" + AsciiArt::TROLL_AA + "</p>");
                print_message2("\nYou've encountered a Troll!\n");
                break;
            case BEAR:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px #8b4513;'>" + AsciiArt::BEAR_AA + "</p>");
                print_message2("\nYou've encountered a Bear!\n");
                break;
            case MINOTAUR:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px #800000;'>" + AsciiArt::MINOTAUR_AA + "</p>");
                print_message2("\nYou've encountered a Minotaur!\n");
                break;
            case GARGOYLE:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px #696969;'>" + AsciiArt::GARGOYLE_AA + "</p>");
                print_message2("\nYou've encountered a Gargoyle!\n");
                break;
            case CHIMERA:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px #ff4500;'>" + AsciiArt::CHIMERA_AA + "</p>");
                print_message2("\nYou've encountered a Chimera!\n");
                break;
            case DRAGON:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px #ff4500;'>" + AsciiArt::DRAGON_AA + "</p>");
                print_message2("\nYou've encountered a Dragon!\n");
                break;
            case BALROG:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px #ff0000;'>" + AsciiArt::BALROG_AA + "</p>");
                print_message2("\nYou've encountered a Balrog!\n");
                break;
            case VENDOR:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px gold;'>" + AsciiArt::VENDOR_AA + "</p>");
                print_message2("\nYou've encountered a Vendor!\n");
                break;
            case MIMIC:
                mapDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI Emoji\", \"Apple Color Emoji\", \"Noto Color Emoji\"; text-shadow: 2px 2px 4px gold;'>" + AsciiArt::MIMIC_AA + "</p>");
                print_message2("\nYou've encountered a Mimic!\n");
                break;

            case EMPTY_ROOM:
                print_message2("\nYou are in an empty room.  It's voidness is chilling.\n");
                break;
       }
    }
    if(player->blindness_flag == 0)
    {
        print_message2("\n=== MAP OF LEVEL ");
        print_message2("%d", player->level);
        print_message2(" ===\n\n");

        // Print top border with column coordinates
        for(int x=1;x<=CASTLE_SIZE; x++)
        {
            if(x<10)
            {
                print_message2("       %d ", x);
            }
            else {
                print_message2("      %d ", x);
            }
        }
        if(CASTLE_SIZE<10)
        {
            print_message2("\n +");
        }
        else
        {
            print_message2("\n  +");
        }
        for(int x=1;x<=CASTLE_SIZE; x++)
        {
            print_message2("--------+");
        }
        print_message2("\n");


        for (int x = 1; x <= CASTLE_SIZE; x++) {
            // Print row coordinate
            if(x<10 && CASTLE_SIZE>=10)
            {
                 print_message2(" %d", x);
            }
            else
            {
                 print_message2("%d", x);
           
            }
            for (int y = 1; y <= CASTLE_SIZE; y++) {
                print_message2("|");
                if (x == player->x && y == player->y) {
                    print_message2("<p style='color: #00FF00;'>  [YOU] </p>");
                } else if (is_room_discovered(game, x, y, player->level)) {
                    int room_content = get_room_content(game, x, y, player->level);
                    char room_str[10] = "        \0";
                    get_room_description(room_content, room_str);
                    if (room_content>=MONSTER_START && room_content<=MONSTER_END)
                    {
                        monster_level = room_content - MONSTER_START;
    
                        // RGB values
                        red = 255;  // Keep full red
                        // Reduce green and blue as monsters get tougher
                        green = 150 - (monster_level * 12);  // Starts at 150, decreases to ~18
                        blue = 150 - (monster_level * 12);   // Starts at 150, decreases to ~18
    
                        // Ensure we don't go below 0
                        if (green < 0) green = 0;
                        if (blue < 0) blue = 0;
    
                        // Format the color string as RGB
                        sprintf(color_str, "#%02X%02X%02X", red, green, blue);
                        print_message2("<p style='color: %s;'>%s</p>", color_str, room_str);
                    }
                    else if (room_content >= TREASURE_START && room_content <= TREASURE_END)
                    {
                        // Gold color for treasures
                        print_message2("<p style='color: #FFD700;'>%s</p>", room_str);
                    }
                    else if (room_content == VENDOR) {
                        // Blue for vendors
                        print_message2("<p style='color: #4169E1;'>%s</p>", room_str);
                    }
                    else if (room_content == STAIRS_UP || room_content == STAIRS_DOWN) {
                        // Orange for stairs
                        print_message2("<p style='color: #FFA500;'>%s</p>", room_str);
                    }
                    else if (room_content == POOL) {
                        // Cyan for pools
                        print_message2("<p style='color: #00FFFF;'>%s</p>", room_str);
                    }
                    else if (room_content == CHEST) {
                        // Brown for chests
                        print_message2("<p style='color: #8B4513;'>%s</p>", room_str);
                    }
                    else if (room_content == CRYSTAL_ORB) {
                        // Purple for crystal orbs
                        print_message2("<p style='color: #800080;'>%s</p>", room_str);
                    }
                    else if (room_content == BOOK) {
                        // Tan for books
                        print_message2("<p style='color: #D2B48C;'>%s</p>", room_str);
                    }
                    else if (room_content == WARP) {
                        // Magenta for warps
                        print_message2("<p style='color: #FF00FF;'>%s</p>", room_str);
                    }
                    else if (room_content == FLARES) {
                        // Orange-red for flares
                        print_message2("<p style='color: #FF4500;'>%s</p>", room_str);
                    }
                    else if (room_content == GOLD) {
                        // Yellow for gold
                        print_message2("<p style='color: #FFD700;'>%s</p>", room_str);
                    }
                    else if (room_content == SINKHOLE) {
                        print_message2("<span style='color: #800080;'>%s</span>", room_str);  // Dark Purple
                    }
                    else if (room_content == ENTRANCE) {
                        print_message2("<span style='color: #FFD700; font-weight: bold;'>%s</span>", room_str);  // Bold Gold
                    }
                    else
                    {
                         print_message2(room_str);
                    }
                } else {
                    print_message2("????????");
                }
            }
            print_message2("|\n");

            // Print horizontal border between rows
            if (x < CASTLE_SIZE) {
                if (CASTLE_SIZE>=10)
                {
                    print_message2(" ");
                } 
                print_message2(" +");
                for(int x=1;x<=CASTLE_SIZE; x++)
                {
                    print_message2("--------+");
                }
                print_message2("\n");
            }
        }

        // Print bottom border
        if(CASTLE_SIZE>=10)
        {
            print_message2(" ");
        }
        print_message2(" +");
        for(int x=1;x<=CASTLE_SIZE; x++)
        {
            print_message2("--------+");
        }
        print_message2("\n");
         // Map is too large for the default font for MS-DOS (Same Information is available in help)
         print_message2("\nLEGEND:\n");
         print_message2("[YOU]    = Your location   EMPTY    = Empty room     ENTRANCE = Entrance\n");
         print_message2("POOL     = Magic Pool      CHEST    = Treasure Chest\n");
         print_message2("GOLD     = Gold Pieces     FLARES   = Flares\n");
         print_message2("WARP     = Warp/Orb        SINKHOLE = Sinkhole\n");
         print_message2("CRYSTAL  = Crystal Orb     BOOK     = Magic Book\n");
         print_message2("MONSTER  = Monster Name    VENDOR   = Vendor\n");
         print_message2("TREASURE = Treasure Name   ???????? = Undiscovered\n");
         print_message2("STAIRS UP= Stairs U        STAIRS D = Stairs Down\n");

         print_message2("\n=== PLAYER STATUS ===\n");

    }
    else
    {
         print_message2_formatted("\nBlind %s can't see maps.\n\n", get_race_name(player->race));
    }
    
    
    // Print player race and attributes
    print_message2("Race: ");
    print_message2_formatted(get_race_name(player->race));
    print_message2("   Sex: "); 
    if (player->sex==FEMALE)
    {
        print_message2("Female");
    }
    else
    {
        print_message2("Male");
    }
    print_message2("\n");
    print_message2("Strength: %d  Intelligence: %d  Dexterity: %d\n", player->strength, player->intelligence, player->dexterity);

    // Print player position
    print_message2("Location: Level %d, Room (%d, %d)\n", player->level, player->x, player->y);

    // Print player inventory
    print_message2("Gold Pieces: %d  Flares: %d\n", player->gold, player->flares);

    // Print armor and weapon
    const char* armor_types[] = {"No Armor", "Leather", "Chainmail", "Plate", "Stone"};
    const char* weapon_types[] = {"No Weapon", "Dagger", "Mace", "Sword", "Excalibur"};
    print_message2("Armor: %s  Weapon: %s\n", armor_types[player->armor_type], weapon_types[player->weapon_type]);
    // Print special items

    print_message2("Special Items: ");
    if (player->lamp_flag) print_message2("Lamp ");
    if (player->runestaff_flag) print_message2("Runestaff ");
    if (player->orb_flag) print_message2("Orb of Zot ");
    print_message2("\n");

    // Print curses or blessings
    print_message2("Status Effects: ");
    if (player->blindness_flag) print_message2("Blind ");
    if (player->stickybook_flag) print_message2("Sticky Book ");
    print_message2("\n");

    print_message2("Armor points: %d\n", player->armor_points);

    // Print number of treasures

    for (int i=0; i<TREASURE_END-TREASURE_START+1; i++)
    {
        if(game->treasure[i]>=1)
        {
            treasurecount+=game->treasure[i];
        }
    }


    print_message2("Treasures Found: %d\n", treasurecount);

    for (int i=0; i<TREASURE_END-TREASURE_START+1; i++)
    {
        if(game->treasure[i])
        {
        	print_message2("     ");
        	print_message2(get_treasure_name(i));
                if(game->treasure[i]>1)
                {
                     print_message2(" * %d", game->treasure[i]);
                }
        	print_message2("\n");
        }
    }
    print_message2("Turn count: %d\n", game->turn_count);

    print_message2("======================\n\n");
    //mapDisplay->append("");

}

void print_message2(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    QString message = QString::fromUtf8(buffer);
        message = "<span style='font-family: Consolas, \"Courier New\", Monaco, monospace; "
              "white-space: pre; "
              "line-height: 1em; "
              "display: inline-block;'>" 
              + message + 
              "</span>";
    appendToMap(message);
}

void print_message2_formatted(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    
    capitalize_sentences(buffer);  // Use the function from utilities.c
    
    va_end(args);
    
    QString message = QString::fromUtf8(buffer);
    message = "<span style='font-family: Consolas, \"Courier New\", Monaco, monospace; "
              "white-space: pre; "
              "line-height: 1em; "
              "display: inline-block;'>" 
              + message + 
              "</span>";    
    appendToMap(message);
}


    void appendToMap(const QString& text) {
    mapDisplay->moveCursor(QTextCursor::End);
    mapDisplay->insertHtml(text);
    mapDisplay->verticalScrollBar()->setValue(outputText->verticalScrollBar()->maximum());
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

    void updateMap() {
        if (g_player && g_game) {
            mapDisplay->clear();  // Clear the previous map
            display_map2(g_game, g_player);
        }
    }
    
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
        inputLine->setPalette(palette);
        outputText->setPalette(palette);
        inputLine->setPalette(palette);
        mapDisplay->setPalette(palette);

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
    QTextEdit *mapDisplay; 
    QTimer *mapUpdateTimer;
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
            player.race = ELF;  // Elf
            player.sex = MALE;   // Male
            player.strength = MAX_STRENGTH;
            player.intelligence = MAX_INTELLIGENCE;
            player.dexterity = MAX_DEXTERITY;
            player.gold = 10000;
            player.flares = 1000;
            player.lamp_flag = 1;
            player.runestaff_flag = 1;
            player.weapon_type = EXCALIBUR;  // Sword
            player.armor_type = STONE;   // Plate
            player.armor_points = MAX_ARMOR_POINTS;

            print_message("DEBUG MODE: You are a male elf with %d Strength, %d Intelligence, and %d Dexterity.\n", player.strength, player.intelligence, player.dexterity);
            print_message("DEBUG MODE: You have Excalibur and Stone armor.\n");
            print_message("DEBUG MODE: You start with the Runestaff, 10000 gold, 1000 flares, and a lamp.\n");
            print_message("DEBUG MODE: All rooms are discovered\n");
            for (q=0;q<TREASURE_END-TREASURE_START+1; q++)
            {
                print_message("%d", q);
                game.treasure[q]=1;
            }
            
        }

        if (debug_mode) {
            print_message("DEBUG MODE: The Orb of Zot is located at (%d, %d) on level %d.\n", 
                   game.orb_location[1], game.orb_location[0], game.orb_location[2]);
            for (q = 0; q < MAP_SIZE; q++) {
                game.discovered_rooms[q] = 1;  // 0 means undiscovered
            }
        }
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
        print_message("Enter a number: ");

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
        print_message("%c\n", input[0]);
        
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
        print_message("%c\n", input[0]);        
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
