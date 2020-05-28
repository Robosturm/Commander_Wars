#ifndef PERKSELECTION_H
#define PERKSELECTION_H

#include <QObject>

#include "oxygine-framework.h"
#include "game/co.h"

class PerkSelection;
typedef oxygine::intrusive_ptr<PerkSelection> spPerkSelection;

class PerkSelection : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit PerkSelection(CO* pCO, qint32 width);

    void updatePerksView(CO* pCO);
signals:
private:
    CO* m_pCO{nullptr};
};

#endif // PERKSELECTION_H
