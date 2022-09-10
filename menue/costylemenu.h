#pragma once

#include "menue/basemenu.h"

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
public slots:
    void exitMenue();
    void selectedCOIDChanged(QString coid);
    void editCOStyle();
    void reloadMenue();
protected slots:
    virtual void onEnter() override;
private:
    QString m_currentCOID;
};
