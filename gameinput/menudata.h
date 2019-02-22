#ifndef MENUDATA_H
#define MENUDATA_H

#include "oxygine-framework.h"

#include <QObject>

#include <QStringList>

#include <QVector>

class MenuData : public QObject
{
    Q_OBJECT
public:
    explicit MenuData();

    QStringList getTexts()
    {
        return texts;
    }
    QStringList getActionIDs()
    {
        return actionIDs;
    }
    QVector<qint32> getCostList()
    {
        return costList;
    }
    QVector<bool> getEnabledList()
    {
        return enabledList;
    }
    QVector<oxygine::spActor> getIconList()
    {
        return iconList;
    }
    oxygine::spSprite getIconSprite(QString icon);
signals:

public slots:
    void addData(QString text, QString actionID, QString icon, qint32 costs = 0, bool enabled = true);
private:
    QStringList texts;
    QStringList actionIDs;
    QVector<qint32> costList;
    QVector<oxygine::spActor> iconList;
    QVector<bool> enabledList;
};

#endif // MENUDATA_H
