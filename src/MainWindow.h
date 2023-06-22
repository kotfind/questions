#pragma once

#include <QMainWindow>
#include <QGraphicsView>

class Scene;

class MainWindow : public QMainWindow {
    public:
        MainWindow(QWidget* parent = nullptr);

    private:
        void createUI();

        Scene* scene;
        QGraphicsView* view;
};
