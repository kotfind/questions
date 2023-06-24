#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QJsonObject>

enum class EditMode;
class Node;

class Scene : public QGraphicsScene {
    Q_OBJECT

    public:
        Scene(QObject* parent = nullptr);

        void setEditMode(EditMode);

        void setInitial(Node*);

        void save(const QString& fileName);

        static Scene* instance;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*);
        void mouseMoveEvent(QGraphicsSceneMouseEvent*);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*);

        void updateCursor(QGraphicsSceneMouseEvent*);

    private:
        QJsonObject toJson() const;

        EditMode mode;

        Node* pressedNode = nullptr;
        QGraphicsLineItem* previewLine = nullptr;

    signals:
        void cursorChanged(const QCursor&);
};
