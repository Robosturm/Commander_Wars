#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include <QObject>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

class CO;
class GameMap;
class PlayerInfo;
using spPlayerInfo = std::shared_ptr<PlayerInfo>;

/**
 * @brief Shows infos about the co's, funds and the powermeter
 */
class PlayerInfo final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit PlayerInfo(GameMap* pMap);
   virtual ~PlayerInfo() = default;

    Q_INVOKABLE GameMap *getMap() const;
    Q_INVOKABLE void updateData();
    Q_INVOKABLE void setFlippedX(bool value);
    Q_INVOKABLE bool getFlippedX() const;
private:
    void showTurnStartInfo(qint32 & yPos);
private:
    QVector<oxygine::spSprite> m_playerBackground;
    bool m_flippedX{false};
    GameMap* m_pMap{nullptr};
};

#endif // PLAYERINFO_H
