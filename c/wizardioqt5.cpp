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
#include "wizardio.h"

class WizardsCastleWindow : public QMainWindow {
    Q_OBJECT

public:
    WizardsCastleWindow(QWidget *parent = nullptr)
        : QMainWindow(parent), waitingForSpecificInput(false)
    {
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

private slots:
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

private:
    QTextEdit *outputText;
    QLineEdit *inputLine;
    std::string lastInput;
    bool waitingForSpecificInput;
    std::string validInputs;
};

WizardsCastleWindow* g_window = nullptr;

extern "C" {

void initialize_qt(int argc, char *argv[]) {
    static QApplication app(argc, argv);
    g_window = new WizardsCastleWindow();
    g_window->show();
}

const char* get_user_input_main() {
    g_window->clearInput();
    while (g_window->inputIsEmpty()) {
        QCoreApplication::processEvents();
    }
    static std::string input = g_window->getLastInput();
    return input.c_str();
}

int get_user_input_number() {
    const char* input = get_user_input_main();
    return atoi(input);
}

char get_user_input() {
    g_window->setWaitingForSpecificInput(true, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    while (g_window->isWaitingForSpecificInput()) {
        QCoreApplication::processEvents();
    }
    return g_window->getLastInput()[0];
}

char get_user_input_custom_prompt(char* prompt) {
    print_message(prompt);
    return get_user_input();
}

char get_user_input_yn() {
    g_window->setWaitingForSpecificInput(true, "YN");
    while (g_window->isWaitingForSpecificInput()) {
        QCoreApplication::processEvents();
    }
    return g_window->getLastInput()[0];
}

void print_message(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    g_window->appendToOutput(QString::fromUtf8(buffer));
    va_end(args);
}

void print_message_formatted(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    
    QString message = QString::fromUtf8(buffer);
    message = message.toUpper();
    QStringList sentences = message.split(QRegularExpression("(?<=[.!?])\\s+"), Qt::SkipEmptyParts);
    for (QString& sentence : sentences) {
        sentence = sentence.trimmed();
        if (!sentence.isEmpty()) {
            sentence[0] = sentence[0].toUpper();
        }
    }
    message = sentences.join(" ");
    
    g_window->appendToOutput(message);
}

} // end of extern "C"

#include "wizardioqt5.moc"
