#include <QApplication>

#include "keyboard.h"
#include "main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    Keyboard::instance()->init();

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
