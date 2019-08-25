#ifndef COSTYLEMENU_H
#define COSTYLEMENU_H
#pragma once

#include "oxygine-framework.h"
#include <QObject>

class COStyleMenu : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit COStyleMenu();
signals:
    void sigExitMenue();
public slots:
    void exitMenue();
    void selectedCOIDChanged(QString coid);
private:
    oxygine::spSprite m_pCurrentCO;
    QString m_currentCOID;
};

#endif // COSTYLEMENU_H
