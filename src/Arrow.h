#pragma once

#include <QGraphicsItem>
#include <QPainterPath>
#include <QRectF>
#include <QPainter>
#include <QString>

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

        void showDialog();

        QString text = "abacaba";

    protected:
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*);

    private:
        QPainterPath getPainterPath() const;

        Node* from;
        Node* to;

        static constexpr double paintWidth = 10;
        static constexpr double arrowHeadWidth = 10;
        static constexpr double arrowHeadLength = 10;

    friend Node;
};
