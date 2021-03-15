#ifndef CAMPAIGNMENU_H
#define CAMPAIGNMENU_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "game/campaign.h"

#include "objects/minimap.h"
#include "objects/mapselection.h"
#include "objects/mapselectionview.h"
#include "objects/base/panel.h"

class CampaignMenu : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit CampaignMenu(spCampaign campaign, bool multiplayer);

signals:
    void sigExitMenue();
    void buttonNext();
    void sigShowSaveCampaign();
public slots:
    // slots for changing the map
    void mapSelectionItemClicked(QString item);
    void mapSelectionItemChanged(QString item);
    void exitMenue();

    // general slots
    void slotButtonNext();
    void showSaveCampaign();
    void saveCampaign(QString file);
private:
    spMapSelectionView m_pMapSelectionView;
    // buttons
    oxygine::spButton m_pButtonNext;
    oxygine::spButton m_pButtonSave;

    bool m_Multiplayer{false};
};

#endif // CAMPAIGNMENU_H
