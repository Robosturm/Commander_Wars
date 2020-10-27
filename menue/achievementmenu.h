#ifndef ACHIEVEMENTMENU_H
#define ACHIEVEMENTMENU_H

#include "oxygine-framework.h"
#include <QObject>

#include "objects/panel.h"
#include "objects/textbox.h"

class Achievementmenu : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Achievementmenu();

signals:
    void sigExitMenue();
    void sigSearch();
public slots:
    void exitMenue();
    void search();
    void searchChanged(QString);
private:
    spPanel m_MainPanel;
    spTextbox m_SearchString;
};

#endif // ACHIEVEMENTMENU_H
