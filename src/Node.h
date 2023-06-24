#pragma once

#include <QGraphicsTextItem>
#include <QString>
#include <QList>
#include <QLineF>
#include <QGraphicsSceneMouseEvent>
#include <QDialog>
#include <QJsonObject>

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

        QPointF centerPos() const;

        QPointF intersect(const QLineF& l) const;

        QDialog::DialogCode showDialog();

        void remove();

        QJsonObject toJson(
            const QHash<Node*, int>& nodeToId,
            const QList<Arrow*>& arrowsFromThis
        ) const;

        bool isInitial = false;
        QString imageUrl = "";

    protected:
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*);


    private:
        void updateArrows() const;

        QList<Arrow*> arrows;

    friend Arrow;
};
