#ifndef PLAYER_H
#define PLAYER_H

#include <QColor>
#include <QObject>
#include "oxygine-flow.h"
#include "game/smartpointers.h"

class Player : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Player(quint32 id);




    void setPlayerID(const quint32 &value);

signals:

public slots:
    QColor getColor() const;
    void setColor(QColor Color);
    quint32 getPlayerID() const;
    QString getArmy();
private:
    quint32 playerID;
    QColor m_Color;
};

#endif // PLAYER_H
