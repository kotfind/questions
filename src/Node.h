#pragma once

#include <QGraphicsTextItem>
#include <QString>
#include <QList>

class Arrow;

class Node : public QGraphicsTextItem {
    public:
        Node();

        void paint(
            QPainter* qp,
            const QStyleOptionGraphicsItem*,
            QWidget*
        ) override;

        QVariant itemChange(GraphicsItemChange, const QVariant&) override;

    private:
        bool isInitial = false;
        QString imageUrl = "";

        QList<Arrow*> arrows;

    friend Arrow;
};
