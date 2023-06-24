#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>

enum class EditMode;
class Node;

class Scene : public QGraphicsScene {
    public:
        Scene(QObject* parent = nullptr);

        void setEditMode(EditMode);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*);
        void mouseMoveEvent(QGraphicsSceneMouseEvent*);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*);

    private:
        EditMode mode;

        Node* pressedNode = nullptr;
        QGraphicsLineItem* previewLine = nullptr;
};
