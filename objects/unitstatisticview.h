#ifndef UNITSTATISTICVIEW_H
#define UNITSTATISTICVIEW_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class Player;

class UnitStatisticView;
using spUnitStatisticView = oxygine::intrusive_ptr<UnitStatisticView>;

class UnitStatisticView : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit UnitStatisticView(QString headline, QMap<QString, qint32> view, qint32 width, qint32 heigth, Player* pPlayer);

signals:
    void sigShowLink(QString pageID);
public slots:
    void showLink(QString pageID);

};

#endif // UNITSTATISTICVIEW_H
