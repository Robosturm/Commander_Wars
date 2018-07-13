#ifndef UNIT_H
#define UNIT_H

#include <QObject>
#include "oxygine-framework.h"


class Unit : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Unit();

signals:

public slots:
};

#endif // UNIT_H
