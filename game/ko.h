#ifndef KO_H
#define KO_H

#include <QObject>

#include "oxygine-framework.h"

class KO : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit KO();

signals:

public slots:
};

#endif // KO_H
