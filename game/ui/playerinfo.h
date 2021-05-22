#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include <QObject>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class CO;
class PlayerInfo;
typedef oxygine::intrusive_ptr<PlayerInfo> spPlayerInfo;

/**
 * @brief Shows infos about the co's, funds and the powermeter
 */
class PlayerInfo : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit PlayerInfo();

public slots:
    void updateData();
    void setFlippedX(bool value);
    bool getFlippedX() const;
private:
    QVector<oxygine::spSprite> m_playerBackground;
    bool m_flippedX{false};
};

#endif // PLAYERINFO_H
