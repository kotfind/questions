#include "MainWindow.h"

#include "Scene.h"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
    createUI();

    scene = new Scene(this);
    view->setScene(scene);
}

void MainWindow::createUI()
{
    view = new QGraphicsView(this);
    setCentralWidget(view);
}
