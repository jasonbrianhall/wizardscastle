#ifndef WIZARDIOQT5_H
#define WIZARDIOQT5_H

#include <QMainWindow>

class WizardsCastleWindow : public QMainWindow {
    Q_OBJECT
public:
    WizardsCastleWindow(QWidget *parent = nullptr);
    // Add other public methods as needed
};

#ifdef __cplusplus
extern "C" {
#endif

void initialize_qt(int argc, char *argv[]);
void close_qt(WizardsCastleWindow* window);

// Declare other extern "C" functions here

#ifdef __cplusplus
}
#endif

#endif 
