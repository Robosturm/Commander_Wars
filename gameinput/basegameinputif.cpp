#include "basegameinputif.h"

#include "coreengine/interpreter.h"

#include "humanplayerinput.h"
#include "ai/veryeasyai.h"
#include "ai/proxyai.h"
#include "ai/normalai.h"
#include "ai/heavyai.h"
#include "game/gamemap.h"

#include "coreengine/mainapp.h"

BaseGameInputIF::BaseGameInputIF(GameEnums::AiTypes aiType)
    : m_AiType(aiType)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void BaseGameInputIF::setPlayer(Player* pPlayer)
{
    m_pPlayer = pPlayer;
}


bool BaseGameInputIF::getEnableNeutralTerrainAttack() const
{
    return enableNeutralTerrainAttack;
}

void BaseGameInputIF::setEnableNeutralTerrainAttack(bool value)
{
    enableNeutralTerrainAttack = value;
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

BaseGameInputIF* BaseGameInputIF::deserializeInterface(QDataStream& pStream, qint32 version)
{
    BaseGameInputIF* ret = nullptr;
    if (version > 7)
    {
        GameEnums::AiTypes type;
        qint32 typeInt;
        pStream >> typeInt;
        type = static_cast<GameEnums::AiTypes>(typeInt);
        ret = createAi(type);
        if (ret != nullptr)
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

BaseGameInputIF* BaseGameInputIF::createAi(GameEnums::AiTypes type)
{
    Console::print("Creating AI " + QString::number(type), Console::eDEBUG);
    BaseGameInputIF* ret = nullptr;
    switch (type)
    {
        case GameEnums::AiTypes_Human:
        {
            ret = new HumanPlayerInput();
            break;
        }
        case GameEnums::AiTypes_VeryEasy:
        {
            ret = new VeryEasyAI();
            break;
        }
        case GameEnums::AiTypes_Normal:
        {
            ret = new NormalAi("normal.ini");
            break;
        }
        case GameEnums::AiTypes_NormalOffensive:
        {
            ret = new NormalAi("normalOffensive.ini");
            break;
        }
        case GameEnums::AiTypes_NormalDefensive:
        {
            ret = new NormalAi("normalDefensive.ini");
            break;
        }
        case GameEnums::AiTypes_ProxyAi:
        {
            ret = new ProxyAi();
            break;
        }
        case GameEnums::AiTypes_Heavy:
        {
            ret = new HeavyAi();
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
            ret = new HumanPlayerInput();
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
                GameMenue::getInstance()->centerMapOnAction(pAction);
            }
            else
            {
                GameMap::getInstance()->centerOnPlayer(m_pPlayer);
            }
        }
    }
}
