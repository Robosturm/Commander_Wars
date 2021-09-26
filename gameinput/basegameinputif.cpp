#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

#include "gameinput/humanplayerinput.h"
#include "gameinput/basegameinputif.h"

#include "ai/veryeasyai.h"
#include "ai/proxyai.h"
#include "ai/normalai.h"
#include "ai/heavyai.h"

#include "game/gamemap.h"

#include "resource_management/gamemanager.h"

BaseGameInputIF::BaseGameInputIF(GameEnums::AiTypes aiType)
    : m_AiType(aiType)
{
    setObjectName("BaseGameInputIF");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    setUnitBuildValue("RECON", 0.6f);
    setUnitBuildValue("FLARE", 0.6f);
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

void BaseGameInputIF::serializeInterface(QDataStream& pStream, BaseGameInputIF* input)
{
    if (input == nullptr)
    {
        pStream << static_cast<qint32>(GameEnums::AiTypes_Open);
    }
    else
    {
        pStream << static_cast<qint32>(input->getAiType());
        input->serializeObject(pStream);
    }
}

spBaseGameInputIF BaseGameInputIF::deserializeInterface(QDataStream& pStream, qint32 version)
{
    CONSOLE_PRINT("reading ai", Console::eDEBUG);
    spBaseGameInputIF ret;
    if (version > 7)
    {
        GameEnums::AiTypes type;
        qint32 typeInt;
        pStream >> typeInt;
        type = static_cast<GameEnums::AiTypes>(typeInt);
        ret = createAi(type);
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
        ret = createAi(type);
    }
    return ret;
}

spBaseGameInputIF BaseGameInputIF::createAi(GameEnums::AiTypes type)
{
    CONSOLE_PRINT("Creating AI " + QString::number(type), Console::eDEBUG);
    spBaseGameInputIF ret;
    switch (type)
    {
        case GameEnums::AiTypes_Human:
        {
            ret = spHumanPlayerInput::create();
            break;
        }
        case GameEnums::AiTypes_VeryEasy:
        {
            ret = spVeryEasyAI::create();
            break;
        }
        case GameEnums::AiTypes_Normal:
        {
            ret = spNormalAi::create("normal.ini");
            break;
        }
        case GameEnums::AiTypes_NormalOffensive:
        {
            ret = spNormalAi::create("normalOffensive.ini");
            break;
        }
        case GameEnums::AiTypes_NormalDefensive:
        {
            ret = spNormalAi::create("normalDefensive.ini");
            break;
        }
        case GameEnums::AiTypes_ProxyAi:
        {
            ret = spProxyAi::create();
            break;
        }
        case GameEnums::AiTypes_Open:
        {
            ret = nullptr;
            break;
        }
        case GameEnums::AiTypes_Closed:
        {
            ret = nullptr;
            break;
        }
        default: // fall back case for damaged files or unset ai's
        {
            GameManager* pGameManager = GameManager::getInstance();
            QString id = pGameManager->getHeavyAiID(static_cast<qint32>(type) - GameEnums::AiTypes_Heavy);
            ret = spHeavyAi::create(id);
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
    // modify existing value
    for (qint32 i = 0; i < m_BuildingChanceModifier.size(); i++)
    {
        if (std::get<0>(m_BuildingChanceModifier[i]) == unitID)
        {
            std::get<1>(m_BuildingChanceModifier[i]) = value;
            return;
        }
    }
    // add modifier
    m_BuildingChanceModifier.append(std::tuple<QString, float>(unitID, value));
}

float BaseGameInputIF::getUnitBuildValue(QString unitID)
{
    float modifier = m_pPlayer->getUnitBuildValue(unitID);
    for (qint32 i = 0; i < m_BuildingChanceModifier.size(); i++)
    {
        if (std::get<0>(m_BuildingChanceModifier[i]) == unitID)
        {
            return std::get<1>(m_BuildingChanceModifier[i]) + modifier;
        }
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

void BaseGameInputIF::centerCameraOnAction(GameAction* pAction)
{
    if (Settings::getAutoCamera())
    {
        if (GameMap::getInstance()->getCurrentPlayer() == m_pPlayer ||
            m_pPlayer == nullptr)
        {
            if (pAction != nullptr)
            {
                spGameMenue pMenu = GameMenue::getInstance();
                if (pMenu.get() != nullptr)
                {
                    pMenu->centerMapOnAction(pAction);
                }
            }
            else
            {
                spGameMap pMap =  GameMap::getInstance();
                if (pMap.get() != nullptr)
                {
                    pMap->centerOnPlayer(m_pPlayer);
                }
            }
        }
    }
}
