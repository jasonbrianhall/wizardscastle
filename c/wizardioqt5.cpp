#include "asciiart.h"
#include "wizardio.h"
#include <QActionGroup>
#include <QApplication>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QScrollBar>
#include <QSpinBox>
#include <QString>
#include <QStyle>
#include <QTextEdit>
#include <QTextStream>
#include <QTimer>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QScreen>
#include <cstdlib>
#include <stdexcept>

extern "C" {
#include "combat.h"
#include "gamestate.h"
#include "player.h"
#include "save_load.h"
#include "utilities.h"
#include "vendor.h"
#include "wizardio.h"
#include "wizards-castle.h"
}

Player *g_player = nullptr;
GameState *g_game = nullptr;

class WizardsCastleWindow : public QMainWindow {
  Q_OBJECT

public:
  WizardsCastleWindow(QWidget *parent = nullptr)
      : QMainWindow(parent), waitingForSpecificInput(false), fontSize(10) {
    setWindowTitle("Wizard's Castle");

   // Try to load and set the application icon
    QFileInfo iconFile("icon.ico");
    if (iconFile.exists()) {
      QIcon icon("icon.ico");
      setWindowIcon(icon);
      QApplication::setWindowIcon(icon);  // Set icon for all windows
    } 
    resize(1280, 768);

    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    
    // Set the window size to match the screen
    resize(screenGeometry.width(), screenGeometry.height());


    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    
    // Left panel for game output and input
    QWidget *leftWidget = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);

    // Right panel containing status, stats, and map
    QWidget *rightWidget = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    
    // Top section of right panel for status and stats
    QWidget *topRightWidget = new QWidget(this);
    QHBoxLayout *topRightLayout = new QHBoxLayout(topRightWidget);

    createMenus();

    // Setup left panel (game output and input)
    outputText = new QTextEdit(this);
    outputText->setReadOnly(true);
    updateFont();
    leftLayout->addWidget(outputText);

    inputLine = new QLineEdit(this);
    leftLayout->addWidget(inputLine);

    // Setup top-right section (status and stats displays)
    statusDisplay = new QTextEdit(this);
    statusDisplay->setReadOnly(true);
    statusDisplay->setAcceptRichText(true);
    statusDisplay->setFont(QFont("Consolas", 10));
    topRightLayout->addWidget(statusDisplay, 1);

    statsDisplay = new QTextEdit(this);
    statsDisplay->setReadOnly(true);
    statsDisplay->setAcceptRichText(true);
    statsDisplay->setFont(QFont("Consolas", 10));
    topRightLayout->addWidget(statsDisplay, 1);

    // Add top section to right layout
    rightLayout->addWidget(topRightWidget, 1);

    // Setup map display (takes remaining space in right panel)
    mapDisplay = new QTextEdit(this);
    mapDisplay->setReadOnly(true);
    mapDisplay->setAcceptRichText(true);
    mapDisplay->setFont(QFont("Consolas", 10));
    rightLayout->addWidget(mapDisplay, 2);

    // Add main sections to the layout
    mainLayout->addWidget(leftWidget, 1);  // Left side takes 1/2 of space
    mainLayout->addWidget(rightWidget, 1); // Right side takes 1/2 of space

    setCentralWidget(centralWidget);

    // Connect signals and setup timers
    connect(inputLine, &QLineEdit::returnPressed, this,
            &WizardsCastleWindow::processInput);
    mapUpdateTimer = new QTimer(this);
    connect(mapUpdateTimer, &QTimer::timeout, this,
            &WizardsCastleWindow::updateMap);
    mapUpdateTimer->start(200);
    
    outputText->installEventFilter(this);
    inputLine->installEventFilter(this);
    commandHistory.clear();
    
