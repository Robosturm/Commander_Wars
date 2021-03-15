#ifndef DIALOGMODIFYBUILDING_H
#define DIALOGMODIFYBUILDING_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "objects/base/panel.h"

#include "game/building.h"

class DialogModifyBuilding;
typedef oxygine::intrusive_ptr<DialogModifyBuilding> spDialogModifyBuilding;

class DialogModifyBuilding : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogModifyBuilding(Building* pBuilding);

signals:
    void sigFinished();
private:
    spPanel m_pPanel;
    Building* m_pBuilding{nullptr};
    oxygine::spButton m_OkButton;
};

#endif // DIALOGMODIFYBUILDING_H
