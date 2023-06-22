#pragma once

#include <QGraphicsTextItem>
#include <QString>

class Node : public QGraphicsTextItem {
    public:
        Node();

        void paint(
            QPainter* qp,
            const QStyleOptionGraphicsItem*,
            QWidget*
        ) override;

    private:
        bool isInitial = false;
        QString imageUrl = "";
};
