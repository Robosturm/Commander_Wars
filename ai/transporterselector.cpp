#include "ai/transporterselector.h"

#include "coreengine/interpreter.h"
#include "coreengine/gameconsole.h"

#include "game/gamemap.h"

TransporterSelector::TransporterSelector(CoreAI & owner)
    : m_owner(owner)
{
}

void TransporterSelector::prepareUnloadInformation(spGameAction &pAction, Unit *pUnit, spQmlVectorUnit &pEnemyUnits)
{
    bool unloaded = false;
    std::vector<qint32> unloadedUnits;
    do
    {
        unloaded = false;
        spMenuData pDataMenu = pAction->getMenuStepData();
        QVector<qint32> unitIDx = pDataMenu->getCostList();
        if (pDataMenu->validData())
        {
            QStringList actions = pDataMenu->getActionIDs();
            std::vector<QList<QVariant>> unloadFields = getUnloadFields(pAction, unitIDx, pDataMenu);
            if (actions.size() > 1)
            {
                unloaded = fillUnloadFields(pAction, pUnit, unloadedUnits, unloadFields, unitIDx, actions);
                if (unloaded == false)
                {
                    unloaded = fallbackUnload(pAction, pUnit, pEnemyUnits, pDataMenu, actions);
                }
            }
        }
        else
        {
            AI_CONSOLE_PRINT("Error invalid menu data received while unloading units", GameConsole::eERROR);
            break;
        }
    } while (unloaded);
    m_owner.addMenuItemData(pAction, CoreAI::ACTION_WAIT, 0);
}

std::vector<QList<QVariant>> TransporterSelector::getUnloadFields(spGameAction &pAction, QVector<qint32> & unitIDx, spMenuData & pDataMenu)
{
    Interpreter *pInterpreter = Interpreter::getInstance();
    std::vector<QList<QVariant>> unloadFields;
    for (qint32 i = 0; i < unitIDx.size() - 1; i++)
    {
        QString function1 = "getUnloadFields";
        QJSValueList args({
            JsThis::getJsThis(pAction.get()),
            unitIDx[i],
            JsThis::getJsThis(m_owner.getMap()),
        });
        QJSValue ret = pInterpreter->doFunction(CoreAI::ACTION_UNLOAD, function1, args);
        unloadFields.push_back(ret.toVariant().toList());
    }
    return unloadFields;
}

bool TransporterSelector::fillUnloadFields(spGameAction &pAction, Unit *pUnit, std::vector<qint32> & unloadedUnits,
                                           std::vector<QList<QVariant>> & unloadFields, QVector<qint32> & unitIDx,
                                           QStringList & actions)
{
    bool unloaded = false;
    for (qint32 i = 0; i < unloadFields.size(); i++)
    {
        Unit *pLoadedUnit = pUnit->getLoadedUnit(i);
        if (!m_owner.needsRefuel(pLoadedUnit))
        {
            if (!GlobalUtils::contains(unloadedUnits, unitIDx[i]))
            {
                if (unloadFields[i].size() == 1)
                {
                    m_owner.addMenuItemData(pAction, actions[i], unitIDx[i]);
                    spMarkedFieldData pFields = pAction->getMarkedFieldStepData();
                    m_owner.addSelectedFieldData(pAction, pFields->getPoints()->at(0));
                    unloaded = true;
                    unloadedUnits.push_back(unitIDx[i]);
                    break;
                }
                else if (unloadFields[i].size() > 0 &&
                           pUnit->getLoadedUnit(i)->getActionList().contains(CoreAI::ACTION_CAPTURE))
                {
                    auto &fields = unloadFields[i];
                    for (auto &field : fields)
                    {
                        QPoint unloadField = field.toPoint();
                        Terrain* pTerrain = m_owner.getMap()->getTerrain(unloadField.x(), unloadField.y());
                        Building *pBuilding = pTerrain->getBuilding();
                        if (pBuilding != nullptr && m_owner.getPlayer()->isEnemy(pBuilding->getOwner()))
                        {
                            m_owner.addMenuItemData(pAction, actions[i], unitIDx[i]);
                            m_owner.addSelectedFieldData(pAction, unloadField);
                            unloaded = true;
                            unloadedUnits.push_back(unitIDx[i]);
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    return unloaded;
}

bool TransporterSelector::fallbackUnload(spGameAction &pAction, Unit *pUnit, spQmlVectorUnit &pEnemyUnits, spMenuData & pDataMenu, QStringList & actions)
{
    bool unloaded = false;
    if (!m_owner.needsRefuel(pUnit->getLoadedUnit(0)))
    {
        qint32 costs = pDataMenu->getCostList()[0];
        m_owner.addMenuItemData(pAction, actions[0], costs);
        unloaded = true;
        spMarkedFieldData pFields = pAction->getMarkedFieldStepData();
        qint32 field = 0;
        qint32 bestDistance = std::numeric_limits<qint32>::max();
        for (qint32 i = 0; i < pFields->getPoints()->size(); ++i)
        {
            QPoint unloadPos = pFields->getPoints()->at(i);
            qint32 currentBestDistance = std::numeric_limits<qint32>::max();
            for (auto &pEnemy : pEnemyUnits->getVector())
            {
                qint32 distance = GlobalUtils::getDistance(unloadPos, pEnemy->Unit::getPosition());
                if (distance < currentBestDistance)
                {
                    currentBestDistance = distance;
                }
            }
            if (currentBestDistance < bestDistance ||
                (currentBestDistance == bestDistance && GlobalUtils::randInt(0, 1) == 1))
            {
                bestDistance = currentBestDistance;
                field = i;
            }
        }
        m_owner.addSelectedFieldData(pAction, pFields->getPoints()->at(field));
    }
    return unloaded;
}
