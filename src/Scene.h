#pragma once

#include <QGraphicsScene>

enum class EditMode;

class Scene : public QGraphicsScene {
    public:
        Scene(QObject* parent = nullptr);

        void setEditMode(EditMode);
};
