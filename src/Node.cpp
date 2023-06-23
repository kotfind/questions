#include "Node.h"

#include "Arrow.h"

#include <QPainter>

Node::Node()
{
    setFlags(
        flags() |
        GraphicsItemFlag::ItemIsMovable |
        GraphicsItemFlag::ItemSendsGeometryChanges
    );
}

void Node::paint(
    QPainter* qp,
    const QStyleOptionGraphicsItem* opt,
    QWidget* widget
)
{
    QGraphicsTextItem::paint(qp, opt, widget);
    qp->drawRect(boundingRect());
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant& value) {
    if (change != ItemPositionChange) return QGraphicsItem::itemChange(change, value);

    for (auto* arrow : arrows) {
        arrow->prepareGeometryChange();
    }

    return value;
}
