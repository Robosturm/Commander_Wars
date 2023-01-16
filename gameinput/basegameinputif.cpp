#include "coreengine/interpreter.h"

#include "gameinput/humanplayerinput.h"
#include "gameinput/basegameinputif.h"

#include "ai/veryeasyai.h"
#include "ai/proxyai.h"
#include "ai/normalai.h"
#include "ai/heavyai.h"
#include "ai/dummyai.h"

#include "game/gamemap.h"

#include "resource_management/gamemanager.h"

BaseGameInputIF::BaseGameInputIF(GameMap* pMap, GameEnums::AiTypes aiType)
    : m_AiType(aiType),
      m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("BaseGameInputIF");
#endif
    Interpreter::setCppOwnerShip(this);
}

void BaseGameInputIF::onGameStart()
{

}

void BaseGameInputIF::setPlayer(Player* pPlayer)
{
    m_pPlayer = pPlayer;
}


bool BaseGameInputIF::getEnableNeutralTerrainAttack() const
{
    return m_enableNeutralTerrainAttack;
}

void BaseGameInputIF::setEnableNeutralTerrainAttack(bool value)
{
    m_enableNeutralTerrainAttack = value;
}

void BaseGameInputIF::serializeInterface(QDataStream& pStream, BaseGameInputIF* input, GameEnums::AiTypes aiType)
{
    if (input == nullptr)
    {
        if (aiType == GameEnums::AiTypes_Open ||
            aiType == GameEnums::AiTypes_Closed)
        {
            pStream << static_cast<qint32>(aiType);
        }
        else
        {
            pStream << GameEnums::AiTypes_Open;
        }
    }
    else
    {
        auto type = input->getAiType();
        CONSOLE_PRINT("Serializing ai " + QString::number(type), GameConsole::eDEBUG);
        pStream << static_cast<qint32>(type);
        input->serializeObject(pStream);
    }
}

spBaseGameInputIF BaseGameInputIF::deserializeInterface(GameMap* pMap, QDataStream& pStream, qint32 version)
{
    CONSOLE_PRINT("reading ai", GameConsole::eDEBUG);
    spBaseGameInputIF ret;
    if (version > 7)
    {
        GameEnums::AiTypes type;
        qint32 typeInt;
        pStream >> typeInt;
        type = static_cast<GameEnums::AiTypes>(typeInt);
        ret = createAi(pMap, type);
        if (ret.get() != nullptr)
        {
            ret->deserializeObject(pStream);
        }
    }
    else
    {
        GameEnums::AiTypes type;
        qint32 typeInt;
        pStream >> typeInt;
        type = static_cast<GameEnums::AiTypes>(typeInt);
        ret = createAi(pMap, type);
    }
    return ret;
}

spBaseGameInputIF BaseGameInputIF::createAi(GameMap* pMap, GameEnums::AiTypes type)
{
    CONSOLE_PRINT("Creating AI " + QString::number(type), GameConsole::eDEBUG);
    spBaseGameInputIF ret;
    switch (type)
    {
        case GameEnums::AiTypes_Human:
        {
            ret = spHumanPlayerInput::create(pMap);
            break;
        }
        case GameEnums::AiTypes_VeryEasy:
        {
            if (Settings::getSpawnAiProcess() &&
                !Settings::getAiSlave())
            {
                ret = spDummyAi::create(pMap, type);
            }
            else
            {
                ret = spVeryEasyAI::create(pMap);
            }
            break;
        }
        case GameEnums::AiTypes_Normal:
        {
            if (Settings::getSpawnAiProcess() &&
                !Settings::getAiSlave())
            {
                ret = spDummyAi::create(pMap, type);
            }
            else
            {
                ret = spNormalAi::create(pMap, "normal.ini", type, "NORMALAI");
            }
            break;
        }
        case GameEnums::AiTypes_NormalOffensive:
        {
            if (Settings::getSpawnAiProcess() &&
                !Settings::getAiSlave())
            {
                ret = spDummyAi::create(pMap, type);
            }
            else
            {
                ret = spNormalAi::create(pMap, "normalOffensive.ini", type, "NORMALAIOFFENSIVE");
            }
            break;
        }
        case GameEnums::AiTypes_NormalDefensive:
        {
            if (Settings::getSpawnAiProcess() &&
                !Settings::getAiSlave())
            {
                ret = spDummyAi::create(pMap, type);
            }
            else
            {
                ret = spNormalAi::create(pMap, "normalDefensive.ini", type, "NORMALAIDEFENSIVE");
            }
            break;
        }
        case GameEnums::AiTypes_ProxyAi:
        {
            ret = spProxyAi::create(pMap);
            break;
        }
        case GameEnums::AiTypes_DummyAi:
        {
            ret = spDummyAi::create(pMap, type);
            break;
        }
        case GameEnums::AiTypes_Open:
        case GameEnums::AiTypes_Closed:
        {
            ret = nullptr;
            break;
        }
        default: // heavy ai case
        {
            if (Settings::getSpawnAiProcess() &&
                !Settings::getAiSlave())
            {
                ret = spDummyAi::create(pMap, type);
            }
            else
            {
                GameManager* pGameManager = GameManager::getInstance();
                QString id = pGameManager->getHeavyAiID(static_cast<qint32>(type) - GameEnums::AiTypes_Heavy);
                ret = spHeavyAi::create(pMap, id, type);
            }
            break;
        }
    }
    return ret;
}

GameEnums::AiTypes BaseGameInputIF::getAiType() const
{
    return m_AiType;
}

void BaseGameInputIF::setUnitBuildValue(QString unitID, float value)
{
    m_BuildingChanceModifier.insert(unitID, value);
}

float BaseGameInputIF::getUnitBuildValue(QString unitID)
{
    float modifier = m_pPlayer->getUnitBuildValue(unitID);
    if (m_BuildingChanceModifier.contains(unitID))
    {
        return modifier + m_BuildingChanceModifier[unitID];
    }
    return 1.0f + modifier;
}

void BaseGameInputIF::setMoveCostMapValue(qint32 x, qint32 y, qint32 value)
{
    if ((m_MoveCostMap.size() > x && x >= 0) &&
        (m_MoveCostMap[x].size() > y && y >= 0))
    {
        m_MoveCostMap[x][y] = std::tuple<qint32, bool>(value, true);
    }
}

qint32 BaseGameInputIF::getMoveCostMapValue(qint32 x, qint32 y)
{
    if ((m_MoveCostMap.size() > x && x >= 0) &&
        (m_MoveCostMap[x].size() > y && y >= 0))
    {
        return std::get<0>(m_MoveCostMap[x][y]);
    }
    return 0.0f;
}

bool BaseGameInputIF::getProcessing() const
{
    return m_processing;
}

void BaseGameInputIF::centerCameraOnAction(GameAction* pAction)
{
    if (Settings::getAutoCamera())
    {
        if ((m_pMap != nullptr && m_pMap->getCurrentPlayer() == m_pPlayer) ||
            m_pPlayer == nullptr)
        {
            if (pAction != nullptr)
            {
                if (m_pMenu != nullptr)
                {
                    m_pMenu->getActionPerformer().centerMapOnAction(pAction);
                }
            }
            else
            {
                if (m_pMap != nullptr)
                {
                    m_pMap->centerOnPlayer(m_pPlayer);
                }
            }
        }
    }
}
