#pragma once

#include <QtTypes>
#include "ai/coreai.h"

class GameAction;
using spGameAction = std::shared_ptr<GameAction>;
class QmlVectorUnit;
using spQmlVectorUnit = std::shared_ptr<QmlVectorUnit>;

class TransporterSelector
{
public:
    TransporterSelector(CoreAI & owner);

    void prepareUnloadInformation(spGameAction &pAction, Unit *pUnit, spQmlVectorUnit &pEnemyUnits);
private:
    bool fillUnloadFields(spGameAction &pAction, Unit *pUnit, std::vector<qint32> & unloadedUnits,
                          std::vector<QList<QVariant>> & unloadFields, QVector<qint32> & unitIDx, QStringList & actions);
    bool fallbackUnload(spGameAction &pAction, Unit *pUnit, spQmlVectorUnit &pEnemyUnits, spMenuData & pDataMenu, QStringList & actions);
    std::vector<QList<QVariant>> getUnloadFields(spGameAction &pAction, QVector<qint32> & unitIDx, spMenuData & pDataMenu);
private:
    CoreAI & m_owner;
};

