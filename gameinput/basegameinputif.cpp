#include "basegameinputif.h"

#include "coreengine/interpreter.h"

#include "humanplayerinput.h"
#include "ai/veryeasyai.h"
#include "ai/proxyai.h"
#include "ai/normalai.h"

#include "coreengine/mainapp.h"

BaseGameInputIF::BaseGameInputIF(AiTypes aiType)
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
        pStream << static_cast<qint32>(AiTypes::Open);
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
        AiTypes type;
        qint32 typeInt;
        pStream >> typeInt;
        type = static_cast<AiTypes>(typeInt);
        ret = createAi(type);
        if (ret != nullptr)
        {
            ret->deserializeObject(pStream);
        }
    }
    else
    {
        AiTypes type;
        qint32 typeInt;
        pStream >> typeInt;
        type = static_cast<AiTypes>(typeInt);
        BaseGameInputIF* ret = createAi(type);
    }
    return ret;
}

BaseGameInputIF* BaseGameInputIF::createAi(BaseGameInputIF::AiTypes type)
{
    BaseGameInputIF* ret = nullptr;
    switch (type)
    {
        case AiTypes::Human:
        {
            ret = new HumanPlayerInput();
            break;
        }
        case AiTypes::VeryEasy:
        {
            ret = new VeryEasyAI();
            break;
        }
        case AiTypes::Normal:
        {
            ret = new NormalAi();
            break;
        }
        case AiTypes::ProxyAi:
        {
            ret = new ProxyAi();
            break;
        }
        case AiTypes::Open:
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

BaseGameInputIF::AiTypes BaseGameInputIF::getAiType() const
{
    return m_AiType;
}
