#include "game/player.h"

#include "game/gamemap.h"

#include "coreengine/mainapp.h"

#include "gameinput/basegameinputif.h"

#include "game/building.h"

#include "game/unit.h"

#include "game/co.h"

#include "game/gamerules.h"

#include "menue/gamemenue.h"

#include "resource_management/unitspritemanager.h"

Player::Player()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_pBaseGameInput = nullptr;
    // for older versions we allow all loaded units to be buildable
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    for (qint32 i = 0; i < pUnitSpriteManager->getUnitCount(); i++)
    {
        m_BuildList.append(pUnitSpriteManager->getUnitID(i));
    }
}

void Player::init()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function = "loadDefaultPlayerColor";
    QJSValueList args;
    QJSValue objArg = pApp->getInterpreter()->newQObject(this);
    args << objArg;
    pApp->getInterpreter()->doFunction("PLAYER", function, args);
    team = getPlayerID();
}

void Player::loadVisionFields()
{
    GameMap* pMap = GameMap::getInstance();
    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();
    m_FogVisionFields.clear();
    for (qint32 x = 0; x < width; x++)
    {
        m_FogVisionFields.append(QVector<QPoint>());
        for (qint32 y = 0; y < heigth; y++)
        {
            m_FogVisionFields[x].append(QPoint());
        }
    }
}

void Player::loadCOMusic()
{
    if (playerCOs[0].get() != nullptr)
    {
       playerCOs[0]->loadCOMusic();
    }
    if (playerCOs[1].get() != nullptr)
    {
       playerCOs[1]->loadCOMusic();
    }
    if (playerCOs[0].get() == nullptr &&
        playerCOs[0].get() == nullptr)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->getAudioThread()->addMusic("resources/music/cos/no_co.mp3");
    }
}

Player::~Player()
{
}

QColor Player::getColor() const
{
    return m_Color;
}

void Player::swapCOs()
{
    if (playerCOs[1].get() != nullptr)
    {
        spCO co0 = playerCOs[0];
        playerCOs[0] = playerCOs[1];
        playerCOs[1] = co0;
        if (GameMenue::getInstance() != nullptr)
        {
            GameMenue::getInstance()->updatePlayerinfo();
        }
    }
}

void Player::setColor(QColor color)
{
    m_Color = color;
}

qint32 Player::getPlayerID() const
{
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        if (pMap->getPlayer(i) == this)
        {
            return i;
        }
    }
    return -1;
}

