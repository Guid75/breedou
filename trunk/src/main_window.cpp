#include <QHBoxLayout>

#include "main_window.h"

MainWindow::MainWindow() : QMainWindow()
{
    setupUi(this);

    resize(800, 600);

    // Playground
    QHBoxLayout *mainLayout = new QHBoxLayout(widgetPlayground);

    mainLayout->addWidget(Playground::instance());

    QList<int> sizes;
    sizes << 400 << 100;
    splitter->setSizes(sizes);
}
