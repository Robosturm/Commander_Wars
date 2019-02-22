#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include <QObject>
#include <QVector>
#include "oxygine-framework.h"

class PlayerInfo : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit PlayerInfo();

signals:

public slots:
    void updateData();
private:
    QVector<oxygine::spSprite> playerBackground;
};

#endif // PLAYERINFO_H