QString Player::getArmy()
{
    if (!playerArmy.isEmpty())
    {
        return playerArmy;
    }
    else
    {
        // editor memu mode
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
}

GameEnums::Alliance Player::checkAlliance(Player* pPlayer)
{
    if (pPlayer == this)
    {
        return GameEnums::Alliance_Friend;
    }
    else
    {
        if ((pPlayer != nullptr) &&
            (team == pPlayer->getTeam()))
        {
            return GameEnums::Alliance_Friend;
        }
        else
        {
            return GameEnums::Alliance_Enemy;
        }
    }
}

bool Player::isEnemyUnit(Unit* pUnit)
{
    return (checkAlliance(pUnit->getOwner()) == GameEnums::Alliance_Enemy);
}

bool Player::isEnemy(Player* pOwner)
{
    return (checkAlliance(pOwner) == GameEnums::Alliance_Enemy);
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

qint32 Player::getBuildingCount(QString buildingID)
{
    qint32 ret = 0;
    GameMap* pMap = GameMap::getInstance();
    for (qint32 y = 0; y < pMap->getMapHeight(); y++)
    {
        for (qint32 x = 0; x < pMap->getMapWidth(); x++)
        {
            spBuilding pBuilding = pMap->getSpTerrain(x, y)->getSpBuilding();
            if (pBuilding.get() != nullptr)
            {
                if (pBuilding->getOwner() == this)
                {
                    if (buildingID.isEmpty() || pBuilding->getBuildingID() == buildingID)
                    {
                        if (pBuilding->getX() == x && pBuilding->getY() == y)
                        {
                            ret++;
                        }
                    }
                }
            }
        }
    }
    return ret;
}

qint32 Player::getUnitCount(QString unitID)
{
    qint32 ret = 0;
    GameMap* pMap = GameMap::getInstance();
    for (qint32 y = 0; y < pMap->getMapHeight(); y++)
    {
        for (qint32 x = 0; x < pMap->getMapWidth(); x++)
        {
            spUnit pUnit = pMap->getSpTerrain(x, y)->getSpUnit();
            if (pUnit.get() != nullptr)
            {
                if (pUnit->getOwner() == this)
                {
                    if (unitID.isEmpty() || pUnit->getUnitID() == unitID)
                    {
                        ret++;
                    }
                }
            }
        }
    }
    return ret;
}

qint32 Player::getTeam() const
{
    return team;
}

void Player::setTeam(const qint32 &value)
{
    team = value;
}

void Player::defeatPlayer(Player* pPLayer, bool units)
{
    GameMap* pMap = GameMap::getInstance();
    for (qint32 y = 0; y < pMap->getMapHeight(); y++)
    {
        for (qint32 x = 0; x < pMap->getMapWidth(); x++)
        {
            spBuilding pBuilding = pMap->getSpTerrain(x, y)->getSpBuilding();
            spUnit pUnit = pMap->getSpTerrain(x, y)->getSpUnit();
            if (pBuilding.get() != nullptr)
            {
                if (pBuilding->getOwner() == this)
                {
                    pBuilding->setOwner(pPLayer);
                    // reset capturing for buildings we earned at this moment
                    if (pUnit.get() != nullptr &&
                        pUnit->getOwner() == pPLayer)
                    {
                        pUnit->setCapturePoints(0);
                    }
                }
            }

            if (pUnit.get() != nullptr)
            {
                if (pUnit->getOwner() == this)
                {
                    if ((pPLayer != nullptr) && units)
                    {
                        pUnit->setOwner(pPLayer);
                        if (pUnit->getUnitRank() >= GameEnums::UnitRank_CO0)
                        {
                            pUnit->setUnitRank(GameEnums::UnitRank_Veteran);
                        }
                    }
                    else
                    {
                        pUnit->killUnit();
                    }
                }
            }
        }
    }
    isDefeated = true;
    if (GameMenue::getInstance() != nullptr)
    {
        GameMenue::getInstance()->updatePlayerinfo();
    }
}

bool Player::getIsDefeated() const
{
    return isDefeated;
}

qint32 Player::getIncomeReduction(Building* pBuilding, qint32 income)
{
    qint32 reduction = 0;
    CO* pCO = getCO(0);
    if (pCO != nullptr)
    {
        reduction += pCO->getIncomeReduction(pBuilding, income);
    }
    pCO = getCO(1);
    if (pCO != nullptr)
    {
        reduction += pCO->getIncomeReduction(pBuilding, income);
    }
    return reduction;
}

qint32 Player::calcIncome(float modifier)
{
    qint32 ret = 0;
    GameMap* pMap = GameMap::getInstance();
    for (qint32 y = 0; y < pMap->getMapHeight(); y++)
    {
        for (qint32 x = 0; x < pMap->getMapWidth(); x++)
        {
            spBuilding pBuilding = pMap->getSpTerrain(x, y)->getSpBuilding();
            if (pBuilding.get() != nullptr)
            {
                if (pBuilding->getOwner() == this)
                {
                    qint32 income = static_cast<qint32>(pBuilding->getBaseIncome() * modifier * fondsModifier);
                    qint32 modifier = 0;
                    CO* pCO = getCO(0);
                    if (pCO != nullptr)
                    {
                        modifier += pCO->getBonusIncome(pBuilding.get(), income);
                    }
                    pCO = getCO(1);
                    if (pCO != nullptr)
                    {
                        modifier += pCO->getBonusIncome(pBuilding.get(), income);
                    }
                    income = static_cast<qint32>(income) + modifier;

                    modifier = 0;
                    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
                    {
                        Player* pPlayer = pMap->getPlayer(i);
                        if (isEnemy(pPlayer))
                        {
                            modifier -= pPlayer->getIncomeReduction(pBuilding.get(), income);
                        }
                    }
                    income += modifier;

                    if (income < 0)
                    {
                        income = 0;
                    }
                    ret += income;
                }
            }
        }
    }
    return ret;
}

void Player::earnMoney(float modifier)
{


    setFonds(fonds + calcIncome(modifier));
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

void Player::postBattleActions(Unit* pAttacker, float atkDamage, Unit* pDefender, bool gotAttacked)
{
    if (playerCOs[0].get() != nullptr)
    {
       playerCOs[0]->postBattleActions(pAttacker, atkDamage, pDefender, gotAttacked);
    }
    if (playerCOs[1].get() != nullptr)
    {
       playerCOs[1]->postBattleActions(pAttacker, atkDamage, pDefender, gotAttacked);
    }
}

void Player::buildedUnit(Unit* pUnit)
{
    if (playerCOs[0].get() != nullptr)
    {
       playerCOs[0]->buildedUnit(pUnit);
    }
    if (playerCOs[1].get() != nullptr)
    {
       playerCOs[1]->buildedUnit(pUnit);
    }
}

QStringList Player::getBuildList() const
{
    return m_BuildList;
}

void Player::setBuildList(const QStringList &BuildList)
{
    m_BuildList = BuildList;
}

void Player::addVisionField(qint32 x, qint32 y, qint32 duration)
{
    m_FogVisionFields[x][y].setX(1);
    if (duration > m_FogVisionFields[x][y].y())
    {
        m_FogVisionFields[x][y].setY(duration);
    }
}

void Player::updatePlayerVision(bool reduceTimer)
{
    GameMap* pMap = GameMap::getInstance();
    // only update visual stuff if needed
    if (reduceTimer ||
        pMap->getCurrentPlayer() == this ||
        pMap->getCurrentViewPlayer() == this)
    {
        qint32 width = pMap->getMapWidth();
        qint32 heigth = pMap->getMapHeight();
        for (qint32 x = 0; x < width; x++)
        {
            for (qint32 y = 0; y < heigth; y++)
            {
                if (reduceTimer)
                {
                    m_FogVisionFields[x][y].setY(m_FogVisionFields[x][y].y() - 1);
                }
                if (m_FogVisionFields[x][y].y() <= 0)
                {
                    m_FogVisionFields[x][y].setX(0);
                    m_FogVisionFields[x][y].setY(0);
                }
            }
        }
        for (qint32 x = 0; x < width; x++)
        {
            for (qint32 y = 0; y < heigth; y++)
            {
                Terrain* pTerrain = pMap->getTerrain(x, y);
                qint32 visionRange = pTerrain->getVision();
                if (visionRange >= 0)
                {
                    QmlVectorPoint* pPoints = Mainapp::getCircle(0, visionRange);
                    for (qint32 i = 0; i < pPoints->size(); i++)
                    {
                        QPoint point = pPoints->at(i);
                        if (pMap->onMap(point.x() + x, point.y() + y))
                        {
                            Terrain* visionField = pMap->getTerrain(point.x() + x,point.y() + y);
                            Unit* pUnit = visionField->getUnit();
                            bool visionHide = visionField->getVisionHide(this);
                            if ((!visionHide) ||
                                ((pUnit != nullptr) && visionHide &&
                                 !pUnit->useTerrainDefense() && !pUnit->isStealthed(this)))
                            {
                                m_FogVisionFields[point.x() + x][point.y() + y].setX(1);
                            }
                        }
                    }
                    delete pPoints;
                }
                Building* pBuilding = pTerrain->getBuilding();
                if ((pBuilding != nullptr) &&
                    ((pBuilding->getOwner() == this) ||
                     (checkAlliance(pBuilding->getOwner()) == GameEnums::Alliance_Friend)))
                {
                    m_FogVisionFields[x][y].setX(1);

                }
                Unit* pUnit = pTerrain->getUnit();
                if ((pUnit != nullptr) &&
                    (pUnit->getOwner() == this))
                {
                    qint32 visionRange = pUnit->getVision(QPoint(x, y));
                    QmlVectorPoint* pPoints = Mainapp::getCircle(0, visionRange);
                    for (qint32 i = 0; i < pPoints->size(); i++)
                    {
                        QPoint point = pPoints->at(i);
                        if (pMap->onMap(point.x() + x, point.y() + y))
                        {
                            Terrain* visionField = pMap->getTerrain(point.x() + x,point.y() + y);
                            Unit* pUnit = visionField->getUnit();
                            bool visionHide = visionField->getVisionHide(this);
                            if ((!visionHide) ||
                                ((pUnit != nullptr) && visionHide &&
                                 !pUnit->useTerrainDefense() && !pUnit->isStealthed(this)))
                            {
                                m_FogVisionFields[point.x() + x][point.y() + y].setX(1);
                            }
                            // terrain hides are visible if we're near it.
                            else if (((qAbs(point.x()) + qAbs(point.y())) <= 1))
                            {
                                m_FogVisionFields[point.x() + x][point.y() + y].setX(1);
                            }
                            else
                            {
                                // do nothing
                            }
                        }
                    }
                    delete pPoints;
                }
            }
        }
    }
}

bool Player::getFieldVisible(qint32 x, qint32 y)
{
    GameMap* pMap = GameMap::getInstance();
    switch (pMap->getGameRules()->getFogMode())
    {
        case GameEnums::Fog_Off:
        {
            return true;
        }
        case GameEnums::Fog_OfWar:
        {
            return static_cast<bool>(m_FogVisionFields[x][y].x());
        }
    }
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

qint32 Player::getMovementcostModifier(Unit* pUnit, QPoint position)
{
    qint32 modifier = 0;
    if (playerCOs[0].get() != nullptr)
    {
       modifier += playerCOs[0]->getMovementcostModifier(pUnit, position);
    }
    if (playerCOs[1].get() != nullptr)
    {
       modifier += playerCOs[1]->getMovementcostModifier(pUnit, position);
    }
    GameMap* pMap = GameMap::getInstance();
    modifier += pMap->getGameRules()->getCurrentWeather()->getMovementCostModifier();
    return modifier;
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
       playerCOs[1]->startOfTurn();
    }
}

QmlVectorUnit* Player::getUnits()
{
    return GameMap::getInstance()->getUnits(this);
}

QmlVectorUnit* Player::getEnemyUnits()
{
    GameMap* pMap = GameMap::getInstance();
    qint32 heigth = pMap->getMapHeight();
    qint32 width = pMap->getMapWidth();
    QmlVectorUnit* ret = new QmlVectorUnit();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
            if (pUnit != nullptr)
            {
                if ((isEnemyUnit(pUnit)))
                {
                   ret->append(pUnit);
                }
            }
        }
    }
    return ret;
}

QmlVectorBuilding* Player::getEnemyBuildings()
{
    GameMap* pMap = GameMap::getInstance();
    qint32 heigth = pMap->getMapHeight();
    qint32 width = pMap->getMapWidth();
    QmlVectorBuilding* ret = new QmlVectorBuilding();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
            if (pBuilding != nullptr)
            {
                if (isEnemy(pBuilding->getOwner()))
                {
                   ret->append(pBuilding);
                }
            }
        }
    }
    return ret;
}