    setColorScheme("Default");
  }  
  void display_map2(GameState *game, Player *player) {
    int currentRoom =
        get_room_content(game, player->x, player->y, player->level);
    int treasurecount = 0;
    int red, green, blue, monster_level;
    char color_str[12];

    char lowercase_species[100]; // Adjust size based on max monster name length
    to_lowercase(lowercase_species, get_race_name(player->race));

    if (currentRoom >= ROOM_START && currentRoom <= ROOM_END) {
      switch (currentRoom) {
      case ENTRANCE:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px gold;'>" +
                           AsciiArt::ENTRANCE_AA + "</p>");
        statusDisplay->append("You are at the entrance.");
        break;
      case STAIRS_UP:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px gold;'>" +
                           AsciiArt::STAIRSUP_AA + "</p>");
        statusDisplay->append("You found stairs going up.");
        break;
      case STAIRS_DOWN:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px gold;'>" +
                           AsciiArt::STAIRSDOWN_AA + "</p>");
        statusDisplay->append("You found stairs going down.");
        break;
      case POOL:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 0 0 10px #00ffff;'>" +
                           AsciiArt::POOL_AA + "</p>");
        statusDisplay->append("You found a pool.");
        break;
      case CHEST:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px gold;'>" +
                           AsciiArt::CHEST_AA + "</p>");
        statusDisplay->append("You found a chest.");

        break;
      case CRYSTAL_ORB:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 0 0 10px #ff00ff;'>" +
                           AsciiArt::CRYSTALORB_AA + "</p>");
        statusDisplay->append("You've discovered a mystical crystal orb! It seems "
                       "to swirl with magical energy.");
        break;
      case BOOK:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 0 0 10px #8b4513;'>" +
                           AsciiArt::BOOK_AA + "</p>");
        statusDisplay->append("You've discovered an old book. Open it to reveal "
                       "it's mysteries.");
        break;
      case KOBOLD:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px #ff0000;'>" +
                           AsciiArt::KOBOLD_AA + "</p>");
        statusDisplay->append("You've encountered a Kobold!");
        break;
      case ORC:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px #ff0000;'>" +
                           AsciiArt::ORC_AA + "</p>");
        statusDisplay->append("You've encountered an Orc!");
        break;
      case WOLF:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px #808080;'>" +
                           AsciiArt::WOLF_AA + "</p>");
        statusDisplay->append("You've encountered a Wolf!");
        break;
      case GOBLIN:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px #00ff00;'>" +
                           AsciiArt::GOBLIN_AA + "</p>");
        statusDisplay->append("You've encountered a Goblin!");
        break;
      case OGRE:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px #8b4513;'>" +
                           AsciiArt::OGRE_AA + "</p>");
        statusDisplay->append("You've encountered an Ogre!");
        break;
      case TROLL:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px #4b0082;'>" +
                           AsciiArt::TROLL_AA + "</p>");
        statusDisplay->append("You've encountered a Troll!");
        break;
      case BEAR:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px #8b4513;'>" +
                           AsciiArt::BEAR_AA + "</p>");
        statusDisplay->append("You've encountered a Bear!");
        break;
      case MINOTAUR:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px #800000;'>" +
                           AsciiArt::MINOTAUR_AA + "</p>");
        statusDisplay->append("You've encountered a Minotaur!");
        break;
      case GARGOYLE:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px #696969;'>" +
                           AsciiArt::GARGOYLE_AA + "</p>");
        statusDisplay->append("You've encountered a Gargoyle!");
        break;
      case CHIMERA:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px #ff4500;'>" +
                           AsciiArt::CHIMERA_AA + "</p>");
        statusDisplay->append("You've encountered a Chimera!");
        break;
      case DRAGON:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px #ff4500;'>" +
                           AsciiArt::DRAGON_AA + "</p>");
        statusDisplay->append("You've encountered a Dragon!");
        break;
      case BALROG:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px #ff0000;'>" +
                           AsciiArt::BALROG_AA + "</p>");
        statusDisplay->append("You've encountered a Balrog!");
        break;
      case VENDOR:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px gold;'>" +
                           AsciiArt::VENDOR_AA + "</p>");
        statusDisplay->append("You've encountered a Vendor!");
        break;
      case MIMIC:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px gold;'>" +
                           AsciiArt::MIMIC_AA + "</p>");
        statusDisplay->append("You've encountered a Mimic!");
        break;

      case EMPTY_ROOM:
        statusDisplay->append("<p style='font-size: 48pt; font-family: \"Segoe UI "
                           "Emoji\", \"Apple Color Emoji\", \"Noto Color "
                           "Emoji\"; text-shadow: 2px 2px 4px gold;'>" +
                           AsciiArt::EMPTY_AA + "</p>");

        statusDisplay->append(
            "You are in an empty room.  It's voidness is chilling.");
        break;
      }
      print_message2("Current location:  Level %d, Room (%d %d)\n\n",
                     player->level, player->y, player->x);
    }
    if (player->blindness_flag == 0) {

      if (player->x != lastPlayerX || player->y != lastPlayerY ||
          player->level != lastPlayerLevel) {
        displayLevel = -1; // Reset to player's current level
        // Update last known position
        lastPlayerX = player->x;
        lastPlayerY = player->y;
        lastPlayerLevel = player->level;
      }

      // Use either the display level or player's current level
      int levelToDisplay = (displayLevel == -1) ? player->level : displayLevel;

      // When displaying room content:
      print_message2("=== MAP OF LEVEL ");
      print_message2("%d", levelToDisplay);
      print_message2(" ===\n\n");

      // Print top border with column coordinates
      for (int x = 1; x <= CASTLE_SIZE; x++) {
        if (x < 10) {
          print_message2("       %d ", x);
        } else {
          print_message2("      %d ", x);
        }
      }
      if (CASTLE_SIZE < 10) {
        print_message2("\n +");
      } else {
        print_message2("\n  +");
      }
      for (int x = 1; x <= CASTLE_SIZE; x++) {
        print_message2("--------+");
      }
      print_message2("\n");

      for (int x = 1; x <= CASTLE_SIZE; x++) {
        // Print row coordinate
        if (x < 10 && CASTLE_SIZE >= 10) {
          print_message2(" %d", x);
        } else {
          print_message2("%d", x);
        }
        for (int y = 1; y <= CASTLE_SIZE; y++) {
          print_message2("|");
          if (x == player->x && y == player->y &&
              levelToDisplay == player->level) {
            print_message2("<p style='color: #00FF00;'>  [YOU] </p>");
          } else if (is_room_discovered(game, x, y, levelToDisplay)) {
            int room_content = get_room_content(game, x, y, levelToDisplay);
            char room_str[10] = "        \0";
            get_room_description(room_content, room_str);
            if (room_content >= MONSTER_START && room_content <= MONSTER_END) {
              monster_level = room_content - MONSTER_START;

              // RGB values
              red = 255; // Keep full red
              // Reduce green and blue as monsters get tougher
              green =
                  150 - (monster_level * 12); // Starts at 150, decreases to ~18
              blue =
                  150 - (monster_level * 12); // Starts at 150, decreases to ~18

              // Ensure we don't go below 0
              if (green < 0)
                green = 0;
              if (blue < 0)
                blue = 0;

              // Format the color string as RGB
              snprintf(color_str, sizeof(color_str), "#%02X%02X%02X", red, green, blue);
              print_message2("<p style='color: %s;'>%s</p>", color_str,
                             room_str);
            } else if (room_content >= TREASURE_START &&
                       room_content <= TREASURE_END) {
              // Gold color for treasures
              print_message2("<p style='color: #FFD700;'>%s</p>", room_str);
            } else if (room_content == VENDOR) {
              // Blue for vendors
              print_message2("<p style='color: #4169E1;'>%s</p>", room_str);
            } else if (room_content == STAIRS_UP ||
                       room_content == STAIRS_DOWN) {
              // Orange for stairs
              print_message2("<p style='color: #FFA500;'>%s</p>", room_str);
            } else if (room_content == POOL) {
              // Cyan for pools
              print_message2("<p style='color: #00FFFF;'>%s</p>", room_str);
            } else if (room_content == CHEST) {
              // Brown for chests
              print_message2("<p style='color: #8B4513;'>%s</p>", room_str);
            } else if (room_content == CRYSTAL_ORB) {
              // Purple for crystal orbs
              print_message2("<p style='color: #800080;'>%s</p>", room_str);
            } else if (room_content == BOOK) {
              // Tan for books
              print_message2("<p style='color: #D2B48C;'>%s</p>", room_str);
            } else if (room_content == WARP) {
              // Magenta for warps
              print_message2("<p style='color: #FF00FF;'>%s</p>", room_str);
            } else if (room_content == FLARES) {
              // Orange-red for flares
              print_message2("<p style='color: #FF4500;'>%s</p>", room_str);
            } else if (room_content == GOLD) {
              // Yellow for gold
              print_message2("<p style='color: #FFD700;'>%s</p>", room_str);
            } else if (room_content == SINKHOLE) {
              print_message2("<span style='color: #800080;'>%s</span>",
                             room_str); // Dark Purple
            } else if (room_content == ENTRANCE) {
              print_message2(
                  "<span style='color: #FFD700; font-weight: bold;'>%s</span>",
                  room_str); // Bold Gold
            } else {
              print_message2(room_str);
            }
          } else {
            print_message2("????????");
          }
        }
        print_message2("|\n");

        // Print horizontal border between rows
        if (x < CASTLE_SIZE) {
          if (CASTLE_SIZE >= 10) {
            print_message2(" ");
          }
          print_message2(" +");
          for (int x = 1; x <= CASTLE_SIZE; x++) {
            print_message2("--------+");
          }
          print_message2("\n");
        }
      }

      // Print bottom border
      if (CASTLE_SIZE >= 10) {
        print_message2(" ");
      }
      print_message2(" +");
      for (int x = 1; x <= CASTLE_SIZE; x++) {
        print_message2("--------+");
      }
      print_message2("\n");
      // Map is too large for the default font for MS-DOS (Same Information is
      // available in help)
      print_message2("\nLEGEND:\n");
      print_message2("[YOU]    = Your location   EMPTY    = Empty room     "
                     "ENTRANCE = Entrance\n");
      print_message2("POOL     = Magic Pool      CHEST    = Treasure Chest\n");
      print_message2("GOLD     = Gold Pieces     FLARES   = Flares\n");
      print_message2("WARP     = Warp/Orb        SINKHOLE = Sinkhole\n");
      print_message2("CRYSTAL  = Crystal Orb     BOOK     = Magic Book\n");
      print_message2("MONSTER  = Monster Name    VENDOR   = Vendor\n");
      print_message2("TREASURE = Treasure Name   ???????? = Undiscovered\n");
      print_message2("STAIRS U = Stairs Up       STAIRS D = Stairs Down\n");

      print_message_status("=== PLAYER STATUS ===\n");

    } else {
      print_message2("\nBlind %s can't see maps.\n\n", lowercase_species);
    }

    // Print player race and attributes
    print_message_status("Race: %s", get_race_name_flc(player->race));

    print_message_status("   Sex: ");
    if (player->sex == FEMALE) {
      print_message_status("Female");
    } else {
      print_message_status("Male");
    }
    print_message_status("\n");
    print_message_status("Strength: %d  Intelligence: %d  Dexterity: %d\n",
                   player->strength, player->intelligence, player->dexterity);

    // Print player position
    print_message_status("Location: Level %d, Room (%d, %d)\n", player->level,
                   player->x, player->y);

    // Print player inventory
    print_message_status("Gold Pieces: %d  Flares: %d\n", player->gold,
                   player->flares);

    // Print armor and weapon
    const char *armor_types[] = {"No Armor", "Leather", "Chainmail", "Plate",
                                 "Stone"};
    const char *weapon_types[] = {"No Weapon", "Dagger", "Mace", "Sword",
                                  "Excalibur"};
    print_message_status("Armor: %s  Weapon: %s\n", armor_types[player->armor_type],
                   weapon_types[player->weapon_type]);
    // Print special items

    print_message_status("Special Items: ");
    if (player->lamp_flag)
      print_message_status("Lamp ");
    if (player->runestaff_flag)
      print_message_status("Runestaff ");
    if (player->orb_flag)
      print_message_status("Orb of Zot ");
    print_message_status("\n");

    // Print curses or blessings
    print_message_status("Status Effects: ");
    if (player->blindness_flag)
      print_message_status("Blind ");
    if (player->stickybook_flag)
      print_message_status("Sticky Book ");
    print_message_status("\n");

    print_message_status("Armor points: %d\n", player->armor_points);

    // Print number of treasures

    for (int i = 0; i < TREASURE_END - TREASURE_START + 1; i++) {
      if (game->treasure[i] >= 1) {
        treasurecount += game->treasure[i];
      }
    }

    print_message_status("Treasures Found: %d\n", treasurecount);

    for (int i = 0; i < TREASURE_END - TREASURE_START + 1; i++) {
      if (game->treasure[i]) {
        print_message_status("     ");
        print_message_status(get_treasure_name(i));
        if (game->treasure[i] > 1) {
          print_message_status(" * %d", game->treasure[i]);
        }
        print_message_status("\n");
      }
    }
    print_message_status("Turn count: %d\n", game->turn_count);

    print_message_status("======================\n\n");
    // mapDisplay->append("");
  }

  void print_message2(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    QString message = QString::fromUtf8(buffer);
    message = "<span style='font-family: Consolas, \"Courier New\", Monaco, "
              "monospace; "
              "white-space: pre; "
              "line-height: 1em; "
              "display: inline-block;'>" +
              message + "</span>";
    appendToMap(message);
  }

  void print_message_status(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    QString message = QString::fromUtf8(buffer);
    message = "<span style='font-family: Consolas, \"Courier New\", Monaco, "
              "monospace; "
              "white-space: pre; "
              "line-height: 1em; "
              "display: inline-block;'>" +
              message + "</span>";
    appendToMap_status(message);
  }


  void print_message2_formatted(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);

    capitalize_sentences(buffer); // Use the function from utilities.c

    va_end(args);

    QString message = QString::fromUtf8(buffer);
    message = "<span style='font-family: Consolas, \"Courier New\", Monaco, "
              "monospace; "
              "white-space: pre; "
              "line-height: 1em; "
              "display: inline-block;'>" +
              message + "</span>";
    appendToMap(message);
  }

  void appendToMap(const QString &text) {
    mapDisplay->moveCursor(QTextCursor::End);
    mapDisplay->insertHtml(text);
    mapDisplay->verticalScrollBar()->setValue(
        outputText->verticalScrollBar()->maximum());
  }

  void appendToMap_status(const QString &text) {
    statsDisplay->moveCursor(QTextCursor::End);
    statsDisplay->insertHtml(text);
    statsDisplay->verticalScrollBar()->setValue(
        outputText->verticalScrollBar()->maximum());
  }


  void appendToOutput(const QString &text) {
    outputText->moveCursor(QTextCursor::End);
    outputText->insertPlainText(text);
    outputText->verticalScrollBar()->setValue(
        outputText->verticalScrollBar()->maximum());
  }

  bool inputIsEmpty() const { return lastInput.empty(); }

  std::string getLastInput() {
    std::string input = lastInput;
    lastInput.clear();
    return input;
  }

  void clearInput() {
    inputLine->clear();
    lastInput.clear();
  }

  void clearOutput() 
  { 
       outputText->clear(); 
       statusDisplay->clear();
       statsDisplay->clear();
       mapDisplay->clear();
  }

  void setWaitingForSpecificInput(bool waiting,
                                  const std::string &validInputs) {
    waitingForSpecificInput = waiting;
    this->validInputs = validInputs;
  }

  bool isWaitingForSpecificInput() const { return waitingForSpecificInput; }

  void setGamePointers(Player *player, GameState *game) {
    g_player = player;
    g_game = game;
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
      palette.setColor(QPalette::Base, QColor(0, 43, 54)); // Dark blue-gray
      palette.setColor(QPalette::Text,
                       QColor(131, 148, 150)); // Light gray-blue
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
      palette.setColor(QPalette::Base, QColor(46, 52, 64)); // Dark blue-gray
      palette.setColor(QPalette::Text,
                       QColor(216, 222, 233)); // Light gray-blue
    } else {                                   // Default
      /*palette.setColor(QPalette::Base, QColor(46, 52, 64));    // Dark
      blue-gray palette.setColor(QPalette::Text, QColor(216, 222, 233)); //
      Light gray-blue */
      palette = QApplication::style()->standardPalette();
    }
    inputLine->setPalette(palette);
    outputText->setPalette(palette);
    inputLine->setPalette(palette);
    mapDisplay->setPalette(palette);
    statusDisplay->setPalette(palette);
    statsDisplay->setPalette(palette);

  }

