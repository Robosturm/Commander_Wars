#ifndef DIALOGMODIFYUNIT_H
#define DIALOGMODIFYUNIT_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "objects/base/panel.h"

class Unit;
class GameMap;
class Player;
using spPlayer = std::shared_ptr<Player>;
class DialogModifyUnit;
using spDialogModifyUnit = std::shared_ptr<DialogModifyUnit>;

class DialogModifyUnit final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogModifyUnit(GameMap* pMap, Unit* pUnit);
    virtual ~DialogModifyUnit() = default;
signals:
    void sigFinished();
    void sigUpdateData();
    void sigLoadUnit(QString unitID, qint32 index);
    void sigShowEditLoadedUnit(GameMap* pMap, Unit* pUnit);
protected slots:
    void updateData();
    void loadUnit(QString unitID, qint32 index);
    void addLoadLoopPoints(qint32& y, qint32 sliderOffset);
    void remove();
    void showEditLoadedUnit(GameMap* pMap, Unit* pUnit);
private:
    void addLoadUnit(qint32 index, qint32 sliderOffset, qint32& y);
    spPanel m_pPanel;
    Unit* m_pUnit{nullptr};
    oxygine::spButton m_OkButton;
    GameMap* m_pMap{nullptr};
    spPlayer m_dropDownPlayer;
};

#endif // DIALOGMODIFYUNIT_H