QmlVectorBuilding* Player::getBuildings()
{
    return GameMap::getInstance()->getBuildings(this);
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

spCO Player::getspCO(quint8 id)
{
    if (id <= 1)
    {
        return playerCOs[id];
    }
    else
    {
        return nullptr;
    }
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
        if (coId.isEmpty())
        {
            playerCOs[idx] = nullptr;
        }
        else
        {
            playerCOs[idx] = new CO(coId, this);
        }

    }
}

QPoint Player::getRockettarget(qint32 radius, qint32 damage, float ownUnitValue, GameEnums::RocketTarget targetType)
{
    GameMap* pMap = GameMap::getInstance();
    QmlVectorPoint* pPoints = Mainapp::getCircle(0, radius);
    qint32 highestDamage = -1;
    QVector<QPoint> targets;

    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    qint32 averageCosts = 0;
    for (qint32 i = 0; i < pUnitSpriteManager->getUnitCount(); i++)
    {
        QString unitId = pUnitSpriteManager->getUnitID(i);
        Mainapp* pApp = Mainapp::getInstance();
        QString function1 = "getBaseCost";
        QJSValue erg = pApp->getInterpreter()->doFunction(unitId, function1);
        if (erg.isNumber())
        {
             averageCosts += erg.toInt();
        }
    }
    averageCosts = averageCosts / pUnitSpriteManager->getUnitCount();

    for (qint32 x = 0; x < pMap->getMapWidth(); x++)
    {
        for (qint32 y = 0; y < pMap->getMapHeight(); y++)
        {
            qint32 damageDone = 0;
            for (qint32 i = 0; i < pPoints->size(); i++)
            {
                qint32 x2 = x + pPoints->at(i).x();
                qint32 y2 = y + pPoints->at(i).y();
                // is there a unit?
                if ((pMap->onMap(x2, y2)) &&
                    (pMap->getTerrain(x2, y2)->getUnit() != nullptr))
                {
                    Unit* pUnit = pMap->getTerrain(x2, y2)->getUnit();
                    float modifier = 1.0f;
                    if (!isEnemyUnit(pUnit))
                    {
                        modifier = -ownUnitValue;
                    }
                    float damagePoints = damage;
                    qint32 hpRounded = pUnit->getHpRounded();
                    if (hpRounded < damage)
                    {
                        damagePoints = hpRounded;
                    }
                    switch (targetType)
                    {
                        case GameEnums::RocketTarget_Money:
                        {
                            // calc fonds damage
                            damageDone += damagePoints / 10.0f * modifier * pUnit->getCosts();
                            break;
                        }
                        case GameEnums::RocketTarget_HpHighMoney:
                        {
                            // calc fonds damage
                            if (pUnit->getCosts() >= averageCosts / 2)
                            {
                                damageDone += damagePoints * modifier * 4;
                            }
                            else
                            {
                                damageDone += damagePoints * modifier;
                            }
                            break;
                        }
                        case GameEnums::RocketTarget_HpLowMoney:
                        {
                            // calc fonds damage
                            if (pUnit->getCosts() <= averageCosts / 2)
                            {
                                damageDone += damagePoints * modifier * 4;
                            }
                            else
                            {
                                damageDone += damagePoints * modifier;
                            }
                            break;
                        }
                    }
                }
            }
            if (damageDone > highestDamage)
            {
                highestDamage = damageDone;
                targets.clear();
                targets.append(QPoint(x, y));
            }
            else if ((damageDone == highestDamage) && highestDamage >= 0)
            {
                targets.append(QPoint(x, y));
            }
        }
    }
    delete pPoints;

    if (targets.size() >= 0)
    {
        return targets[Mainapp::randInt(0, targets.size() - 1)];
    }
    else
    {
        return QPoint(-1, -1);
    }
}