signals:
  void programExit();
  void gameStateChanged();
  void newGameRequested();
  void inputReady();

protected:
  void closeEvent(QCloseEvent *event) override {
    if (g_player && g_game) {
      // Attempt to save the current game state
      if (g_game->game_over == 0) {
        save_game("lastcastle.wcs", g_player, g_game);
      }
    }
    event->accept();
    std::exit(0);
  }

  bool eventFilter(QObject *obj, QEvent *event) override {
    if (obj == outputText && event->type() == QEvent::Wheel) {
      QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
      if (wheelEvent->modifiers() & Qt::ControlModifier) {
        const int delta = wheelEvent->angleDelta().y();
        if (delta > 0) {
          increaseFontSize();
        } else if (delta < 0) {
          decreaseFontSize();
        }
        return true;
      }
    } else if (obj == inputLine && event->type() == QEvent::KeyPress) {
      QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
      if (keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_Down) {
        handleCommandHistory(keyEvent->key());
        return true;
      }
    }
    return QMainWindow::eventFilter(obj, event);
  }

private slots:

  void showCheat() {
    int fogOfWarChecked;
    if (!g_player) {
      QMessageBox::warning(this, tr("Error"), tr("No active game found."));
      return;
    }
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Cheat Menu"));
    dialog.setModal(true);
    dialog.setMinimumWidth(400);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);

    // Stats section
    QGroupBox *statsGroup = new QGroupBox(tr("Stats"), &dialog);
    QGridLayout *statsLayout = new QGridLayout(statsGroup);

    // Character section
    QGroupBox *charGroup = new QGroupBox(tr("Character"), &dialog);
    QGridLayout *charLayout = new QGridLayout(charGroup);

    // Race selection
    QComboBox *raceBox = new QComboBox(&dialog);
    raceBox->addItems({"Hobbit", "Elf", "Human", "Dwarf", "Drow"});
    raceBox->setCurrentIndex(g_player->race - 1); // Race enum starts at 1
    charLayout->addWidget(new QLabel(tr("Race:")), 0, 0);
    charLayout->addWidget(raceBox, 0, 1);

    // Sex selection
    QComboBox *sexBox = new QComboBox(&dialog);
    sexBox->addItems({"Female", "Male"});
    sexBox->setCurrentIndex(g_player->sex); // FEMALE is 0, MALE is 1
    charLayout->addWidget(new QLabel(tr("Sex:")), 1, 0);
    charLayout->addWidget(sexBox, 1, 1);

    mainLayout->addWidget(charGroup);

    // Strength
    QSpinBox *xlocation = new QSpinBox(&dialog);
    xlocation->setRange(1, CASTLE_SIZE);
    xlocation->setValue(g_player->y);
    statsLayout->addWidget(new QLabel(tr("Position X:")), 0, 3);
    statsLayout->addWidget(xlocation, 0, 4);

    QSpinBox *ylocation = new QSpinBox(&dialog);
    ylocation->setRange(1, CASTLE_SIZE);
    ylocation->setValue(g_player->x);
    statsLayout->addWidget(new QLabel(tr("Position y:")), 1, 3);
    statsLayout->addWidget(ylocation, 1, 4);

    QSpinBox *zlocation = new QSpinBox(&dialog);
    zlocation->setRange(1, CASTLE_SIZE);
    zlocation->setValue(g_player->level);
    statsLayout->addWidget(new QLabel(tr("Level:")), 2, 3);
    statsLayout->addWidget(zlocation, 2, 4);
    statsLayout->addWidget(new QLabel(tr("[Entrance is at (4,1) Level 1]")), 3,
                           3);

    // Strength
    QSpinBox *strengthBox = new QSpinBox(&dialog);
    strengthBox->setRange(0, MAX_STRENGTH);
    strengthBox->setValue(g_player->strength);
    statsLayout->addWidget(new QLabel(tr("Strength:")), 0, 0);
    statsLayout->addWidget(strengthBox, 0, 1);

    // Intelligence
    QSpinBox *intelligenceBox = new QSpinBox(&dialog);
    intelligenceBox->setRange(0, MAX_INTELLIGENCE);
    intelligenceBox->setValue(g_player->intelligence);
    statsLayout->addWidget(new QLabel(tr("Intelligence:")), 1, 0);
    statsLayout->addWidget(intelligenceBox, 1, 1);

    // Dexterity
    QSpinBox *dexterityBox = new QSpinBox(&dialog);
    dexterityBox->setRange(0, MAX_DEXTERITY);
    dexterityBox->setValue(g_player->dexterity);
    statsLayout->addWidget(new QLabel(tr("Dexterity:")), 2, 0);
    statsLayout->addWidget(dexterityBox, 2, 1);

    // Armor Points
    QSpinBox *armorPointsBox = new QSpinBox(&dialog);
    armorPointsBox->setRange(0, MAX_ARMOR_POINTS);
    armorPointsBox->setValue(g_player->armor_points);
    statsLayout->addWidget(new QLabel(tr("Armor Points:")), 3, 0);
    statsLayout->addWidget(armorPointsBox, 3, 1);

    // Equipment section
    QGroupBox *equipGroup = new QGroupBox(tr("Equipment"), &dialog);
    QGridLayout *equipLayout = new QGridLayout(equipGroup);

    QComboBox *weaponBox = new QComboBox(&dialog);
    weaponBox->addItems({"Nothing", "Dagger", "Mace", "Sword", "Excalibur"});
    weaponBox->setCurrentIndex(g_player->weapon_type);
    equipLayout->addWidget(new QLabel(tr("Weapon:")), 0, 0);
    equipLayout->addWidget(weaponBox, 0, 1);

    QComboBox *armorBox = new QComboBox(&dialog);
    armorBox->addItems({"Nothing", "Leather", "Chainmail", "Plate", "Stone"});
    armorBox->setCurrentIndex(g_player->armor_type);
    equipLayout->addWidget(new QLabel(tr("Armor:")), 1, 0);
    equipLayout->addWidget(armorBox, 1, 1);

    mainLayout->addWidget(equipGroup);

    // Flags section
    QGroupBox *flagsGroup = new QGroupBox(tr("Status"), &dialog);
    QGridLayout *flagsLayout = new QGridLayout(flagsGroup);

    QCheckBox *lampFlag = new QCheckBox(tr("Lamp"), &dialog);
    lampFlag->setChecked(g_player->lamp_flag);
    flagsLayout->addWidget(lampFlag, 0, 0);

    QCheckBox *blindnessFlag = new QCheckBox(tr("Blindness"), &dialog);
    blindnessFlag->setChecked(g_player->blindness_flag);
    flagsLayout->addWidget(blindnessFlag, 0, 1);

    QCheckBox *stickyBookFlag = new QCheckBox(tr("Sticky Book"), &dialog);
    stickyBookFlag->setChecked(g_player->stickybook_flag);
    flagsLayout->addWidget(stickyBookFlag, 1, 0);

    QCheckBox *orbFlag = new QCheckBox(tr("Orb of Zot"), &dialog);
    orbFlag->setChecked(g_player->orb_flag);
    flagsLayout->addWidget(orbFlag, 1, 1);

    QString orbLocation =
        QString("The Orb of Zot is located at (%1, %2) on level %3")
            .arg(g_game->orb_location[1])
            .arg(g_game->orb_location[0])
            .arg(g_game->orb_location[2]);
    QLabel *orbLocationLabel = new QLabel(orbLocation, &dialog);
    flagsLayout->addWidget(orbLocationLabel, 7, 0, 1, 2);

    QString runeLocation =
        QString("The Rune Staff is located at (%1, %2) on level %3")
            .arg(g_game->runestaff_location[1])
            .arg(g_game->runestaff_location[0])
            .arg(g_game->runestaff_location[2]);
    QLabel *runeLocationLabel = new QLabel(runeLocation, &dialog);
    flagsLayout->addWidget(runeLocationLabel, 8, 0, 1, 2);

    QCheckBox *runestaffFlag = new QCheckBox(tr("Runestaff"), &dialog);
    runestaffFlag->setChecked(g_player->runestaff_flag);
    flagsLayout->addWidget(runestaffFlag, 2, 0);

    QCheckBox *fogOfWar =
        new QCheckBox(tr("Display Entire Map (not reversable)"), &dialog);
    flagsLayout->addWidget(fogOfWar, 2, 1);

    QCheckBox *rubyRed = new QCheckBox(tr("Ruby Red"), &dialog);
    rubyRed->setChecked(g_game->treasure[0]);
    flagsLayout->addWidget(rubyRed, 3, 0);

    QCheckBox *nornStone = new QCheckBox(tr("Norn Stone"), &dialog);
    nornStone->setChecked(g_game->treasure[1]);
    flagsLayout->addWidget(nornStone, 3, 1);

    QCheckBox *palePearl = new QCheckBox(tr("Pale Pearl"), &dialog);
    palePearl->setChecked(g_game->treasure[2]);
    flagsLayout->addWidget(palePearl, 4, 0);

    QCheckBox *opalEye = new QCheckBox(tr("Opal Eye"), &dialog);
    opalEye->setChecked(g_game->treasure[3]);
    flagsLayout->addWidget(opalEye, 4, 1);

    QCheckBox *greenGem = new QCheckBox(tr("Green Gem"), &dialog);
    greenGem->setChecked(g_game->treasure[4]);
    flagsLayout->addWidget(greenGem, 5, 0);

    QCheckBox *blueFlame = new QCheckBox(tr("Blue Flame"), &dialog);
    blueFlame->setChecked(g_game->treasure[5]);
    flagsLayout->addWidget(blueFlame, 5, 1);

    QCheckBox *palantir = new QCheckBox(tr("Palantir"), &dialog);
    palantir->setChecked(g_game->treasure[6]);
    flagsLayout->addWidget(palantir, 6, 0);

    QCheckBox *silmaril = new QCheckBox(tr("Silmaril"), &dialog);
    silmaril->setChecked(g_game->treasure[7]);
    flagsLayout->addWidget(silmaril, 6, 1);

    mainLayout->addWidget(flagsGroup);

    // Resources section
    QGroupBox *resourcesGroup = new QGroupBox(tr("Resources"), &dialog);
    QGridLayout *resourcesLayout = new QGridLayout(resourcesGroup);

    QSpinBox *goldBox = new QSpinBox(&dialog);
    goldBox->setRange(0, 1048576);
    goldBox->setValue(g_player->gold);
    resourcesLayout->addWidget(new QLabel(tr("Gold:")), 0, 0);
    resourcesLayout->addWidget(goldBox, 0, 1);

    QSpinBox *flaresBox = new QSpinBox(&dialog);
    flaresBox->setRange(0, 1048576);
    flaresBox->setValue(g_player->flares);
    resourcesLayout->addWidget(new QLabel(tr("Flares:")), 1, 0);
    resourcesLayout->addWidget(flaresBox, 1, 1);

    mainLayout->addWidget(resourcesGroup);

    // Dialog buttons
    QDialogButtonBox *buttonBox =
        new QDialogButtonBox(QDialogButtonBox::Apply | QDialogButtonBox::Cancel,
                             Qt::Horizontal, &dialog);
    mainLayout->addWidget(buttonBox);

    mainLayout->addWidget(statsGroup);

    connect(buttonBox, &QDialogButtonBox::clicked, [&]() {
      // Apply all changes
      g_player->race =
          raceBox->currentIndex() + 1; // Add 1 because race enum starts at 1
      g_player->sex = sexBox->currentIndex(); // 0 for Female, 1 for Male
      g_player->strength = strengthBox->value();
      g_player->intelligence = intelligenceBox->value();
      g_player->dexterity = dexterityBox->value();
      g_player->armor_points = armorPointsBox->value();
      g_player->weapon_type = weaponBox->currentIndex();
      g_player->armor_type = armorBox->currentIndex();
      g_player->lamp_flag = lampFlag->isChecked();
      g_player->blindness_flag = blindnessFlag->isChecked();
      g_player->stickybook_flag = stickyBookFlag->isChecked();
      g_player->orb_flag = orbFlag->isChecked();
      g_player->runestaff_flag = runestaffFlag->isChecked();
      g_player->gold = goldBox->value();
      g_player->flares = flaresBox->value();
      g_player->x = ylocation->value();
      g_player->y = xlocation->value();
      g_player->level = zlocation->value();

      fogOfWarChecked = fogOfWar->isChecked();
      if (!g_game->treasure[0] && rubyRed->isChecked())
        g_game->treasure[0] = 1;
      if (!g_game->treasure[1] && nornStone->isChecked())
        g_game->treasure[1] = 1;
      if (!g_game->treasure[2] && palePearl->isChecked())
        g_game->treasure[2] = 1;
      if (!g_game->treasure[3] && opalEye->isChecked())
        g_game->treasure[3] = 1;
      if (!g_game->treasure[4] && greenGem->isChecked())
        g_game->treasure[4] = 1;
      if (!g_game->treasure[5] && blueFlame->isChecked())
        g_game->treasure[5] = 1;
      if (!g_game->treasure[6] && palantir->isChecked())
        g_game->treasure[6] = 1;
      if (!g_game->treasure[7] && silmaril->isChecked())
        g_game->treasure[7] = 1;
      if (fogOfWarChecked) {
        for (int q = 0; q < MAP_SIZE; q++) {
          g_game->discovered_rooms[q] = 1; // 0 means undiscovered
        }
      }
      // Update the display
      // updateMap();
    });

    dialog.exec();
  }

  void showAbout() {
    // Calculate castle dimensions correctly
    int roomsPerLevel = CASTLE_SIZE * CASTLE_SIZE; // rooms on each level
    int totalRooms =
        CASTLE_SIZE * CASTLE_SIZE * CASTLE_SIZE; // total rooms in the castle

    QMessageBox aboutBox(this);
    aboutBox.setWindowTitle("About The Wizard's Castle");
    aboutBox.setTextFormat(Qt::RichText);
    aboutBox.setText(
        tr("<h3>The Wizard's Castle</h3>"
           "<p><i>Hear ye, brave adventurer! Within these ancient walls lies a "
           "tale of courage, magic, and destiny...</i><br/><br/>"

           "Deep within the realms of digital enchantment lies a legendary "
           "fortress - The Wizard's Castle. "
           "Whispered tales speak of the mythical Orb of Zot, a relic of "
           "unimaginable power, hidden within its ever-changing halls. "
           "Will you answer the call to adventure?<br/>"
           "<br/>"
           "<b>⚔️ Your Quest Awaits ⚔️</b><br/>"
           "• Journey through %3 mysterious levels, each containing %1 "
           "chambers of wonder and peril<br/>"
           "• Explore a vast castle of %2 rooms across %3×%3 floors<br/>"
           "• Choose your adventurer from noble races: Hardy Dwarves, Nimble "
           "Elves, Cunning Hobbits, or Versatile Humans<br/>"
           "• Face fearsome creatures from legend - from mischievous Kobolds "
           "to the dreaded Balrog<br/>"
           "• Master arcane treasures: The Ruby Red, The Pale Pearl, and other "
           "artifacts of power<br/>"
           "• Barter with the mysterious Vendor, keeper of potent magical "
           "wares<br/>"
           "• Wield weapons from simple daggers to the legendary Excalibur<br/>"
           "• Don armor ranging from humble leather to enchanted platemail<br/>"
           "• Navigate treacherous chambers with magical flares and an "
           "enchanted lamp<br/>"
           "<br/>"
           "<i>But beware, intrepid soul! The castle's depths hold many "
           "dangers:</i><br/>"
           "• Lethal combat with mythical beasts<br/>"
           "• Cursed books of forbidden knowledge<br/>"
           "• Mysterious pools of magical essence<br/>"
           "• Sinkholes leading to deeper levels<br/>"
           "• And the ever-present threat of the dreaded Vendor's wrath!<br/>"
           "<br/>"
           "Arm yourself with wit and wisdom - Type 'H' to consult your "
           "adventurer's tome, or 'M' to unfurl your magical map.<br/>"
           "<br/>"
           "<i>Remember, brave hero: Fortune favors the bold, but only the "
           "wise survive the Wizard's Castle!</i><br/><br/>"

           "<font size='2'>This magical tome's source incantations can be "
           "found in the Grand Library of: "
           "<a href='https://github.com/jasonbrianhall/wizardscastle'>The "
           "Wizard's Castle Repository</a><br/>"
           "Enchanted by Jason Hall &lt;jasonbrianhall@gmail.com&gt; in "
           "2024</font></p>")
            .arg(roomsPerLevel) // rooms per level (CASTLE_SIZE^2)
            .arg(totalRooms)    // total rooms (CASTLE_SIZE^3)
            .arg(CASTLE_SIZE)); // dimension size and number of levels

    // Enable link opening
    aboutBox.setTextInteractionFlags(Qt::TextBrowserInteraction);
    aboutBox.setIcon(QMessageBox::Information);

    // Set a minimum width to make the text more readable
    aboutBox.setMinimumWidth(4096);
    aboutBox.exec();
  }
  void updateMap() {
    if (g_player && g_game) {
      mapDisplay->clear(); // Clear the previous map
      statusDisplay->clear();
      statsDisplay->clear();
      display_map2(g_game, g_player);
    }
  }

  void saveGame() {
    if (g_game->game_over == 1) {
      QMessageBox::warning(this, tr("Save Failed"),
                           tr("Dead adventurers can't save."));
      return;
    }
    if (!g_player || !g_game) {
      QMessageBox::warning(this, tr("Save Failed"),
                           tr("No active game to save."));
      return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save Game"), "", tr("Wizard's Castle Save (*.wcs)"));
    if (fileName.isEmpty()) {
      return;
    }

    // Ensure the file has a .wcs extension
    if (!fileName.endsWith(".wcs", Qt::CaseInsensitive)) {
      fileName += ".wcs";
    }

    if (save_game(fileName.toStdString().c_str(), g_player, g_game)) {
      QMessageBox::information(this, tr("Game Saved"),
                               tr("Your game has been saved successfully."));
    } else {
      QMessageBox::warning(this, tr("Save Failed"),
                           tr("Failed to save the game. Please try again."));
    }
  }

  void qsaveGame() {
    if (g_game->game_over == 1) {
      QMessageBox::warning(this, tr("Save Failed"),
                           tr("Dead adventurers can't save."));
      return;
    }
    if (!g_player || !g_game) {
      QMessageBox::warning(this, tr("Save Failed"),
                           tr("No active game to save."));
      return;
    }

    if (save_game("quicksave.wcs", g_player, g_game)) {
      /*QMessageBox::information(this, tr("Game Saved"),
                               tr("Your game has been saved successfully.")); */  // Do nothing
    } else {
      QMessageBox::warning(this, tr("Save Failed"),
                           tr("Failed to save the game. Please try again."));
    }
  }


  void loadGame() {
    if (!g_player || !g_game) {
      QMessageBox::warning(this, tr("Load Failed"),
                           tr("Cannot load game at this time."));
      return;
    }

    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Load Game"), "",
        tr("Wizard's Castle Save (*.wcs);;All Files (*)"));
    if (fileName.isEmpty()) {
      return;
    }

    if (load_game(fileName.toStdString().c_str(), g_player, g_game)) {
        QMessageBox::information(this, tr("Game Loaded"),
                               tr("Your game has been loaded successfully."));
      // You might want to trigger an update of the UI here
      emit gameStateChanged();
    } else {
      QMessageBox::warning(this, tr("Load Failed"),
                           tr("Failed to load the game. The file might be "
                              "corrupted or incompatible."));
    }
  }

  void qloadGame() {
    if (!g_player || !g_game) {
      QMessageBox::warning(this, tr("Load Failed"),
                           tr("Cannot load game at this time."));
      return;
    }

    if (load_game("quicksave.wcs", g_player, g_game)) {
      /* QMessageBox::information(this, tr("Game Loaded"),
                               tr("Your game has been loaded successfully.")); */
      // You might want to trigger an update of the UI here
      emit gameStateChanged();
    } else {
      QMessageBox::warning(this, tr("Load Failed"),
                           tr("Failed to load the game. The file might be "
                              "corrupted or incompatible."));
    }
  }




  void newGame() {
    // Ask for confirmation before starting a new game
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "New Game",
                                  "Are you sure you want to start a new game? "
                                  "Any unsaved progress will be lost.",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      emit newGameRequested();
    }
  }

  void quit() { close(); }

