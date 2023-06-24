#include "MainWindow.h"

#include "Scene.h"
#include "EditMode.h"

#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QMenuBar>
#include <QMenu>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
    createUI();
    createMenuBar();
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

void MainWindow::createMenuBar()
{
    auto* bar = menuBar();

    auto* fileMenu = new QMenu(tr("File"), this);
    bar->addMenu(fileMenu);

    auto* saveAction = new QAction(tr("Save"), this);
    fileMenu->addAction(saveAction);
    connect(
        saveAction,
        &QAction::triggered,
        this,
        &MainWindow::save
    );

    auto* openAction = new QAction(tr("Open"), this);
    fileMenu->addAction(openAction);
    connect(
        openAction,
        &QAction::triggered,
        this,
        &MainWindow::open
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

void MainWindow::save()
{
    scene->save("file.json");
}

void MainWindow::open()
{
    scene->load("file.json");
}
