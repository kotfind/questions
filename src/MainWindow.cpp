#include "MainWindow.h"

#include "Scene.h"
#include "EditMode.h"

#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
    createUI();
    createMenuBar();
    createToolBar();
    updateTitle();
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

    auto* newAction = new QAction(tr("New"), this);
    newAction->setShortcut(Qt::CTRL | Qt::Key_N);
    fileMenu->addAction(newAction);
    connect(
        newAction,
        &QAction::triggered,
        this,
        &MainWindow::new_
    );

    auto* saveAction = new QAction(tr("Save"), this);
    saveAction->setShortcut(Qt::CTRL | Qt::Key_S);
    fileMenu->addAction(saveAction);
    connect(
        saveAction,
        &QAction::triggered,
        this,
        &MainWindow::save
    );

    auto* saveAsAction = new QAction(tr("Save As"), this);
    saveAsAction->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_S);
    fileMenu->addAction(saveAsAction);
    connect(
        saveAsAction,
        &QAction::triggered,
        this,
        &MainWindow::saveAs
    );

    auto* openAction = new QAction(tr("Open"), this);
    openAction->setShortcut(Qt::CTRL | Qt::Key_O);
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
        {":move.svg", EditMode::MOVE},
        {":remove.svg", EditMode::DELETE},
        {":new_node.svg", EditMode::NEW_NODE},
        {":new_arrow.svg", EditMode::NEW_ARROW},
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

bool MainWindow::askForSave()
{
    if (!scene->hasChanged) return true;

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Save changes?"));
    msgBox.setText(tr(
        "You have unsaved changes."
        "Would you like to save them?"
    ));
    msgBox.setStandardButtons(
        QMessageBox::Save |
        QMessageBox::Discard |
        QMessageBox::Cancel
    );
    msgBox.setIcon(QMessageBox::Question);

    switch (msgBox.exec()) {
        case QMessageBox::Save:
            return save();

        case QMessageBox::Discard:
            return true;

        case QMessageBox::Cancel:
            return false;
    }
}

bool MainWindow::save()
{
    if (openedFileName.isEmpty()) {
        return saveAs();
    }

    bool code = scene->save(openedFileName);
    updateTitle();
    return code;
}

bool MainWindow::saveAs()
{
    auto fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save file"),
        "",
        tr("Questions file (*.json)")
    );

    if (fileName.isEmpty()) return false;

    if (!fileName.endsWith(".json")) {
        fileName += ".json";
    }

    if (scene->save(fileName)) {
        openedFileName = fileName;
        updateTitle();
        return true;
    } else {
        return false;
    }
}

bool MainWindow::open()
{
    if (!askForSave()) return false;

    auto fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open file"),
        "",
        tr("Questions file (*.json)")
    );

    if (fileName.isEmpty()) return false;

    scene->clear();

    if (scene->load(fileName)) {
        openedFileName = fileName;
        updateTitle();
        return true;
    } else {
        openedFileName = "";
        return false;
    }
}

bool MainWindow::new_()
{
    if (!askForSave()) return false;

    scene->clear();
    openedFileName = "";
    updateTitle();
    return true;
}

void MainWindow::updateTitle()
{
    setWindowTitle(openedFileName.isEmpty()
        ? tr("[Untitled]")
        : QFileInfo(openedFileName).fileName()
    );
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    if (!askForSave()) {
        e->ignore();
        return;
    }

    scene->clear();
    e->accept();
}