void processInput() {
    QString input = inputLine->text().toUpper();
    inputLine->clear();
    currentHistoryIndex = -1;

    if (!input.isEmpty()) {
        commandHistory.append(input);
        while (commandHistory.size() > 100) {
            commandHistory.removeFirst();
        }
    }

    if (waitingForSpecificInput) {
        if (validInputs.find(input[0].toLatin1()) != std::string::npos) {
            lastInput = input[0].toLatin1();
            waitingForSpecificInput = false;
            emit inputReady(); // Add signal to notify input is ready
        } else {
            appendToOutput(QString("Please enter one of the following: %1\n")
                .arg(QString::fromStdString(validInputs)));
        }
    } else {
        lastInput = input.toStdString();
        emit inputReady(); // Add signal to notify input is ready
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
  QTextEdit *statusDisplay;
  QTextEdit *statsDisplay;

  QTimer *mapUpdateTimer;
  std::string lastInput;
  bool waitingForSpecificInput;
  std::string validInputs;
  int fontSize;

  int displayLevel = -1; // -1 means show player's current level
  int lastPlayerX = -1;  // Track player position
  int lastPlayerY = -1;
  int lastPlayerLevel = -1;

  QStringList commandHistory;
  int currentHistoryIndex;
  QString currentInput;

  void checkForLastGame() {
    QFile file("lastcastle.wcs");
    if (file.exists()) {
      QMessageBox::StandardButton reply;
      reply = QMessageBox::question(
          this, "Resume Game", "Would you like to continue your last game?",
          QMessageBox::Yes | QMessageBox::No);

      if (reply == QMessageBox::Yes) {
        if (load_game("lastcastle.wcs", g_player, g_game)) {
          QMessageBox::information(
              this, "Game Loaded",
              "Your previous game has been loaded successfully.");
          emit gameStateChanged();
        } else {
          QMessageBox::warning(
              this, "Load Failed",
              "Failed to load the previous game. Starting a new game.");
          emit newGameRequested();
        }
      } else {
        // User chose not to load the last game
        emit newGameRequested();
      }
    } else {
      // No previous game found
      emit newGameRequested();
    }
  }

  void handleCommandHistory(int key) {
    if (commandHistory.isEmpty()) {
      return;
    }

    // Save current input if we're just starting to navigate history
    if (currentHistoryIndex == -1) {
      currentInput = inputLine->text();
    }

    if (key == Qt::Key_Up) {
      // Move backwards through history
      if (currentHistoryIndex < commandHistory.size() - 1) {
        currentHistoryIndex++;
        inputLine->setText(
            commandHistory[commandHistory.size() - 1 - currentHistoryIndex]);
      }
    } else if (key == Qt::Key_Down) {
      // Move forwards through history
      if (currentHistoryIndex > 0) {
        currentHistoryIndex--;
        inputLine->setText(
            commandHistory[commandHistory.size() - 1 - currentHistoryIndex]);
      } else if (currentHistoryIndex == 0) {
        // Restore the current input when reaching the bottom of history
        currentHistoryIndex = -1;
        inputLine->setText(currentInput);
      }
    }

    // Move cursor to end of line
    inputLine->setCursorPosition(inputLine->text().length());
  }

  void createMenus() {
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *fileMenu = menuBar->addMenu(tr("&File"));

    QAction *newGameAction = new QAction(tr("&New Game"), this);
    newGameAction->setShortcut(QKeySequence::New);
    connect(newGameAction, &QAction::triggered, this,
            &WizardsCastleWindow::newGame);
    fileMenu->addAction(newGameAction);

    QAction *saveAction = new QAction(tr("&Save Game"), this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this,
            &WizardsCastleWindow::saveGame);
    fileMenu->addAction(saveAction);

    QAction *qsaveAction = new QAction(tr("Quick save Game"), this);
    qsaveAction->setShortcut(QKeySequence(Qt::Key_F5));
    connect(qsaveAction, &QAction::triggered, this,
            &WizardsCastleWindow::qsaveGame);
    fileMenu->addAction(qsaveAction);

    QAction *qloadAction = new QAction(tr("Quick load Game"), this);
    qloadAction->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_F5));
    connect(qloadAction, &QAction::triggered, this,
            &WizardsCastleWindow::qloadGame);
    fileMenu->addAction(qloadAction);

    QAction *loadAction = new QAction(tr("Quick Load Game"), this);
    loadAction->setShortcut(QKeySequence::Open);
    connect(loadAction, &QAction::triggered, this,
            &WizardsCastleWindow::loadGame);
    fileMenu->addAction(qloadAction);

    fileMenu->addSeparator();
    QAction *quitAction = new QAction(tr("&Quit"), this);
    quitAction->setShortcuts(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &WizardsCastleWindow::quit);
    fileMenu->addAction(quitAction);

    QMenu *optionsMenu = menuBar->addMenu(tr("&Options"));

    QMenu *colorSchemeMenu = optionsMenu->addMenu(tr("&Color Scheme"));

    QActionGroup *colorSchemeGroup = new QActionGroup(this);
    colorSchemeGroup->setExclusive(true);

    QStringList schemes = {"Default",
                           "Commodore 64",
                           "Blue and White",
                           "Black and White",
                           "White and Black",
                           "Solarized Light",
                           "Solarized Dark",
                           "Monokai",
                           "Gruvbox Light",
                           "Gruvbox Dark",
                           "Nord"};

    for (const QString &scheme : schemes) {
      QAction *schemeAction = new QAction(scheme, this);
      schemeAction->setCheckable(true);
      colorSchemeMenu->addAction(schemeAction);
      colorSchemeGroup->addAction(schemeAction);
      connect(schemeAction, &QAction::triggered,
              [this, scheme]() { setColorScheme(scheme); });
      if (scheme == "Default") {
        schemeAction->setChecked(true);
      }
    }

    QAction *cheatAction = new QAction(tr("&Cheat"), this);
    connect(cheatAction, &QAction::triggered, this,
            &WizardsCastleWindow::showCheat);
    optionsMenu->addAction(cheatAction);

    QMenu *viewMenu = menuBar->addMenu(tr("&View"));
    QMenu *levelMenu = viewMenu->addMenu(tr("&Level"));

    // Add numbered levels
    for (int i = 1; i <= CASTLE_SIZE; i++) {
      QAction *levelAction = levelMenu->addAction(tr("Level %1").arg(i));
      connect(levelAction, &QAction::triggered, [this, i]() {
        displayLevel = i;
        updateMap();
      });
    }

    // Add Help menu
    QMenu *helpMenu = menuBar->addMenu(tr("&Help"));
    QAction *aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, &QAction::triggered, this,
            &WizardsCastleWindow::showAbout);
    helpMenu->addAction(aboutAction);
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

