#include "Scene.h"

#include "Node.h"
#include "Arrow.h"
#include "EditMode.h"

#include <QGraphicsEllipseItem>
#include <QTransform>
#include <QBrush>

Scene::Scene(QObject* parent)
  : QGraphicsScene(parent)
{}

void Scene::setEditMode(EditMode m)
{
    mode = m;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    auto pos = e->scenePos();

    switch (mode) {
        case EditMode::MOVE:
            QGraphicsScene::mousePressEvent(e);
            break;

        case EditMode::DELETE: {
            auto* item = itemAt(pos, QTransform());
            if (auto* node = dynamic_cast<Node*>(item)) {
                node->remove();
            }
            if (auto* arrow = dynamic_cast<Arrow*>(item)) {
                arrow->remove();
            }
        } break;

        case EditMode::NEW_NODE: {
            auto* n = new Node;
            n->setPos(pos);
            n->showDialog();
            addItem(n);
        } break;

        case EditMode::NEW_ARROW:
            pressedNode = dynamic_cast<Node*>(itemAt(pos, QTransform()));
            if (!pressedNode) break;
            previewLine = new QGraphicsLineItem(QLineF(pos, pos));
            addItem(previewLine);
            break;
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
    auto pos = e->scenePos();

    switch (mode) {
        case EditMode::MOVE:
            QGraphicsScene::mouseMoveEvent(e);
            break;

        case EditMode::DELETE:
        case EditMode::NEW_NODE:
            break;

        case EditMode::NEW_ARROW: {
            if (!previewLine) break;
            auto line = previewLine->line();
            line.setP2(pos);
            previewLine->setLine(line);
        } break;
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
    auto pos = e->scenePos();

    switch (mode) {
        case EditMode::MOVE:
            QGraphicsScene::mouseReleaseEvent(e);
            break;

        case EditMode::DELETE:
        case EditMode::NEW_NODE:
            break;

        case EditMode::NEW_ARROW: {
            if (!previewLine) break;

            removeItem(previewLine);
            delete previewLine;
            previewLine = nullptr;

            auto* node = dynamic_cast<Node*>(itemAt(pos, QTransform()));
            if (!node) break;
            
            auto* arrow = new Arrow(pressedNode, node);
            arrow->showDialog();
            addItem(arrow);
        } break;
    }
}
