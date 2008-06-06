#include <QHBoxLayout>

#include "battleground.h"

#include "main_window.h"

MainWindow::MainWindow() : QMainWindow()
{
    setupUi(this);

    resize(800, 600);

    // Playground
    QHBoxLayout *mainLayout = new QHBoxLayout(widgetBattleground);

    mainLayout->addWidget(Battleground::instance());

    QList<int> sizes;
    sizes << 400 << 100;
    splitter->setSizes(sizes);
}
