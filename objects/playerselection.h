#ifndef PLAYERSELECTION_H
#define PLAYERSELECTION_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/panel.h"

#include "objects/spinbox.h"

#include "objects/dropdownmenu.h"
#include "objects/dropdownmenucolor.h"

#include "network/NetworkInterface.h"

class PlayerSelection;
typedef oxygine::intrusive_ptr<PlayerSelection> spPlayerSelection;

class PlayerSelection : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit PlayerSelection(qint32 width, qint32 heigth);
    virtual ~PlayerSelection() = default;

    void showPlayerSelection();
    void resetPlayerSelection();
    qint32 getPlayerAI(qint32 player);
signals:
    void buttonAllCOsRandom();
    void sigShowSelectCO(qint32 player, quint8 co);
    void buttonShowAllBuildList();
    void buttonShowPlayerBuildList(qint32 player);
    void sigAiChanged(qint32 player);
public slots:
    // slots for changing player data
    void allPlayerIncomeChanged(float value);
    void playerIncomeChanged(float value, qint32 playerIdx);
    void allPlayerStartFondsChanged(float value);
    void playerStartFondsChanged(float value, qint32 playerIdx);
    void playerTeamChanged(qint32 value, qint32 playerIdx);
    void playerColorChanged(QColor value, qint32 playerIdx);
    void showSelectCO(qint32 player, quint8 co);
    void playerCO1Changed(QString coid, qint32 playerIdx);
    void playerCO2Changed(QString coid, qint32 playerIdx);
    void playerCOCanceled();
    void slotAllCOsRandom();
    void slotShowAllBuildList();
    void slotShowPlayerBuildList(qint32 player);
    void slotChangeAllBuildList(qint32, QStringList buildList);
    void slotChangePlayerBuildList(qint32 player, QStringList buildList);
    void selectAI(qint32 player);
private:
    // player selection
    spPanel m_pPlayerSelection;

    QVector<oxygine::spSprite> m_playerCO1;
    QVector<oxygine::spSprite> m_playerCO2;
    QVector<spDropDownmenuColor> m_playerColors;
    QVector<spSpinBox> m_playerIncomes;
    QVector<spSpinBox> m_playerStartFonds;
    QVector<spDropDownmenu> m_playerAIs;
    QVector<spDropDownmenu> m_playerTeams;
    QVector<oxygine::spButton> m_playerBuildlist;

    spNetworkInterface m_pNetworkInterface{nullptr};
};

#endif // PLAYERSELECTION_H
