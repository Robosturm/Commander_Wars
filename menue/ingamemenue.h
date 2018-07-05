#ifndef INGAMEMENUE_H
#define INGAMEMENUE_H

#include <QObject>

#include "oxygine-framework.h"

class InGameMenue : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit InGameMenue(qint32 width = 20, qint32 heigth = 20);
    virtual ~InGameMenue();

signals:
    sigMouseWheel(qint32 direction);
public slots:
    void mouseWheel(qint32 direction);
};

#endif // INGAMEMENUE_H
