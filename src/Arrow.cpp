#include "Arrow.h"

#include "Node.h"

#include <QPainterPathStroker>

Arrow::Arrow(Node* from, Node* to)
  : from(from),
    to(to)
{}

void Arrow::paint(
    QPainter* qp,
    const QStyleOptionGraphicsItem*,
    QWidget*
)
{
    qp->drawPath(getPainterPath());
}

QPainterPath Arrow::shape() const {
    QPainterPathStroker stroker;
    stroker.setWidth(paintWidth);
    return stroker.createStroke(getPainterPath());
}

QRectF Arrow::boundingRect() const {
    return shape().boundingRect();
}

QPainterPath Arrow::getPainterPath() const {
    QPainterPath path;
    path.moveTo(from->pos());
    path.lineTo(to->pos());
    return path;
}