WizardsCastleWindow *g_window = nullptr;

extern "C" {

bool parse_arguments(int argc, char *argv[]) {
  std::vector<std::string> args(argv + 1, argv + argc);
  return std::find(args.begin(), args.end(), "--debug") != args.end();
}

void initialize_qt(int argc, char *argv[]) {
  QApplication *app = new QApplication(argc, argv);
  g_window = new WizardsCastleWindow();
  g_window->show();

  Player player;
  GameState game;

  g_window->setGamePointers(&player, &game);

  // Use a single-shot timer to set the color scheme after the event loop starts
  QTimer::singleShot(0, [&]() { g_window->setColorScheme("Default"); });

  bool debug_mode = parse_arguments(argc, argv);
  int q, playagain = 1;

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
      player.race = ELF; // Elf
      player.sex = MALE; // Male
      player.strength = MAX_STRENGTH;
      player.intelligence = MAX_INTELLIGENCE;
      player.dexterity = MAX_DEXTERITY;
      player.gold = 10000;
      player.flares = 1000;
      player.lamp_flag = 1;
      player.runestaff_flag = 1;
      player.weapon_type = EXCALIBUR; // Sword
      player.armor_type = STONE;      // Plate
      player.armor_points = MAX_ARMOR_POINTS;

      print_message("DEBUG MODE: You are a male elf with %d Strength, %d "
                    "Intelligence, and %d Dexterity.\n",
                    player.strength, player.intelligence, player.dexterity);
      print_message("DEBUG MODE: You have Excalibur and Stone armor.\n");
      print_message("DEBUG MODE: You start with the Runestaff, 10000 gold, "
                    "1000 flares, and a lamp.\n");
      print_message("DEBUG MODE: All rooms are discovered\n");
      for (q = 0; q < TREASURE_END - TREASURE_START + 1; q++) {
        print_message("%d", q);
        game.treasure[q] = 1;
      }
    }

    if (debug_mode) {
      print_message(
          "DEBUG MODE: The Orb of Zot is located at (%d, %d) on level %d.\n",
          game.orb_location[1], game.orb_location[0], game.orb_location[2]);
      for (q = 0; q < MAP_SIZE; q++) {
        game.discovered_rooms[q] = 1; // 0 means undiscovered
      }
    }
  };
  QObject::connect(g_window, &WizardsCastleWindow::newGameRequested,
                   startNewGame);

  // Start the initial game
  // startNewGame();

  while (playagain) {
    startNewGame();
    playagain = main_game_loop(&player, &game);
    if (playagain) {
    }
  }
  // app->exec();
  delete g_window;
  delete app;
}

