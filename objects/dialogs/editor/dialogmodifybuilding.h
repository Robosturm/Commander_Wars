#ifndef DIALOGMODIFYBUILDING_H
#define DIALOGMODIFYBUILDING_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"
#include "objects/base/panel.h"

#include "game/building.h"

class GameMap;
class DialogModifyBuilding;
using spDialogModifyBuilding = oxygine::intrusive_ptr<DialogModifyBuilding>;

class DialogModifyBuilding final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogModifyBuilding(GameMap* pMap, Building* pBuilding);
    ~DialogModifyBuilding() = default;
signals:
    void sigFinished();
public slots:
    void remove();
private:
    spPanel m_pPanel;
    Building* m_pBuilding{nullptr};
    oxygine::spButton m_OkButton;
};

#endif // DIALOGMODIFYBUILDING_H
