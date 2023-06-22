#include "Scene.h"

#include "Node.h"

#include <QGraphicsEllipseItem>
#include <QBrush>

Scene::Scene(QObject* parent)
  : QGraphicsScene(parent)
{
    for (int i = 0; i < 5; ++i) {
        auto* n = new Node;
        n->setPlainText(QString("Node %1").arg(i + 1));
        n->setPos(0, i * 100);
        addItem(n);
    }

    update();
}
