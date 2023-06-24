#include "getOrThrow.h"

#include <QJsonValue>
#include <stdexcept>

QJsonValue& getOrThrow(QJsonValue& t) {
    if (t.isUndefined())
        throw std::runtime_error("Couldn't parse");
    return t;
}

const QJsonValue& getOrThrow(const QJsonValue& t) {
    if (t.isUndefined())
        throw std::runtime_error("Couldn't parse");
    return t;
}
