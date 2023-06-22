#pragma once

#include <QGraphicsItem>
#include <QPainterPath>
#include <QRectF>
#include <QPainter>

class Node;

class Arrow : public QGraphicsItem {
    public:
        Arrow(Node* from, Node* to);

        void paint(
            QPainter* qp,
            const QStyleOptionGraphicsItem*,
            QWidget*
        ) override;

        QPainterPath shape() const override;

        QRectF boundingRect() const override;

    private:
        QPainterPath getPainterPath() const;

        Node* from;
        Node* to;

        static constexpr double paintWidth = 10;
};
