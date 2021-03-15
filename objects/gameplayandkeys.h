#ifndef GAMEPLAYANDKEYS_H
#define GAMEPLAYANDKEYS_H

#include "qobject.h"


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/panel.h"

class GameplayAndKeys;
typedef oxygine::intrusive_ptr<GameplayAndKeys> spGameplayAndKeys;

class GameplayAndKeys : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    GameplayAndKeys(qint32 heigth);

private:
    spPanel m_pOptions;
};

#endif // GAMEPLAYANDKEYS_H
