#ifndef COSTYLEMENU_H
#define COSTYLEMENU_H
#pragma once


#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "menue/basemenu.h"

class COStyleMenu;
using spCOStyleMenu = oxygine::intrusive_ptr<COStyleMenu>;

class COStyleMenu : public Basemenu
{
    Q_OBJECT
public:
    explicit COStyleMenu();
signals:
    void sigExitMenue();
    void sigEditCOStyle();
public slots:
    void exitMenue();
    void selectedCOIDChanged(QString coid);
    void editCOStyle();
    void reloadMenue();
private:

    QString m_currentCOID;
};

#endif // COSTYLEMENU_H
