#ifndef COSTYLEMENU_H
#define COSTYLEMENU_H
#pragma once


#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include <QObject>

class COStyleMenu : public QObject, public oxygine::Actor
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
