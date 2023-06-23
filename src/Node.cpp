#include "Node.h"

#include "Arrow.h"

#include <QPainter>
#include <stdexcept>

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

QPointF Node::centerPos() const
{
    auto r = boundingRect();
    return pos() + QPointF(r.width(), r.height()) / 2;
}

QPointF Node::intersect(const QLineF& l1) const
{ 
    QList<QPointF> pts = {
        mapToScene(boundingRect().bottomLeft()),
        mapToScene(boundingRect().bottomRight()),
        mapToScene(boundingRect().topRight()),
        mapToScene(boundingRect().topLeft()),
    };
    pts.append(pts.front());

    QPointF ans;
    for (int i = 0; i < 4; ++i) {
        QLineF l2(pts[i], pts[i + 1]);
        if (l1.intersects(l2, &ans) == QLineF::BoundedIntersection) {
            return ans;
        }
    }

    throw std::runtime_error("don't intersect");
}
