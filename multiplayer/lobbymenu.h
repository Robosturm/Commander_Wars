#ifndef LOBBYMENU_H
#define LOBBYMENU_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/panel.h"

class LobbyMenu : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit LobbyMenu();

signals:    
    void sigExitMenue();
public slots:
    void exitMenue();

private:
    spPanel m_pGamesPanel;
};

#endif // LOBBYMENU_H