const char* get_user_input_main() {
    static std::string input;
    static const char* dr_command = "DR";
    static const char* qs_command = "QS";
    static const char* ql_command = "QL";
    
    print_message("ENTER YOUR COMMAND: ");
    g_window->clearInput();
    
    // Create an event loop
    QEventLoop loop;
    QObject::connect(g_window, &WizardsCastleWindow::inputReady, &loop, &QEventLoop::quit);
    loop.exec();
    
    input = g_window->getLastInput();
    input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
    std::transform(input.begin(), input.end(), input.begin(), ::toupper);
    
    if (input.empty()) {
        print_message("\n");
        return input.c_str();
    }
    
    char firstChar = input[0];
    if (firstChar == 'D' && input.length() > 1 && input[1] == 'R') {
        print_message("DR\n");
        return dr_command;
    } else if (firstChar == 'Q' && input.length() > 1 && input[1] == 'S') {
        print_message("QS\n");
        return qs_command;
    } else if (firstChar == 'Q' && input.length() > 1 && input[1] == 'L') {
        print_message("QL\n");
        return ql_command;
    } else if (strchr("ADEFGHILMNOQSTUWYZ", firstChar) != NULL) {
        print_message("%c\n", firstChar);
        return input.c_str();
    }
    
    print_message("Invalid command. Type 'H' for help.\n");
    return get_user_input_main();
}

