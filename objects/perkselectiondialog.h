#ifndef PERKSELECTIONDIALOG_H
#define PERKSELECTIONDIALOG_H

#include <QObject>
#include "game/player.h"

#include "objects/panel.h"
#include "objects/perkselection.h"

#include "oxygine-framework.h"

class PerkSelectionDialog;
typedef oxygine::intrusive_ptr<PerkSelectionDialog> spPerkSelectionDialog;

class PerkSelectionDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit PerkSelectionDialog(Player* pPlayer, qint32 maxPerkcount);
protected slots:
    void changeCO(qint32 index);
signals:
    void sigFinished();
private:
    oxygine::spButton m_OkButton;
    Player* m_pPlayer{nullptr};
    spPanel m_pPanel;
    spPerkSelection m_pPerkSelection;
};

#endif // PERKSELECTIONDIALOG_H
