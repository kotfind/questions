#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

class Arrow;

class ArrowDialog : public QDialog {
    Q_OBJECT

    public:
        ArrowDialog(Arrow* arrow, QWidget* parent = nullptr);

    private:
        void createUI();

        Arrow* arrow;

        QLineEdit* textEdit;
        QPushButton* cancelButton;
        QPushButton* okButton;

    private slots:
        void onAccepted();
};
