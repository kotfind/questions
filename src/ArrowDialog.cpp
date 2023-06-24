#include "ArrowDialog.h"

#include "Arrow.h"
#include "Scene.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

ArrowDialog::ArrowDialog(Arrow* arrow, QWidget* parent)
  : QDialog(parent),
    arrow(arrow)
{
    createUI();

    connect(
        this,
        &ArrowDialog::accepted,
        this,
        &ArrowDialog::onAccepted
    );
}

void ArrowDialog::createUI()
{
    auto* vbox = new QVBoxLayout(this);

    vbox->addWidget(new QLabel(tr("Text:"), this));
    vbox->addWidget(
        textEdit = new QLineEdit(arrow->text, this)
    );

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

void ArrowDialog::onAccepted()
{
    arrow->text = textEdit->text();
    Scene::instance->hasChanged = true;
}
