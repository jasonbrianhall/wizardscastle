// wizardioqt5.h
#ifndef WIZARDIOQT5_H
#define WIZARDIOQT5_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QTimer>
#include <string>

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
    WizardsCastleWindow(QWidget *parent = nullptr);
    void appendToMap(const QString& text);
    void appendToOutput(const QString& text);
    bool inputIsEmpty() const;
    std::string getLastInput();
    void clearInput();
    void clearOutput();
    void setWaitingForSpecificInput(bool waiting, const std::string& validInputs);
    bool isWaitingForSpecificInput() const;
    void setGamePointers(Player* player, GameState* game);
    void display_map2(GameState *game, Player *player);
    void print_message2(const char *format, ...);
    void print_message2_formatted(const char *format, ...);

signals:
    void programExit();
    void gameStateChanged();
    void newGameRequested();

protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void updateMap();
    void saveGame();
    void loadGame();
    void newGame();
    void setColorScheme(const QString &scheme);
    void quit();
    void processInput();
    void increaseFontSize();
    void decreaseFontSize();

private:
    QTextEdit *outputText;
    QLineEdit *inputLine;
    QTextEdit *mapDisplay;
    QTimer *mapUpdateTimer;
    std::string lastInput;
    bool waitingForSpecificInput;
    std::string validInputs;
    int fontSize;

    void createMenus();
    void updateFont();
    void cleanup();
};

#ifdef __cplusplus
extern "C" {
#endif

void initialize_qt(int argc, char *argv[]);
bool parse_arguments(int argc, char* argv[]);

#ifdef __cplusplus
}
#endif

#endif // WIZARDIOQT5_H
