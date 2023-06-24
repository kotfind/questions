#pragma once

#include <QMainWindow>
#include <QGraphicsView>
#include <QString>
#include <QCloseEvent>

class Scene;
enum class EditMode;

class MainWindow : public QMainWindow {
    public:
        MainWindow(QWidget* parent = nullptr);

    private:
        void createUI();
        void createMenuBar();
        void createToolBar();

        bool askForSave();
        void updateTitle();

        Scene* scene;
        QGraphicsView* view;

        QString openedFileName = "";

    protected:
        void closeEvent(QCloseEvent*) override;

    private slots:
        void setEditMode(EditMode);

        bool save();
        bool saveAs();
        bool open();
        bool new_();
};
