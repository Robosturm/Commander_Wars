#ifndef BOX9OBJECT_H
#define BOX9OBJECT_H

#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include <QObject>

class Player;
class Box9Object;
using spBox9Object = std::shared_ptr<Box9Object>;

class Box9Object : public QObject, public oxygine::Box9Sprite
{
    Q_OBJECT
public:
    Box9Object();
    Q_INVOKABLE void setColorTableFromPlayer(Player* pPlayer, bool matrix);
    Q_INVOKABLE void setIsVisible(bool vis);
    Q_INVOKABLE void setPosX(qint32 x);
    Q_INVOKABLE void setPosY(qint32 y);
};

Q_DECLARE_INTERFACE(Box9Object, "Box9Object");

#endif // BOX9OBJECT_H
