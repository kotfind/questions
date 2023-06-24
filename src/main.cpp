#include "MainWindow.h"

#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    auto font = app.font();
    font.setPointSize(12);
    app.setFont(font);

    MainWindow master;
    master.show();

    return app.exec();
}
