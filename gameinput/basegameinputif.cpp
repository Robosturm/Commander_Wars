#include "basegameinputif.h"

#include "coreengine/interpreter.h"

#include "humanplayerinput.h"
#include "ai/veryeasyai.h"

#include "coreengine/mainapp.h"

BaseGameInputIF::BaseGameInputIF()
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

    HumanPlayerInput* humanIf = dynamic_cast<HumanPlayerInput*>(input);
     VeryEasyAI* veryEasyAi = dynamic_cast<VeryEasyAI*>(input);
    if (humanIf != nullptr)
    {
        pStream << static_cast<qint32>(AiTypes::Human);
        input->serialize(pStream);
    }
    else if (veryEasyAi != nullptr)
    {
        pStream << static_cast<qint32>(AiTypes::VeryEasy);
        input->serialize(pStream);
    }
    else
    {
        pStream << static_cast<qint32>(AiTypes::Unkown);
    }
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
            ret->deserialize(pStream);
            break;
        }
        case AiTypes::VeryEasy:
        {
            ret = new VeryEasyAI();
            ret->deserialize(pStream);
            break;
        }
        case AiTypes::Unkown: // fall back case for damaged files
        {
            ret = new HumanPlayerInput();
            break;
        }
    }
    return ret;
}
