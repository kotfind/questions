#pragma once

#include <QMainWindow>
#include <QGraphicsView>

class Scene;
enum class EditMode;

class MainWindow : public QMainWindow {
    public:
        MainWindow(QWidget* parent = nullptr);

    private:
        void createUI();
        void createMenuBar();
        void createToolBar();

        Scene* scene;
        QGraphicsView* view;

    private slots:
        void setEditMode(EditMode);

        void save();
        void open();
};
