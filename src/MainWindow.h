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
        void createToolBar();

        Scene* scene;
        QGraphicsView* view;

    private slots:
        void setEditMode(EditMode);
};
