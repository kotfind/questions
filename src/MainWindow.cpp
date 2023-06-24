#include "MainWindow.h"

#include "Scene.h"
#include "EditMode.h"

#include <QToolBar>
#include <QAction>
#include <QActionGroup>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
    createUI();
    createToolBar();
}

void MainWindow::createUI()
{
    view = new QGraphicsView(this);
    setCentralWidget(view);

    scene = new Scene(this);
    view->setScene(scene);

    connect(
        scene,
        &Scene::cursorChanged,
        view,
        &QGraphicsView::setCursor
    );
}

void MainWindow::createToolBar()
{
    QList<QPair<QString, EditMode>> arr = {
        /* Icon Path | EditMode */
        {":new_node.svg", EditMode::NEW_NODE},
        {":new_arrow.svg", EditMode::NEW_ARROW},
        {":move.svg", EditMode::MOVE},
        {":remove.svg", EditMode::DELETE},
    };

    auto* bar = addToolBar("Tool Bar");

    auto* group = new QActionGroup(this);
    group->setExclusive(true);

    for (const auto& [iconPath, mode] : arr) {
        auto* action = new QAction(QIcon(iconPath), "", this);
        connect(
            action,
            &QAction::triggered,
            [this, mode]() {
                setEditMode(mode);
            }
        );
        action->setCheckable(true);
        bar->addAction(action);
        group->addAction(action);
    }

    auto action = group->actions().first();
    action->setChecked(true);
    action->trigger();
}

void MainWindow::setEditMode(EditMode m)
{
    scene->setEditMode(m);
}
