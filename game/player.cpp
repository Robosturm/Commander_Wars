

#include "game/player.h"

#include "game/gamemap.h"

#include "coreengine/mainapp.h"

#include "gameinput/basegameinputif.h"

#include "game/building.h"

#include "game/unit.h"

#include "game/co.h"

#include "menue/gamemenue.h"

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

GameEnums::Alliance Player::checkAlliance(Player* pPlayer)
{
    if (pPlayer == this)
    {
        return GameEnums::Alliance_Friend;
    }
    else
    {
        // todo implement real check for alliance
        return GameEnums::Alliance_Enemy;
    }
}

bool Player::isEnemyUnit(Unit* pUnit)
{
    return (checkAlliance(pUnit->getOwner()) == GameEnums::Alliance_Enemy);
}


void Player::setFonds(const qint32 &value)
{
    fonds = value;
    if (GameMenue::getInstance() != nullptr)
    {
        GameMenue::getInstance()->updatePlayerinfo();
    }
}

void Player::addFonds(const qint32 &value)
{
    setFonds(fonds + value);
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
                if (pBuilding->getOwner() == this)
                {
                    quint32 income = static_cast<quint32>(pBuilding->getBaseIncome() * modifier);
                    // todo modifier income by co's and rules

                    fonds += income;
                }
            }
        }
    }
    setFonds(fonds);
}

qint32 Player::getCostModifier(QString id, qint32 baseCost)
{
    qint32 costModifier = 0;
    if (playerCOs[0].get() != nullptr)
    {
       costModifier += playerCOs[0]->getCostModifier(id, baseCost);
    }
    if (playerCOs[1].get() != nullptr)
    {
       costModifier += playerCOs[1]->getCostModifier(id, baseCost);
    }
    return costModifier;
}

qint32 Player::getCosts(QString id)
{
    Mainapp* pApp = Mainapp::getInstance();
    QJSValue ret = pApp->getInterpreter()->doFunction(id, "getBaseCost");
    qint32 costs = 0;
    if (ret.isNumber())
    {
        costs = ret.toInt();
    }
    costs += getCostModifier(id, costs);
    return costs;
}

void Player::gainPowerstar(qint32 fondsDamage, QPoint position)
{
    if (playerCOs[0].get() != nullptr)
    {
       playerCOs[0]->gainPowerstar(fondsDamage, position);
    }
    if (playerCOs[1].get() != nullptr)
    {
       playerCOs[1]->gainPowerstar(fondsDamage, position);
    }
    if (GameMenue::getInstance() != nullptr)
    {
        GameMenue::getInstance()->updatePlayerinfo();
    }
}

void Player::startOfTurn()
{
    if (playerCOs[0].get() != nullptr)
    {
       playerCOs[0]->setPowerMode(GameEnums::PowerMode_Off);
       playerCOs[0]->startOfTurn();
    }
    if (playerCOs[1].get() != nullptr)
    {
       playerCOs[1]->setPowerMode(GameEnums::PowerMode_Off);
       playerCOs[0]->startOfTurn();
    }
}

QmlVectorUnit* Player::getUnits()
{
    return GameMap::getInstance()->getUnits(this);
}

void Player::updateVisualCORange()
{
    if (playerCOs[0].get() != nullptr)
    {
       if (playerCOs[0]->getCOUnit() != nullptr)
       {
           playerCOs[0]->getCOUnit()->createCORange(playerCOs[0]->getCORange());
       }
    }
    if (playerCOs[1].get() != nullptr)
    {
        if (playerCOs[1]->getCOUnit() != nullptr)
        {
            playerCOs[1]->getCOUnit()->createCORange(playerCOs[1]->getCORange());
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

void Player::setCO(QString coId, quint8 idx)
{
    if (idx <= 1)
    {
        playerCOs[idx] = new CO(coId, this);
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
