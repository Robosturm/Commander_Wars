#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include <QObject>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class CO;
class GameMap;
class PlayerInfo;
using spPlayerInfo = oxygine::intrusive_ptr<PlayerInfo>;

/**
 * @brief Shows infos about the co's, funds and the powermeter
 */
class PlayerInfo : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit PlayerInfo(GameMap* pMap);
    virtual ~PlayerInfo() = default;

public slots:
    GameMap *getMap() const;
    void updateData();
    void setFlippedX(bool value);
    bool getFlippedX() const;
private:
    QVector<oxygine::spSprite> m_playerBackground;
    bool m_flippedX{false};
    GameMap* m_pMap{nullptr};
};

#endif // PLAYERINFO_H
