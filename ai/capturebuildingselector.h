#pragma once

#include <QtTypes>
#include "ai/coreai.h"

class GameAction;
using spGameAction = std::shared_ptr<GameAction>;
class QmlVectorUnit;
using spQmlVectorUnit = std::shared_ptr<QmlVectorUnit>;
class QmlVectorBuilding;
using spQmlVectorBuilding = std::shared_ptr<QmlVectorBuilding>;


class CaptureBuildingSelector
{
    struct CaptureInfo
    {
        CaptureInfo(qint32 x, qint32 y, qint32 unitIdx, bool farAway)
            : m_x(x),
            m_y(y),
            m_unitIdx(unitIdx),
            m_farAway(farAway)
        {
        }
        qint32 m_x;
        qint32 m_y;
        qint32 m_unitIdx;
        bool m_farAway{false};
    };

    using TargetBuildings = std::vector<CaptureInfo>;
public:
    CaptureBuildingSelector(CoreAI & owner);

    spGameAction getNextCaptureBuilding(std::vector<CoreAI::MoveUnitData> & rOwnUnits, CoreAI::MoveUnitData** targetUnitData);

    void addUsedFarAwayBuildings(QPoint farAwayTarget);
    void resetUsedFarAwayBuildings();
private:
    TargetBuildings getTargetBuildings(std::vector<CoreAI::MoveUnitData> & rOwnUnits, QStringList & highPrioBuildings, spGameAction & pAction, CoreAI::MoveUnitData** targetUnitData);
    spGameAction getPerformingAction(TargetBuildings & captures, Unit *pUnit, CoreAI::MoveUnitData &unitData, qint32 targetIndex);
    bool getNextTarget(TargetBuildings & captureBuildings, TargetBuildings & captures, qint32 & targetIndex);
    qint32 getPrio(CaptureInfo & info);
    TargetBuildings filterCaptures(TargetBuildings & captureBuildings, qint32 unitIdx);
    bool findSingleCaptureBuilding(TargetBuildings & captureBuildings, TargetBuildings & captures, qint32 & targetIndex);
private:
    CoreAI & m_owner;
    QVector<QPoint> m_usedFarAwayBuildings;
};