int get_user_input_number() {
    //print_message("Enter a number: ");
    g_window->clearInput();
    
    while (true) {
        // Create an event loop for waiting for input
        QEventLoop loop;
        QObject::connect(g_window, &WizardsCastleWindow::inputReady, &loop, &QEventLoop::quit);
        loop.exec();
        
        std::string input = g_window->getLastInput();
        
        try {
            int data = std::stoi(input);
            print_message("%d\n", data);
            return data;
        } catch (const std::invalid_argument&) {
            print_message("Invalid input. Please enter a valid integer.\n");
            print_message("Enter a number: ");
            g_window->clearInput();
        } catch (const std::out_of_range&) {
            print_message("Input out of range. Please enter a smaller number.\n");
            print_message("Enter a number: ");
            g_window->clearInput();
        }
    }
}

char get_user_input() {
    print_message("Enter your command: ");
    g_window->clearInput();
    
    QEventLoop loop;
    QObject::connect(g_window, &WizardsCastleWindow::inputReady, &loop, &QEventLoop::quit);
    loop.exec();
    
    std::string input = g_window->getLastInput();
    std::transform(input.begin(), input.end(), input.begin(), ::toupper);
    
    if (input.empty()) {
        print_message("Please enter a command.\n");
        return get_user_input();
    }
    
    char command = input[0];
    print_message("%c\n", command);
    
    if (strchr("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ", command) != NULL) {
        return command;
    }
    
    print_message("Invalid command. Type 'H' for help.\n");
    return get_user_input();
}