void Player::defineArmy()
{
    if (playerCOs[0].get() != nullptr)
    {
        playerArmy = playerCOs[0]->getCOArmy();
    }
}

float Player::getFondsModifier() const
{
    return fondsModifier;
}

void Player::setFondsModifier(float value)
{
    fondsModifier = value;
}

qint32 Player::calculatePlayerStrength()
{
    qint32 ret = 0;
    GameMap* pMap = GameMap::getInstance();
    for (qint32 x = 0; x < pMap->getMapWidth(); x++)
    {
        for (qint32 y = 0; y < pMap->getMapHeight(); y++)
        {
            Terrain* pTerrain = pMap->getTerrain(x, y);
            Unit* pUnit = pTerrain->getUnit();
            if (pUnit != nullptr &&
                pUnit->getOwner() == this)
            {
                ret += pUnit->getUnitValue();
            }
        }
    }
    return ret + calcIncome();
}

void Player::serializeObject(QDataStream& pStream)
{
    pStream << getVersion();
    quint32 color = m_Color.rgb();
    pStream << color;

    pStream << fonds;
    pStream << fondsModifier;
    pStream << playerArmy;
    if (playerCOs[0].get() == nullptr)
    {
        pStream << false;
    }
    else
    {
        pStream << true;
        playerCOs[0]->serializeObject(pStream);
    }
    if (playerCOs[1].get() == nullptr)
    {
        pStream << false;
    }
    else
    {
        pStream << true;
        playerCOs[1]->serializeObject(pStream);
    }
     pStream << team;
     pStream << isDefeated;
     BaseGameInputIF::serializeInterface(pStream, m_pBaseGameInput.get());
     qint32 width = m_FogVisionFields.size();
     qint32 heigth = 0;
     if (width > 0)
     {
        heigth = m_FogVisionFields[0].size();
     }
     pStream << width;
     pStream << heigth;
     for (qint32 x = 0; x < width; x++)
     {
         for (qint32 y = 0; y < heigth; y++)
         {
             pStream << m_FogVisionFields[x][y].x();
             pStream << m_FogVisionFields[x][y].y();
         }
     }
     pStream << m_BuildList;
}
void Player::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    quint32 color;
    pStream >> color;
    m_Color = QColor::fromRgb(color);
    if (version > 1)
    {
        if (version < 3)
        {
            qint32 dummy = 0;
            pStream >> dummy;
        }
        pStream >> fonds;
        pStream >> fondsModifier;
        pStream >> playerArmy;
        bool hasC0 = false;
        pStream >> hasC0;
        if (hasC0)
        {
            playerCOs[0] = new CO("", this);
            playerCOs[0]->deserializeObject(pStream);
        }
        bool hasC1 = false;
        pStream >> hasC1;
        if (hasC1)
        {
            playerCOs[1] = new CO("", this);
            playerCOs[1]->deserializeObject(pStream);
        }
        if (version > 3)
        {
            pStream >> team;
        }
        if (version > 4)
        {
            pStream >> isDefeated;
            m_pBaseGameInput = BaseGameInputIF::deserializeInterface(pStream);
            m_pBaseGameInput->setPlayer(this);
        }
        if (version > 5)
        {
            qint32 width = 0;
            qint32 heigth = 0;
            pStream >> width;
            pStream >> heigth;
            for (qint32 x = 0; x < width; x++)
            {
                m_FogVisionFields.append(QVector<QPoint>());
                for (qint32 y = 0; y < heigth; y++)
                {
                    m_FogVisionFields[x].append(QPoint());
                    qint32 value = 0;
                    pStream >> value;
                    m_FogVisionFields[x][y].setX(value);
                    pStream >> value;
                    m_FogVisionFields[x][y].setY(value);
                }
            }
        }
    }
    m_BuildList.clear();
    if (version > 6)
    {
        pStream >> m_BuildList;
    }
    else
    {
        // for older versions we allow all loaded units to be buildable
        UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
        for (qint32 i = 0; i < pUnitSpriteManager->getUnitCount(); i++)
        {
            m_BuildList.append(pUnitSpriteManager->getUnitID(i));
        }
    }
}
