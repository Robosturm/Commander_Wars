#pragma once

#include "menue/basemenu.h"
#include "game/player.h"
#include "objects/coselection.h"

class COStyleMenu;
using spCOStyleMenu = oxygine::intrusive_ptr<COStyleMenu>;

class COStyleMenu final : public Basemenu
{
    Q_OBJECT
public:
    explicit COStyleMenu();
    ~COStyleMenu() = default;
signals:
    void sigExitMenue();
    void sigEditCOStyle();
    void sigShowCOInfo();
public slots:
    void exitMenue();
    void selectedCOIDChanged(QString coid);
    void editCOStyle();
    void reloadMenue();
    void showCOInfo();
protected slots:
    virtual void onEnter() override;
private:
    QString m_currentCOID;
    spPlayer m_pPlayer;
    spCOSelection m_COSelection;
};
