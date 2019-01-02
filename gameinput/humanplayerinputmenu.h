#ifndef HUMANPLAYERINPUTMENU_H
#define HUMANPLAYERINPUTMENU_H

#include <QObject>

#include <QVector>

#include <QStringList>

#include "oxygine-framework.h"


class HumanPlayerInputMenu;
typedef oxygine::intrusive_ptr<HumanPlayerInputMenu> spHumanPlayerInputMenu;

class HumanPlayerInputMenu : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit HumanPlayerInputMenu(QStringList texts, QStringList actionIDs, QVector<oxygine::spSprite> icons);

signals:
    void sigItemSelected(QString actionID);
public slots:

private:
    oxygine::spSprite m_Cursor;
};

#endif // HUMANPLAYERINPUTMENU_H
