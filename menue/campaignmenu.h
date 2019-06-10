#ifndef CAMPAIGNMENU_H
#define CAMPAIGNMENU_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/mapselection.h"

#include "game/campaign.h"

#include "objects/minimap.h"

#include "objects/mapselectionview.h"

#include "objects/panel.h"

class CampaignMenu : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit CampaignMenu(spCampaign campaign, bool multiplayer);

signals:
    void sigExitMenue();
    void buttonNext();
public slots:
    // slots for changing the map
    void mapSelectionItemClicked(QString item);
    void mapSelectionItemChanged(QString item);
    void exitMenue();

    // general slots
    void slotButtonNext();
private:
    spMapSelectionView m_pMapSelectionView;
    // buttons
    oxygine::spButton m_pButtonNext;

    bool m_Multiplayer{false};
};

#endif // CAMPAIGNMENU_H
