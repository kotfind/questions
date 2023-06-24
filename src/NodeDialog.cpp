#include "NodeDialog.h"

#include "Node.h"
#include "Scene.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

NodeDialog::NodeDialog(Node* node, QWidget* parent)
  : QDialog(parent),
    node(node)
{
    createUI();

    connect(
        this,
        &NodeDialog::accepted,
        this,
        &NodeDialog::onAccepted
    );
}

void NodeDialog::createUI()
{
    auto* vbox = new QVBoxLayout(this);

    vbox->addWidget(new QLabel(tr("Text:"), this));
    vbox->addWidget(
        textEdit = new QLineEdit(node->toPlainText(), this)
    );

    vbox->addWidget(new QLabel(tr("Image Url:"), this));
    vbox->addWidget(
        imageUrlEdit = new QLineEdit(node->imageUrl, this)
    );

    vbox->addWidget(
        initialCheck = new QCheckBox(tr("Is Initial"), this)
    );
    initialCheck->setChecked(node->isInitial);

    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    hbox->addStretch(1);

    hbox->addWidget(
        cancelButton = new QPushButton(tr("Cancel"), this)
    );
    connect(
        cancelButton,
        &QPushButton::clicked,
        [this]() {
            reject();
        }
    );

    hbox->addWidget(
        okButton = new QPushButton(tr("Ok"), this)
    );
    okButton->setDefault(true);
    connect(
        okButton,
        &QPushButton::clicked,
        [this]() {
            accept();
        }
    );
}

void NodeDialog::onAccepted()
{
    node->setPlainText(textEdit->text());
    node->imageUrl = imageUrlEdit->text();
    if (initialCheck->isChecked()) {
        Scene::instance->setInitial(node);
    }
    Scene::instance->hasChanged = true;
}
