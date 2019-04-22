#include "basegameinputif.h"

#include "coreengine/interpreter.h"

#include "humanplayerinput.h"
#include "ai/veryeasyai.h"

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


void BaseGameInputIF::serializeInterface(QDataStream& pStream, BaseGameInputIF* input)
{
    pStream << static_cast<qint32>(input->getAiType());
    input->serializeObject(pStream);
}

BaseGameInputIF* BaseGameInputIF::deserializeInterface(QDataStream& pStream)
{
    BaseGameInputIF* ret = nullptr;
    AiTypes type;
    qint32 typeInt;
    pStream >> typeInt;
    type = static_cast<AiTypes>(typeInt);
    switch (type)
    {
        case AiTypes::Human:
        {
            ret = new HumanPlayerInput();
            ret->deserializeObject(pStream);
            break;
        }
        case AiTypes::VeryEasy:
        {
            ret = new VeryEasyAI();
            ret->deserializeObject(pStream);
            break;
        }
        case AiTypes::Unkown: // fall back case for damaged files
        {
            ret = new HumanPlayerInput();
            ret->deserializeObject(pStream);
            break;
        }
    }
    return ret;
}

BaseGameInputIF::AiTypes BaseGameInputIF::getAiType() const
{
    return m_AiType;
}
