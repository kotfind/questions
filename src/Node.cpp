#include "Node.h"

#include <QPainter>

Node::Node()
{
    setFlags(
        flags() |
        GraphicsItemFlag::ItemIsMovable
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
