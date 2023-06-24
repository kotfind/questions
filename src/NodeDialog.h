#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

class Node;

class NodeDialog : public QDialog {
    Q_OBJECT

    public:
        NodeDialog(Node* node, QWidget* parent = nullptr);

    private:
        void createUI();

        Node* node;

        QLineEdit* textEdit;
        QLineEdit* imageUrlEdit;
        QCheckBox* initialCheck;
        QPushButton* cancelButton;
        QPushButton* okButton;

    private slots:
        void onAccepted();
};
