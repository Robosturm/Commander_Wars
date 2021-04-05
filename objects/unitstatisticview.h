#ifndef UNITSTATISTICVIEW_H
#define UNITSTATISTICVIEW_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "game/gamerecording/gamerecorder.h"

#include "objects/base/panel.h"

class Player;

class UnitStatisticView;
using spUnitStatisticView = oxygine::intrusive_ptr<UnitStatisticView>;

class UnitStatisticView : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit UnitStatisticView(const GameRecorder::PlayerData & data, qint32 width, qint32 heigth, Player* pPlayer);
signals:
    void sigShowLink(QString pageID);
public slots:
    void showLink(QString pageID);
private:
    /**
     * @brief addStatistic
     * @param pPanel
     * @param headline
     * @param view
     * @param pPlayer
     * @param x
     * @param y
     * @param width
     */
    void addStatistic(spPanel & pPanel, QString headline, const QMap<QString, qint32> & view, Player* pPlayer, qint32 & y, qint32 width);

};

#endif // UNITSTATISTICVIEW_H
