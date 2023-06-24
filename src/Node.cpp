#include "Node.h"

#include "Arrow.h"
#include "NodeDialog.h"
#include "getOrThrow.h"

#include <QPainter>
#include <stdexcept>
#include <QGraphicsScene>
#include <QJsonArray>

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
    if (isInitial) {
        qp->drawEllipse(boundingRect());
    } else {
        qp->drawRect(boundingRect());
    }
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change != ItemPositionChange) return QGraphicsItem::itemChange(change, value);

    updateArrows();

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

QDialog::DialogCode Node::showDialog()
{
    auto code = static_cast<QDialog::DialogCode>(
        NodeDialog(this).exec()
    );
    updateArrows();
    return code;
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)
{
    showDialog();
}

void Node::updateArrows() const
{
    for (auto* arrow : arrows) {
        arrow->prepareGeometryChange();
    }
}

void Node::remove()
{
    while (!arrows.isEmpty()) {
        arrows[0]->remove();
    }
    if (scene()) {
        scene()->removeItem(this);
    }
    delete this;
}

QJsonObject Node::toJson(
    const QHash<Node*, int>& nodeToId,
    const QList<Arrow*>& arrowsFromThis
) const
{
    QJsonObject j;
    j["x"] = pos().x();
    j["y"] = pos().y();
    j["text"] = toPlainText();
    j["imageUrl"] = imageUrl;

    QJsonArray jsonArrows;
    for (const auto* arrow : arrowsFromThis) {
        QJsonObject j;
        j["text"] = arrow->text;
        j["to"] = nodeToId[arrow->to];
        jsonArrows << j;
    }

    j["arrows"] = jsonArrows;
    return j;
}

void Node::fromJson(const QJsonObject& j)
{
    setPos(
        getOrThrow(j["x"]).toDouble(),
        getOrThrow(j["y"]).toDouble()
    );
    setPlainText(getOrThrow(j["text"]).toString());
    imageUrl = getOrThrow(j["imageUrl"]).toString();
}
