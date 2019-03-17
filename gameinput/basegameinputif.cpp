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
    if (dynamic_cast<HumanPlayerInput*>(input) != nullptr)
    {
        pStream << static_cast<qint32>(AiTypes::Human);
        input->serialize(pStream);
    }
    else
    {
        pStream << static_cast<qint32>(AiTypes::Human);
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
        default: // fall back case for damaged files
        {
            ret = new HumanPlayerInput();
            break;
        }
    }
}
