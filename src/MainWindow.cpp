#include "MainWindow.h"

#include "Scene.h"
#include "EditMode.h"

#include <QToolBar>
#include <QAction>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
    createUI();
    createToolBar();

    scene = new Scene(this);
    view->setScene(scene);
}

void MainWindow::createUI()
{
    view = new QGraphicsView(this);
    setCentralWidget(view);
}

void MainWindow::createToolBar()
{
    auto* bar = addToolBar("Tool Bar");

    QList<QPair<QString, EditMode>> arr = {
        /* Icon Path | EditMode */
        {":new_node.svg", EditMode::NEW_NODE},
        {":new_arrow.svg", EditMode::NEW_ARROW},
        {":move.svg", EditMode::MOVE},
        {":remove.svg", EditMode::DELETE},
    };

    for (const auto& [iconPath, mode] : arr) {
        auto* action = new QAction(QIcon(iconPath), "", this);
        connect(
            action,
            &QAction::triggered,
            [this, mode]() {
                setEditMode(mode);
            }
        );
        bar->addAction(action);
    }
}

void MainWindow::setEditMode(EditMode m)
{
    scene->setEditMode(m);
}
