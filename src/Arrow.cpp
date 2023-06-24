#include "Arrow.h"

#include "Node.h"
#include "ArrowDialog.h"

#include <QLineF>
#include <QPainterPathStroker>
#include <QApplication>

Arrow::Arrow(Node* from, Node* to)
  : from(from),
    to(to)
{
    from->arrows.append(this);
    to->arrows.append(this);
}

void Arrow::paint(
    QPainter* qp,
    const QStyleOptionGraphicsItem*,
    QWidget*
)
{
    qp->drawPath(getPainterPath());
    qp->drawText(
        (from->centerPos() + to->centerPos()) / 2,
        text
    );
}

QPainterPath Arrow::shape() const
{
    QPainterPathStroker stroker;
    stroker.setWidth(paintWidth);
    return stroker.createStroke(getPainterPath());
}

QRectF Arrow::boundingRect() const
{
    return shape().boundingRect();
}

QPainterPath Arrow::getPainterPath() const
{
    if (from->collidesWithItem(to)) return QPainterPath();

    QLineF line(
        from->centerPos(),
        to->centerPos()
    );

    auto p1 = from->intersect(line);
    auto p2 =   to->intersect(line);

    auto l = hypot(p1.x() - p2.x(), p1.y() - p2.y());
    auto dir_ = line.unitVector();
    auto norm_ = dir_.normalVector();

    QPointF dir(dir_.dx(), dir_.dy());
    QPointF norm(norm_.dx(), norm_.dy());

    auto base = p1 + dir * (l - arrowHeadLength);
    auto d = norm * arrowHeadWidth / 2;
    auto p3 = base + d;
    auto p4 = base - d;

    QPainterPath path;
    path.moveTo(p1);
    path.lineTo(p2);
    path.lineTo(p3);
    path.moveTo(p2);
    path.lineTo(p4);
    return path;
}

void Arrow::showDialog()
{
    ArrowDialog(this).exec();
    update();
}

void Arrow::mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)
{
    showDialog();
}