char get_user_input_custom_prompt(char *prompt) {
    while (true) {
        print_message("%s", prompt);
        g_window->clearInput();
        
        // Create an event loop to wait for input
        QEventLoop loop;
        QObject::connect(g_window, &WizardsCastleWindow::inputReady, &loop, &QEventLoop::quit);
        loop.exec();
        
        std::string input = g_window->getLastInput();
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        
        if (input.empty()) {
            print_message("Please enter a command.\n");
            continue;
        }
        
        char command = input[0];
        print_message("%c\n", command);
        
        if (strchr("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ", command) != NULL) {
            return command;
        }
        
        print_message("Invalid command. Type 'H' for help.\n");
    }
}

char get_user_input_yn() {
    while (true) {
        g_window->clearInput();
        
        // Create an event loop to wait for input
        QEventLoop loop;
        QObject::connect(g_window, &WizardsCastleWindow::inputReady, &loop, &QEventLoop::quit);
        loop.exec();
        
        std::string input = g_window->getLastInput();
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        
        if (input.empty()) {
            print_message("Please Enter Y or N.\n");
            continue;
        }
        
        char command = input[0];
        print_message("%c\n", command);
        
        if (strchr("YN", command) != NULL) {
            return command;
        }
        
        print_message("Invalid command. Please enter Y or N.\n");
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

  capitalize_sentences(buffer); // Use the function from utilities.c

  va_end(args);

  QString message = QString::fromUtf8(buffer);
  g_window->appendToOutput(message);
}

} // end of extern "C"

#include "wizardioqt5.moc"
