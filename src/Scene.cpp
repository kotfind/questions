#include "Scene.h"

#include "Node.h"
#include "Arrow.h"

#include <QGraphicsEllipseItem>
#include <QBrush>

Scene::Scene(QObject* parent)
  : QGraphicsScene(parent)
{
    constexpr int n = 5;
    Node* nodes[n];
    for (int i = 0; i < 5; ++i) {
        auto* n = nodes[i] = new Node;
        n->setPlainText(QString("Node %1").arg(i + 1));
        n->setPos(0, i * 100);
        addItem(n);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            auto* a = new Arrow(nodes[i], nodes[j]);
            addItem(a);
        }
    }

    update();
}
