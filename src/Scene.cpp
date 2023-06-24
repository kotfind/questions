#include "Scene.h"

#include "Node.h"
#include "Arrow.h"
#include "EditMode.h"
#include "getOrThrow.h"

#include <QGraphicsEllipseItem>
#include <QTransform>
#include <QBrush>
#include <QHash>
#include <QList>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <stdexcept>
#include <QDebug>

Scene* Scene::instance = nullptr;

Scene::Scene(QObject* parent)
  : QGraphicsScene(parent)
{
    instance = this;
}

void Scene::setEditMode(EditMode m)
{
    mode = m;
}

void Scene::setInitial(Node* n)
{
    for (auto* item : items()) {
        if (auto* node = dynamic_cast<Node*>(item)) {
            node->isInitial = node == n;
            node->update();
        }
    }
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
                hasChanged = true;
            }
            if (auto* arrow = dynamic_cast<Arrow*>(item)) {
                arrow->remove();
                hasChanged = true;
            }
        } break;

        case EditMode::NEW_NODE: {
            auto* n = new Node;
            if (n->showDialog() == QDialog::Accepted) {
                n->setPos(pos);
                addItem(n);
                hasChanged = true;
            } else {
                n->remove();
            }
        } break;

        case EditMode::NEW_ARROW:
            pressedNode = dynamic_cast<Node*>(itemAt(pos, QTransform()));
            if (!pressedNode) break;
            previewLine = new QGraphicsLineItem(QLineF(pos, pos));
            addItem(previewLine);
            break;
    }

    updateCursor(e);
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

    updateCursor(e);
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
            if (!node || node == pressedNode) break;
            
            auto* arrow = new Arrow(pressedNode, node);
            if (arrow->showDialog() == QDialog::Accepted) {
                addItem(arrow);
                hasChanged = true;
            } else {
                arrow->remove();
            }
        } break;
    }

    updateCursor(e);
}

void Scene::updateCursor(QGraphicsSceneMouseEvent* e)
{
    auto pos = e->scenePos();
    auto* item = itemAt(pos, QTransform());
    emit cursorChanged(
        item
            ? Qt::PointingHandCursor
            : Qt::ArrowCursor
    );
}

QJsonObject Scene::toJson() const
{
    QList<Node*> nodes;
    QList<Arrow*> arrows;

    for (auto* item : items()) {
        if (auto* n = dynamic_cast<Node*>(item)) {
            nodes << n;
        }
        if (auto* a = dynamic_cast<Arrow*>(item)) {
            arrows << a;
        }
    }

    int initial = -1;
    QHash<Node*, int> nodeToId;
    for (int i = 0; i < nodes.size(); ++i) {
        nodeToId[nodes[i]] = i;
        if (nodes[i]->isInitial) {
            initial = i;
        }
    }

    if (initial == -1) {
        QMessageBox::warning(
            nullptr,
            tr("No initial node!"),
            tr("Initial node was not set!")
        );
    }

    QHash<Node*, QList<Arrow*>> arrowsFrom;
    for (auto* arrow : arrows) {
        arrowsFrom[arrow->from] << arrow;
    }

    QJsonObject json;
    json["initial"] = initial;

    QJsonArray jsonNodes;
    for (auto* node : nodes) {
        jsonNodes << node->toJson(nodeToId, arrowsFrom[node]);
    }

    json["nodes"] = jsonNodes;

    return json;
}

bool Scene::save(const QString& fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(
            nullptr,
            tr("Couldn't open file"),
            tr("Couldn't open file \"%1\" for writing.").arg(fileName)
        );
        return false;
    }

    file.write(QJsonDocument(toJson()).toJson());
    hasChanged = false;
    return true;
}

void Scene::fromJson(const QJsonObject& j)
{
    int initial = getOrThrow(j["initial"]).toInt();
    const auto& jsonNodes = getOrThrow(j["nodes"]).toArray();

    QList<Node*> nodes(jsonNodes.size());
    for (int i = 0; i < jsonNodes.size(); ++i) {
        auto* n = new Node;
        n->fromJson(jsonNodes[i].toObject());
        n->isInitial = i == initial;
        nodes[i] = n;
    }

    QList<Arrow*> arrows;
    for (int i = 0; i < jsonNodes.size(); ++i) {
        const auto& jsonArrows = getOrThrow(jsonNodes[i]["arrows"]).toArray();
        for (int j = 0; j < jsonArrows.size(); ++j) {
            const auto& jsonArrow = jsonArrows[j].toObject();
            auto* arrow = new Arrow(
                nodes[i],
                nodes[getOrThrow(jsonArrow["to"]).toInt()]
            );
            arrow->text = getOrThrow(jsonArrow["text"]).toString();
            arrows << arrow;
        }
    }

    for (auto* node : nodes) {
        addItem(node);
    }

    for (auto* arrow : arrows) {
        addItem(arrow);
    }
}

bool Scene::load(const QString& fileName)
{
    clear();

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(
            nullptr,
            tr("Couldn't open file"),
            tr("Couldn't open file \"%1\" for reading.").arg(fileName)
        );
        return false;
    }

    try {
        fromJson(QJsonDocument::fromJson(file.readAll()).object());
        hasChanged = false;
        update();
        return true;
    } catch(const std::runtime_error&) {
        QMessageBox::critical(
            nullptr,
            tr("Couldn't parse"),
            tr("Couldn't parse file \"%1\".").arg(fileName)
        );
        update();
        return false;
    }
}

void Scene::clear()
{
    while (!items().isEmpty()) {
        auto item = items()[0];
        if (auto* node = dynamic_cast<Node*>(item)) {
            node->remove();
        }
        if (auto* arrow = dynamic_cast<Arrow*>(item)) {
            arrow->remove();
        }
    }
    hasChanged = false;
}
