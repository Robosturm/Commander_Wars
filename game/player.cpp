

#include "game/player.h"

#include "game/gamemap.h"

#include "coreengine/mainapp.h"

#include "gameinput/basegameinputif.h"

#include "game/building.h"

#include "game/unit.h"

#include "game/co.h"

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

Player::~Player()
{
    if (m_pBaseGameInput != nullptr)
    {
        delete m_pBaseGameInput;
    }
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

Player::Alliance Player::checkAlliance(Player* pPlayer)
{
    if (pPlayer == this)
    {
        return Alliance::Friend;
    }
    else
    {
        // todo implement real check for alliance
        return Alliance::Enemy;
    }
}

bool Player::isEnemyUnit(Unit* pUnit)
{
    return (checkAlliance(pUnit->getOwner()) == Alliance::Enemy);
}


void Player::setFonds(const qint32 &value)
{
    fonds = value;
}

void Player::addFonds(const qint32 &value)
{
    fonds += value;
}

qint32 Player::getFonds() const
{
    return fonds;
}

void Player::earnMoney(float modifier)
{
    GameMap* pMap = GameMap::getInstance();
    for (qint32 y = 0; y < pMap->getMapHeight(); y++)
    {
        for (qint32 x = 0; x < pMap->getMapHeight(); x++)
        {
            spBuilding pBuilding = pMap->getSpTerrain(x, y)->getSpBuilding();
            if (pBuilding.get() != nullptr)
            {
                quint32 income = static_cast<quint32>(pBuilding->getBaseIncome() * modifier);
                // todo modifier income by co's and rules

                fonds += income;
            }
        }
    }
}

void Player::setBaseGameInput(BaseGameInputIF *pBaseGameInput)
{
    m_pBaseGameInput = pBaseGameInput;
    m_pBaseGameInput->setPlayer(this);
}

CO* Player::getCO(quint8 id)
{
    if (id <= 1)
    {
        return playerCOs[id].get();
    }
    else
    {
        return nullptr;
    }
}

void Player::serialize(QDataStream& pStream)
{
    pStream << getVersion();
    quint32 color = m_Color.rgb();
    pStream << color;
}
void Player::deserialize(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    quint32 color;
    pStream >> color;
    m_Color.fromRgb(color);
}
