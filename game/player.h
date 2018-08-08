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
    /**
     * @brief getColor the color of this player
     * @return
     */
    QColor getColor() const;
    /**
     * @brief setColor sets the color of this player. Note this want update existing sprites
     * @param Color
     */
    void setColor(QColor Color);
    /**
     * @brief getPlayerID player id of this player from 0 to n
     * @return
     */
    quint32 getPlayerID() const;
    /**
     * @brief getArmy the army string id of this player.
     * @return
     */
    QString getArmy();
private:
    quint32 playerID;
    QColor m_Color;
};

#endif // PLAYER_H
