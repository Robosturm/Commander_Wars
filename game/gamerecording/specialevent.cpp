#include "game/gamerecording/specialevent.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

SpecialEvent::SpecialEvent()
{
    setObjectName("SpecialEvent");
    moveToThread(Mainapp::getInstance()->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

SpecialEvent::SpecialEvent(qint32 player, qint32 day, GameEnums::GameRecord_SpecialEvents event)
    : QObject(),
      m_Player(player),
      m_Day(day),
      m_Event(event)
{
    setObjectName("SpecialEvent");
    moveToThread(Mainapp::getInstance()->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

qint32 SpecialEvent::getOwner() const
{
    return m_Player;
}

qint32 SpecialEvent::getDay() const
{
    return m_Day;
}

GameEnums::GameRecord_SpecialEvents SpecialEvent::getEvent() const
{
    return m_Event;
}

void SpecialEvent::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_Player;
    pStream << m_Day;
    pStream << static_cast<qint32>(m_Event);
}

void SpecialEvent::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_Player;
    pStream >> m_Day;
    qint32 event = 0;
    pStream >> event;
    m_Event = static_cast<GameEnums::GameRecord_SpecialEvents>(event);
}

