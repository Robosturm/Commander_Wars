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
    /**
     * @brief setMenuPosition changes the position of this menu
     * @param x position in pixel
     * @param y position in pixel
     */
    void setMenuPosition(qint32 x, qint32 y);
signals:
    void sigItemSelected(QString actionID);
public slots:

private:
    oxygine::spSprite m_Cursor;
};

#endif // HUMANPLAYERINPUTMENU_H
