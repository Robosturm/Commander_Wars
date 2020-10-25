#ifndef ACHIEVEMENTMENU_H
#define ACHIEVEMENTMENU_H

#include "oxygine-framework.h"
#include <QObject>

#include "objects/panel.h"

class Achievementmenu : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Achievementmenu();

signals:
    void sigExitMenue();
public slots:
    void exitMenue();
private:
    spPanel m_MainPanel;
};

#endif // ACHIEVEMENTMENU_H
