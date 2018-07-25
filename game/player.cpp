#include "game/player.h"

#include "coreengine/mainapp.h"

Player::Player(quint32 id)
    : playerID(id)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function = "loadDefaultPlayerColor";
    QJSValueList args;
    QJSValue objArg = pApp->getInterpreter()->newQObject(this);
    args << objArg;
    pApp->getInterpreter()->doFunction("PLAYER", function, args);
}

QColor Player::getColor() const
{
    return m_Color;
}

void Player::setColor(QColor color)
{
    m_Color = color;
}

quint32 Player::getPlayerID() const
{
    return playerID;
}

void Player::setPlayerID(const quint32 &value)
{
    playerID = value;
}

QString Player::getArmy()
{
    // todo return ko army
    Mainapp* pApp = Mainapp::getInstance();
    QJSValueList args;
    QJSValue objArg = pApp->getInterpreter()->newQObject(this);
    args << objArg;
    QJSValue ret = pApp->getInterpreter()->doFunction("PLAYER", "getDefaultArmy", args);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "OS";
    }
}
