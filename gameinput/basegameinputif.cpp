#include "basegameinputif.h"

#include "coreengine/interpreter.h"

#include "humanplayerinput.h"

BaseGameInputIF::BaseGameInputIF()
{
    Interpreter::setCppOwnerShip(this);
}

void BaseGameInputIF::setPlayer(Player* pPlayer)
{
    m_pPlayer = pPlayer;
}


void BaseGameInputIF::serializeInterface(QDataStream& pStream, BaseGameInputIF* input)
{

    HumanPlayerInput* humanIf = dynamic_cast<HumanPlayerInput*>(input);
    if (humanIf != nullptr)
    {
        pStream << static_cast<qint32>(AiTypes::Human);
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
        case AiTypes::Unkown: // fall back case for damaged files
        {
            ret = new HumanPlayerInput();
            break;
        }
    }
    return ret;
}
