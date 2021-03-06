#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"
#include "coreengine/audiothread.h"

#include "gameinput/basegameinputif.h"
#include "gameinput/humanplayerinput.h"

#include "game/building.h"
#include "game/unit.h"
#include "game/co.h"
#include "game/gamerules.h"
#include "game/player.h"
#include "game/gamemap.h"
#include "game/gameanimation/gameanimation.h"

#include "menue/gamemenue.h"

#include "resource_management/unitspritemanager.h"

oxygine::spResAnim Player::m_neutralTableAnim = nullptr;

void Player::releaseStaticData()
{
    m_neutralTableAnim = nullptr;
}

Player::Player()
{
    setObjectName("Player");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_pBaseGameInput = nullptr;
    // for older versions we allow all loaded units to be buildable
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    for (qint32 i = 0; i < pUnitSpriteManager->getCount(); i++)
    {
        m_BuildList.append(pUnitSpriteManager->getID(i));
    }
}

void Player::init()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "loadDefaultPlayerColor";
    QJSValueList args;
    QJSValue objArg = pInterpreter->newQObject(this);
    args << objArg;
    pInterpreter->doFunction("PLAYER", function, args);
    m_team = getPlayerID();
    setColor(m_Color, m_team);
}

BaseGameInputIF* Player::getBaseGameInput()
{
    return m_pBaseGameInput.get();
}

float Player::getUnitBuildValue(QString unitID)
{
    float modifier = 0.0f;
    if (m_playerCOs[0].get() != nullptr)
    {
        modifier += m_playerCOs[0]->getUnitBuildValue(unitID);
    }
    if (m_playerCOs[1].get() != nullptr)
    {
        modifier += m_playerCOs[1]->getUnitBuildValue(unitID);
    }
    return modifier;
}

void Player::loadVisionFields()
{
    spGameMap pMap = GameMap::getInstance();
    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();
    GameEnums::Fog mode = pMap->getGameRules()->getFogMode();
    m_FogVisionFields.clear();
    for (qint32 x = 0; x < width; x++)
    {
        m_FogVisionFields.append(QVector<std::tuple<GameEnums::VisionType, qint32, bool>>());
        for (qint32 y = 0; y < heigth; y++)
        {
            switch (mode)
            {
                case GameEnums::Fog::Fog_Off:
                {
                    m_FogVisionFields[x].append(std::tuple<GameEnums::VisionType, qint32, bool>(GameEnums::VisionType_Clear, 0, false));
                    break;
                }
                case GameEnums::Fog::Fog_OfWar:
                {
                    m_FogVisionFields[x].append(std::tuple<GameEnums::VisionType, qint32, bool>(GameEnums::VisionType_Fogged, 0, false));
                    break;
                }
                case GameEnums::Fog::Fog_OfShroud:
                {
                    m_FogVisionFields[x].append(std::tuple<GameEnums::VisionType, qint32, bool>(GameEnums::VisionType_Shrouded, 0, false));
                    break;
                }
            }
        }
    }
}

void Player::loadCOMusic()
{
    if (m_playerCOs[0].get() != nullptr)
    {
        m_playerCOs[0]->loadCOMusic();
    }
    if (m_playerCOs[1].get() != nullptr)
    {
        m_playerCOs[1]->loadCOMusic();
    }
    if (m_playerCOs[0].get() == nullptr &&
        m_playerCOs[0].get() == nullptr)
    {
        Mainapp* pApp = Mainapp::getInstance();
        qint32 count = GlobalUtils::randIntBase(0, 1);
        pApp->getAudioThread()->addMusic("resources/music/cos/no_co" + QString::number(count) + ".mp3", 4726, 58364);
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
    if (m_playerCOs[1].get() != nullptr)
    {
        spCO co0 = m_playerCOs[0];
        m_playerCOs[0] = m_playerCOs[1];
        m_playerCOs[1] = co0;
        spGameMenue pGameMenue = GameMenue::getInstance();
        if (pGameMenue.get() != nullptr)
        {
            pGameMenue->updatePlayerinfo();
        }
    }
}

void Player::setColor(QColor color, qint32 table)
{
    Console::print("Setting player color", Console::eDEBUG);
    m_Color = color;
    bool loaded = loadTable(table);
    if (!loaded)
    {
        createTable(m_Color);
    }
    Mainapp::getInstance()->loadResAnim(m_ColorTableAnim.get(), m_colorTable);
}

bool Player::loadTable(qint32 table)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args;
    args << table;
    QJSValue erg = pInterpreter->doFunction("PLAYER", "getColorTable", args);
    QString tablename;
    bool found = false;
    if (erg.isString())
    {
        tablename = erg.toString();
        found = loadTableFromFile(tablename);
    }
    return found;
}

bool Player::loadTableFromFile(QString tablename)
{
    bool found = false;
    QStringList searchPaths;
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        searchPaths.append(Settings::getMods().at(i) + "/images/colortables/");
    }
    searchPaths.append("resources/images/colortables/");
    for (auto & path : searchPaths)
    {
        if (QFile::exists(path + tablename + ".png"))
        {
            m_colorTable.load(path + tablename + ".png");
            found = true;
            break;
        }
    }
    return found;
}

bool Player::colorToTable(QColor baseColor)
{
    bool found = false;
    if (baseColor == QColor("#ff5a00") ||
        baseColor == QColor("#f85800"))
    {
        loadTableFromFile("orange_star");
        found = true;
    }
    else if (baseColor == QColor("#0068e8"))
    {
        loadTableFromFile("blue_moon");
        found = true;
    }
    else if (baseColor == QColor("#00c010"))
    {
        loadTableFromFile("green_earth");
        found = true;
    }
    else if (baseColor == QColor("#f8c000"))
    {
        loadTableFromFile("yellow_comet");
        found = true;
    }
    else if (baseColor == QColor("#5f11b7") ||
             baseColor == QColor("#6038a0"))
    {
        loadTableFromFile("black_hole");
        found = true;
    }
    else if (baseColor == QColor("#2d2dd5") ||
             baseColor == QColor("#483d8b") ||
             baseColor == QColor("#5c5c5c"))
    {
        loadTableFromFile("bolt_guard");
        found = true;
    }
    else if (baseColor == QColor("lightsteelblue"))
    {
        loadTableFromFile("metal_army");
        found = true;
    }
    else if (baseColor == QColor("coral"))
    {
        loadTableFromFile("amber_corona");
        found = true;
    }
    else if (baseColor == QColor("peru"))
    {
        loadTableFromFile("brown_desert");
        found = true;
    }
    else if (baseColor == QColor("goldenrod") ||
             baseColor == QColor("#808000"))
    {
        loadTableFromFile("golden_sun");
        found = true;
    }
    else if (baseColor == QColor("magenta"))
    {
        loadTableFromFile("pink_frontier");
        found = true;
    }
    else if (baseColor == QColor("teal"))
    {
        loadTableFromFile("teal_isle");
        found = true;
    }
    else if (baseColor == QColor("purple"))
    {
        loadTableFromFile("dark_matter");
        found = true;
    }
    else if (baseColor == QColor("#cyan") ||
             baseColor == QColor("#00ffff"))
    {
        loadTableFromFile("cyan");
        found = true;
    }
    else if (baseColor == QColor("#00FF00"))
    {
        loadTableFromFile("dark_green");
        found = true;
    }
    else if (baseColor == QColor("#FF0000"))
    {
        loadTableFromFile("red");
        found = true;
    }
    else if (baseColor == QColor("firebrick"))
    {
        loadTableFromFile("red_fire");
        found = true;
    }
    else if (baseColor == QColor("#FFFF00"))
    {
        loadTableFromFile("light_grey");
        found = true;
    }
    else if (baseColor == QColor("#olive"))
    {
        loadTableFromFile("olive");
        found = true;
    }
    else if (baseColor == QColor("#0000FF"))
    {
        loadTableFromFile("cobalt_ice");
        found = true;
    }
    else if (baseColor == QColor("silver"))
    {
        loadTableFromFile("silver");
        found = true;
    }
    return found;
}

void Player::createTable(QColor baseColor)
{
    m_colorTable = QImage(256, 1, QImage::Format_RGBA8888);
    m_colorTable.fill(Qt::black);
    m_colorTable.setPixelColor(0, 0, QColor(0,0, 0, 0));
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue erg = pInterpreter->doFunction("PLAYER", "getColorTableCount");
    qint32 size = 0;
    if (erg.isNumber())
    {
        size = erg.toInt();
    }
    for (qint32 i = 0; i < size; i++)
    {
        QJSValueList args;
        args << i;
        QJSValue erg = pInterpreter->doFunction("PLAYER", "getColorForTable", args);
        qint32 value = 100;
        if (erg.isNumber())
        {
            value = erg.toInt();
        }
        QColor color;
        if (value >= 100)
        {
            color = baseColor.lighter(value);
        }
        else
        {
            color = baseColor.darker(200 - value);
        }
        m_colorTable.setPixelColor(1 + i * 3, 0, color);
        m_colorTable.setPixelColor(2 + i * 3, 0, color);
        m_colorTable.setPixelColor(3 + i * 3, 0, color);
    }
}

void Player::setPlayerArmy(const QString &value)
{
    m_playerArmy = value;
}

bool Player::getPlayerArmySelected() const
{
    return m_playerArmySelected;
}

void Player::setPlayerArmySelected(bool playerArmySelected)
{
    m_playerArmySelected = playerArmySelected;
}

oxygine::spResAnim Player::getColorTableAnim() const
{
    return m_ColorTableAnim;
}

oxygine::spResAnim Player::getNeutralTableAnim()
{
    if (m_neutralTableAnim.get() == nullptr)
    {
        m_neutralTableAnim = oxygine::spSingleResAnim::create();
        QStringList searchPaths;
        for (qint32 i = 0; i < Settings::getMods().size(); i++)
        {
            searchPaths.append(Settings::getMods().at(i) + "/images/colortables/");
        }
        searchPaths.append("resources/images/colortables/");
        for (auto & path : searchPaths)
        {
            if (QFile::exists(path + "neutral.png"))
            {
                QImage img(path + "neutral.png");
                Mainapp::getInstance()->loadResAnim(m_neutralTableAnim.get(), img);
                break;
            }
        }
    }
    return m_neutralTableAnim;
}

qint32 Player::getPlayerID() const
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            if (pMap->getPlayer(i) == this)
            {
                return i;
            }
        }
    }
    return 0;
}

bool Player::getFlipUnitSprites() const
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        if (pMap->getGameRules()->getTeamFacingUnits())
        {
            return !GlobalUtils::isEven(m_team);
        }
        else
        {
            for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
            {
                if (pMap->getPlayer(i) == this)
                {
                    return !GlobalUtils::isEven(i);
                }
            }
        }
    }
    return false;
}

QString Player::getArmy()
{
    if (!m_playerArmy.isEmpty())
    {
        return m_playerArmy;
    }
    else
    {
        // editor menu mode
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args;
        QJSValue objArg = pInterpreter->newQObject(this);
        args << objArg;
        QJSValue ret = pInterpreter->doFunction("PLAYER", "getDefaultArmy", args);
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
            (m_team == pPlayer->getTeam()))
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

bool Player::isAlly(Player* pOwner)
{
    return (checkAlliance(pOwner) == GameEnums::Alliance_Friend);
}


void Player::setFunds(const qint32 &value)
{
    m_funds = value;
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        pGameMenue->updatePlayerinfo();
    }
}

void Player::addFunds(const qint32 &value)
{
    setFunds(m_funds + value);
}

qint32 Player::getFunds() const
{
    return m_funds;
}

qint32 Player::getBuildingCount(QString buildingID)
{
    qint32 ret = 0;
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
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
                            if (pBuilding->Building::getX() == x && pBuilding->Building::getY() == y)
                            {
                                ret++;
                            }
                        }
                    }
                }
            }
        }
    }
    return ret;
}

qint32 Player::getBuildingListCount(QStringList list, bool whitelist)
{
    qint32 ret = 0;
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        for (qint32 y = 0; y < pMap->getMapHeight(); y++)
        {
            for (qint32 x = 0; x < pMap->getMapWidth(); x++)
            {
                spBuilding pBuilding = pMap->getSpTerrain(x, y)->getSpBuilding();
                if (pBuilding.get() != nullptr)
                {
                    if (pBuilding->getOwner() == this)
                    {
                        QString id = pBuilding->getBuildingID();
                        if (list.isEmpty() ||
                            (list.contains(id) && whitelist) ||
                            (!list.contains(id) && !whitelist))
                        {
                            if (pBuilding->Building::getX() == x && pBuilding->Building::getY() == y)
                            {
                                ret++;
                            }
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
    spGameMap pMap = GameMap::getInstance();
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
                    ret += getUnitCount(pUnit.get(), unitID);
                }
            }
        }
    }
    return ret;
}

qint32 Player::getUnitCount(Unit* pUnit, QString unitID)
{
    qint32 ret = 0;
    for (qint32 i = 0; i < pUnit->getLoadedUnitCount(); i++)
    {
        Unit* pLoadedUnit = pUnit->getLoadedUnit(i);
        if (pLoadedUnit->getUnitID() == unitID)
        {
            ret++;
        }
        ret += getUnitCount(pLoadedUnit, unitID);
    }
    return ret;
}

qint32 Player::getTeam() const
{
    return m_team;
}

void Player::setTeam(const qint32 &value)
{
    m_team = value;
}

void Player::defeatPlayer(Player* pPlayer, bool units)
{
    spGameMap pMap = GameMap::getInstance();
    QVector<GameAnimation*> pAnimations;
    qint32 counter = 0;
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
                    pBuilding->setOwner(pPlayer);
                    // reset capturing for buildings we earned at this moment
                    if (pUnit.get() != nullptr &&
                        pUnit->getOwner()->isAlly(pPlayer))
                    {
                        pUnit->setCapturePoints(0);
                    }
                }
            }


        }
    }
    spQmlVectorUnit pUnits = getUnits();
    for (qint32 i = 0; i < pUnits->size(); ++i)
    {
        Unit* pUnit = pUnits->at(i);
        if ((pPlayer != nullptr) && units)
        {
            pUnit->setOwner(pPlayer);
            pUnit->setCapturePoints(0);
            if (pUnit->getUnitRank() < GameEnums::UnitRank_None)
            {
                pUnit->setUnitRank(pUnit->getMaxUnitRang());
            }
        }
        else
        {
            auto* pAnimation = pUnit->killUnit();
            if (pAnimations.length() < 5)
            {
                pAnimations.append(pAnimation);
            }
            else
            {
                pAnimations[counter]->queueAnimation(pAnimation);
                pAnimations[counter] = pAnimation;
                counter++;
                if (counter >= pAnimations.length())
                {
                    counter = 0;
                }
            }
        }
    }
    m_isDefeated = true;
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        pGameMenue->updatePlayerinfo();
    }
}

bool Player::getIsDefeated() const
{
    return m_isDefeated;
}

qint32 Player::getIncomeReduction(Building* pBuilding, qint32 income)
{
    qint32 reduction = 0;
    if (!m_isDefeated)
    {
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
    }
    return reduction;
}

void Player::postAction(GameAction* pAction)
{
    if (!m_isDefeated)
    {
        CO* pCO = getCO(0);
        if (pCO != nullptr)
        {
            pCO->postAction(pAction);
        }
        pCO = getCO(1);
        if (pCO != nullptr)
        {
            pCO->postAction(pAction);
        }
    }
}

qint32 Player::calcIncome(float modifier)
{
    qint32 ret = 0;
    spGameMap pMap = GameMap::getInstance();
    for (qint32 y = 0; y < pMap->getMapHeight(); y++)
    {
        for (qint32 x = 0; x < pMap->getMapWidth(); x++)
        {
            spBuilding pBuilding = pMap->getSpTerrain(x, y)->getSpBuilding();
            if (pBuilding.get() != nullptr)
            {
                if (pBuilding->getOwner() == this)
                {
                    ret += pBuilding->getIncome() * modifier;
                }
            }
        }
    }

    return ret;
}

qint32 Player::calcArmyValue()
{
    spQmlVectorUnit pUnits = GameMap::getInstance()->getUnits(this);
    qint32 armyValue = 0;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        armyValue += pUnits->at(i)->getUnitValue();
    }
    return armyValue;
}

void Player::earnMoney(float modifier)
{
    setFunds(m_funds + calcIncome(modifier));
}

qint32 Player::getCostModifier(QString id, qint32 baseCost)
{
    qint32 costModifier = 0;
    if (m_playerCOs[0].get() != nullptr)
    {
        costModifier += m_playerCOs[0]->getCostModifier(id, baseCost);
    }
    if (m_playerCOs[1].get() != nullptr)
    {
        costModifier += m_playerCOs[1]->getCostModifier(id, baseCost);
    }
    return costModifier;
}

void Player::postBattleActions(Unit* pAttacker, float atkDamage, Unit* pDefender, bool gotAttacked, qint32 weapon)
{
    if (!m_isDefeated)
    {
        if (m_playerCOs[0].get() != nullptr)
        {
            m_playerCOs[0]->postBattleActions(pAttacker, atkDamage, pDefender, gotAttacked, weapon);
        }
        if (m_playerCOs[1].get() != nullptr)
        {
            m_playerCOs[1]->postBattleActions(pAttacker, atkDamage, pDefender, gotAttacked, weapon);
        }
    }
}

void Player::buildedUnit(Unit* pUnit)
{
    if (m_playerCOs[0].get() != nullptr)
    {
        m_playerCOs[0]->buildedUnit(pUnit);
    }
    if (m_playerCOs[1].get() != nullptr)
    {
        m_playerCOs[1]->buildedUnit(pUnit);
    }
}

bool Player::getWeatherImmune()
{
    if ((m_playerCOs[0].get() != nullptr &&
         m_playerCOs[0]->getWeatherImmune()) ||
        (m_playerCOs[1].get() != nullptr &&
         m_playerCOs[1]->getWeatherImmune()))
    {
        return true;
    }
    return false;
}

QStringList Player::getBuildList() const
{
    return m_BuildList;
}

QStringList Player::getCOUnits(Building* pBuilding)
{
    QStringList ret;
    if (m_playerCOs[0].get() != nullptr)
    {
        ret.append(m_playerCOs[0]->getCOUnits(pBuilding));
    }
    if (m_playerCOs[1].get() != nullptr)
    {
        ret.append(m_playerCOs[1]->getCOUnits(pBuilding));
    }
    return ret;
}

void Player::setBuildList(const QStringList BuildList)
{
    m_BuildList = BuildList;
    m_BuildlistChanged = true;
}

void Player::changeBuildlist(const QString unitID, bool remove)
{
    if (remove)
    {
        m_BuildList.removeAll(unitID);
    }
    else
    {
        if (!m_BuildList.contains(unitID))
        {
            m_BuildList.append(unitID);
        }
    }
}

quint64 Player::getSocketId() const
{
    return m_socketId;
}

void Player::setSocketId(const quint64 &socketId)
{
    m_socketId = socketId;
}

bool Player::getBuildlistChanged() const
{
    return m_BuildlistChanged;
}

void Player::setBuildlistChanged(bool BuildlistChanged)
{
    m_BuildlistChanged = BuildlistChanged;
}

void Player::setIsDefeated(bool value)
{
    m_isDefeated = value;
}

void Player::addVisionField(qint32 x, qint32 y, qint32 duration, bool directView)
{
    addVisionFieldInternal(x, y,duration, directView);
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = pMap->getPlayer(i);
        if (isAlly(pPlayer))
        {
            pPlayer->addVisionFieldInternal(x, y, duration - 1, directView);
        }
    }
}

void Player::addVisionFieldInternal(qint32 x, qint32 y, qint32 duration, bool directView)
{
    std::get<0>(m_FogVisionFields[x][y]) = GameEnums::VisionType_Clear;
    if (duration > std::get<1>(m_FogVisionFields[x][y]))
    {
        std::get<1>(m_FogVisionFields[x][y]) = duration;
    }
    if (std::get<2>(m_FogVisionFields[x][y]) == false)
    {
        std::get<2>(m_FogVisionFields[x][y]) = directView;
    }
}

void Player::updatePlayerVision(bool reduceTimer)
{
    spGameMap pMap = GameMap::getInstance();
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
                    std::get<1>(m_FogVisionFields[x][y]) -= 1;
                }
                qint32 duration = std::get<1>(m_FogVisionFields[x][y]);
                if (duration <= 0)
                {
                    if (std::get<0>(m_FogVisionFields[x][y]) == GameEnums::VisionType_Clear)
                    {
                        std::get<0>(m_FogVisionFields[x][y]) = GameEnums::VisionType_Fogged;
                    }
                    std::get<1>(m_FogVisionFields[x][y]) = 0;
                    std::get<2>(m_FogVisionFields[x][y]) = false;
                }
            }
        }
        bool visionBlock = pMap->getGameRules()->getVisionBlock();
        // create vision for all units and terrain
        for (qint32 x = 0; x < width; x++)
        {
            for (qint32 y = 0; y < heigth; y++)
            {
                // check terrain vision
                Terrain* pTerrain = pMap->getTerrain(x, y);
                qint32 visionRange = pTerrain->getVision(this);
                if (visionRange >= 0)
                {
                    spQmlVectorPoint pPoints;
                    if (visionBlock)
                    {
                        pPoints = pMap->getVisionCircle(x, y, 0, visionRange, pTerrain->getTotalVisionHigh());
                    }
                    else
                    {
                        pPoints = GlobalUtils::getCircle(0, visionRange);
                    }
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
                                 !pUnit->useTerrainDefense() && !pUnit->isStatusStealthed()))
                            {
                                std::get<0>(m_FogVisionFields[point.x() + x][point.y() + y]) = GameEnums::VisionType_Clear;
                            }
                        }
                    }
                }
                // check building vision
                Building* pBuilding = pTerrain->getBuilding();
                if ((pBuilding != nullptr) &&
                    ((isAlly( pBuilding->getOwner())) ||
                     (checkAlliance(pBuilding->getOwner()) == GameEnums::Alliance_Friend)))
                {
                    std::get<0>(m_FogVisionFields[x][y]) = GameEnums::VisionType_Clear;
                    qint32 visionRange = pBuilding->getVision();
                    if (visionRange >= 0)
                    {
                        spQmlVectorPoint pPoints;
                        if (visionBlock)
                        {
                            pPoints = pMap->getVisionCircle(x, y, 0, visionRange, pBuilding->getTotalVisionHigh());
                        }
                        else
                        {
                            pPoints = GlobalUtils::getCircle(0, visionRange);
                        }
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
                                     !pUnit->useTerrainDefense() && !pUnit->isStatusStealthed()))
                                {
                                    std::get<0>(m_FogVisionFields[point.x() + x][point.y() + y]) = GameEnums::VisionType_Clear;
                                }
                            }
                        }
                    }
                }
                // create unit vision
                Unit* pUnit = pTerrain->getUnit();
                if ((pUnit != nullptr) &&
                    (isAlly(pUnit->getOwner())))
                {
                    qint32 visionRange = pUnit->getVision(QPoint(x, y));
                    spQmlVectorPoint pPoints;
                    if (visionBlock)
                    {
                        if (pBuilding != nullptr)
                        {
                            pPoints = pMap->getVisionCircle(x, y, 0, visionRange,  pUnit->getTotalVisionHigh());
                        }
                        else
                        {
                            pPoints = pMap->getVisionCircle(x, y, 0, visionRange,  pUnit->getVisionHigh() + pTerrain->getVisionHigh());
                        }
                    }
                    else
                    {
                        pPoints = GlobalUtils::getCircle(0, visionRange);
                    }
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
                                 !pUnit->useTerrainDefense() && !pUnit->isStatusStealthed()))
                            {
                                std::get<0>(m_FogVisionFields[point.x() + x][point.y() + y]) = GameEnums::VisionType_Clear;
                            }
                            // terrain hides are visible if we're near it.
                            else if (((qAbs(point.x()) + qAbs(point.y())) <= 1))
                            {
                                std::get<0>(m_FogVisionFields[point.x() + x][point.y() + y]) = GameEnums::VisionType_Clear;
                            }
                            else
                            {
                                // do nothing
                            }
                        }
                    }
                }
            }
        }
    }
}

bool Player::getFieldVisible(qint32 x, qint32 y)
{
    spGameMap pMap = GameMap::getInstance();
    switch (pMap->getGameRules()->getFogMode())
    {
        case GameEnums::Fog_Off:
        {
            return true;
        }
        case GameEnums::Fog_OfShroud:
        case GameEnums::Fog_OfWar:
        {
            if (m_FogVisionFields.size() > 0)
            {
                return (std::get<0>(m_FogVisionFields[x][y]) == GameEnums::VisionType_Clear);
            }
            else
            {
                return true;
            }
        }
    }
    return false;
}

GameEnums::VisionType Player::getFieldVisibleType(qint32 x, qint32 y)
{
    spGameMap pMap = GameMap::getInstance();
    switch (pMap->getGameRules()->getFogMode())
    {
        case GameEnums::Fog_Off:
        {
            return GameEnums::VisionType_Clear;
        }
        case GameEnums::Fog_OfShroud:
        case GameEnums::Fog_OfWar:
        {
            if (m_FogVisionFields.size() > 0)
            {
                if (pMap->onMap(x, y) && m_FogVisionFields.size() > 0)
                {
                    return std::get<0>(m_FogVisionFields[x][y]);
                }
                return GameEnums::VisionType_Shrouded;
            }
            else
            {
                return GameEnums::VisionType_Clear;
            }
        }
    }
    return GameEnums::VisionType_Fogged;
}

bool Player::getFieldDirectVisible(qint32 x, qint32 y)
{
    if (m_FogVisionFields.size() > 0)
    {
        return std::get<2>(m_FogVisionFields[x][y]);
    }
    else
    {
        return true;
    }
}

qint32 Player::getCosts(QString id)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue ret = pInterpreter->doFunction(id, "getBaseCost");
    qint32 costs = 0;
    if (ret.isNumber())
    {
        costs = ret.toInt();
    }
    costs += getCostModifier(id, costs);
    return costs;
}

void Player::gainPowerstar(qint32 fundsDamage, QPoint position, qint32 hpDamage, bool defender, bool counterAttack)
{
    float speed = GameMap::getInstance()->getGameRules()->getPowerGainSpeed();
    if (m_playerCOs[0].get() != nullptr)
    {
        m_playerCOs[0]->gainPowerstar(fundsDamage * speed, position, hpDamage * speed, defender, counterAttack);
    }
    if (m_playerCOs[1].get() != nullptr)
    {
        m_playerCOs[1]->gainPowerstar(fundsDamage * speed, position, hpDamage * speed, defender, counterAttack);
    }
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        pGameMenue->updatePlayerinfo();
    }
}

qint32 Player::getMovementcostModifier(Unit* pUnit, QPoint position)
{
    qint32 modifier = 0;
    if (m_playerCOs[0].get() != nullptr)
    {
        modifier += m_playerCOs[0]->getMovementcostModifier(pUnit, position);
    }
    if (m_playerCOs[1].get() != nullptr)
    {
        modifier += m_playerCOs[1]->getMovementcostModifier(pUnit, position);
    }
    if (pUnit->getOwner() == this)
    {
        spGameMap pMap = GameMap::getInstance();
        if (!getWeatherImmune())
        {
            modifier += pMap->getGameRules()->getCurrentWeather()->getMovementCostModifier(pUnit, pMap->getTerrain(position.x(), position.y()));
        }
    }
    return modifier;
}

qint32 Player::getBonusMovementpoints(Unit* pUnit, QPoint position)
{
    qint32 movementModifier = 0;
    CO* pCO = getCO(0);
    if (pCO != nullptr)
    {
        movementModifier += pCO->getMovementpointModifier(pUnit, position);
    }
    pCO = getCO(1);
    if (pCO != nullptr)
    {
        movementModifier += pCO->getMovementpointModifier(pUnit, position);
    }
    if (pUnit->getOwner() == this)
    {
        spGameMap pMap = GameMap::getInstance();
        if (!getWeatherImmune())
        {
            movementModifier += pMap->getGameRules()->getCurrentWeather()->getMovementpointModifier(pUnit, pMap->getTerrain(position.x(), position.y()));
        }
    }
    return movementModifier;
}

void Player::startOfTurn()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "startOfTurn";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    pInterpreter->doFunction("PLAYER", function1, args1);

    if (m_playerCOs[0].get() != nullptr)
    {
        m_playerCOs[0]->setPowerMode(GameEnums::PowerMode_Off);
        m_playerCOs[0]->setCoRangeEnabled(true);
        m_playerCOs[0]->startOfTurn();
    }
    if (m_playerCOs[1].get() != nullptr)
    {
        m_playerCOs[1]->setPowerMode(GameEnums::PowerMode_Off);
        m_playerCOs[1]->setCoRangeEnabled(true);
        m_playerCOs[1]->startOfTurn();
    }
}

QmlVectorUnit* Player::getUnits()
{
    if (GameMap::getInstance() != nullptr)
    {
        return GameMap::getInstance()->getUnits(this);
    }
    else
    {
        return new QmlVectorUnit();
    }
}

qint32 Player::getEnemyCount()
{
    spGameMap pMap = GameMap::getInstance();
    qint32 ret = 0;
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        if (isEnemy(pMap->getPlayer(i)))
        {
            ret++;
        }
    }
    return ret;
}

QmlVectorUnit* Player::getEnemyUnits()
{
    spGameMap pMap = GameMap::getInstance();
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

QVector<spUnit> Player::getSpEnemyUnits()
{
    spGameMap pMap = GameMap::getInstance();
    qint32 heigth = pMap->getMapHeight();
    qint32 width = pMap->getMapWidth();
    QVector<spUnit> ret;
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            spUnit pUnit = pMap->getTerrain(x, y)->getSpUnit();
            if (pUnit.get() != nullptr &&
                !pUnit->getOwner()->getIsDefeated() &&
                isEnemyUnit(pUnit.get()))
            {
                ret.append(pUnit);
            }
        }
    }
    return ret;
}

QmlVectorBuilding* Player::getEnemyBuildings()
{
    spGameMap pMap = GameMap::getInstance();
    qint32 heigth = pMap->getMapHeight();
    qint32 width = pMap->getMapWidth();
    QmlVectorBuilding* ret = new QmlVectorBuilding();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
            if (pBuilding != nullptr &&
                pBuilding->getTerrain() == pMap->getTerrain(x, y) &&
                pBuilding->isEnemyBuilding(this))
            {
                ret->append(pBuilding);
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
    spCO pCo = m_playerCOs[0];
    if (pCo.get() != nullptr)
    {
        spUnit pCoUnit = pCo->getCOUnit();
        if (pCoUnit.get() != nullptr)
        {
            if (pCo->getPowerMode() == GameEnums::PowerMode_Off)
            {
                pCoUnit->createCORange(pCo->getCORange());
            }
            else
            {
                pCoUnit->createCORange(-1);
            }
        }
    }
    pCo = m_playerCOs[1];
    if (pCo.get() != nullptr)
    {
        spUnit pCoUnit = pCo->getCOUnit();
        if (pCoUnit.get() != nullptr)
        {
            if (pCo->getPowerMode() == GameEnums::PowerMode_Off)
            {
                pCoUnit->createCORange(pCo->getCORange());
            }
            else
            {
                pCoUnit->createCORange(-1);
            }
        }
    }
}

void Player::setBaseGameInput(spBaseGameInputIF pBaseGameInput)
{
    m_pBaseGameInput = pBaseGameInput;
    if (m_pBaseGameInput.get() != nullptr)
    {
        m_pBaseGameInput->setPlayer(this);
    }
}

spCO Player::getspCO(quint8 id)
{
    if (id <= 1)
    {
        return m_playerCOs[id];
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
        return m_playerCOs[id].get();
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
            m_playerCOs[idx] = nullptr;
        }
        else
        {
            m_playerCOs[idx] = spCO::create(coId, this);
        }

    }
}

QPoint Player::getRockettarget(qint32 radius, qint32 damage, float ownUnitValue, GameEnums::RocketTarget targetType)
{
    spGameMap pMap = GameMap::getInstance();
    spQmlVectorPoint pPoints = GlobalUtils::getCircle(0, radius);
    qint32 highestDamage = -1;
    QVector<QPoint> targets;

    for (qint32 x = 0; x < pMap->getMapWidth(); x++)
    {
        for (qint32 y = 0; y < pMap->getMapHeight(); y++)
        {
            qint32 damageDone = getRocketTargetDamage(x, y, pPoints.get(), damage, ownUnitValue, targetType, true);
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

    if (targets.size() >= 0)
    {
        return targets[GlobalUtils::randInt(0, targets.size() - 1)];
    }
    else
    {
        return QPoint(-1, -1);
    }
}

QPoint Player::getSiloRockettarget(qint32 radius, qint32 damage, qint32 & highestDamage, float ownUnitValue, GameEnums::RocketTarget targetType)
{
    spGameMap pMap = GameMap::getInstance();
    spQmlVectorPoint pPoints = GlobalUtils::getCircle(0, radius);
    highestDamage = -1;
    QVector<QPoint> targets;

    for (qint32 x = 0; x < pMap->getMapWidth(); x++)
    {
        for (qint32 y = 0; y < pMap->getMapHeight(); y++)
        {
            qint32 damageDone = getRocketTargetDamage(x, y, pPoints.get(), damage, ownUnitValue, targetType, false);
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

    if (targets.size() >= 0)
    {
        return targets[GlobalUtils::randInt(0, targets.size() - 1)];
    }
    else
    {
        return QPoint(-1, -1);
    }
}

qint32 Player::getAverageCost()
{
    if (m_averageCosts < 0)
    {
        UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
        m_averageCosts = 0;
        Interpreter* pInterpreter = Interpreter::getInstance();
        for (qint32 i = 0; i < pUnitSpriteManager->getCount(); i++)
        {
            QString unitId = pUnitSpriteManager->getID(i);
            QString function1 = "getBaseCost";
            QJSValue erg = pInterpreter->doFunction(unitId, function1);
            if (erg.isNumber())
            {
                m_averageCosts += erg.toInt();
            }
        }
        m_averageCosts = m_averageCosts / pUnitSpriteManager->getCount();
    }
    return m_averageCosts;
}

qint32 Player::getRocketTargetDamage(qint32 x, qint32 y, QmlVectorPoint* pPoints, qint32 damage, float ownUnitValue, GameEnums::RocketTarget targetType, bool ignoreStealthed)
{
    qint32 averageCosts = getAverageCost();
    spGameMap pMap = GameMap::getInstance();
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
            if (!pUnit->isStealthed(this) || ignoreStealthed)
            {
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
                        // calc funds damage
                        damageDone += damagePoints / Unit::MAX_UNIT_HP * modifier * pUnit->getCosts();
                        break;
                    }
                    case GameEnums::RocketTarget_HpHighMoney:
                    {
                        // calc funds damage
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
                        // calc funds damage
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
    }
    return damageDone;
}

void Player::defineArmy()
{
    if (m_playerCOs[0].get() != nullptr && !m_playerArmySelected)
    {
        m_playerArmy = m_playerCOs[0]->getCOArmy();
    }
}

float Player::getFundsModifier() const
{
    return m_fundsModifier;
}

void Player::setFundsModifier(float value)
{
    m_fundsModifier = value;
}

qint32 Player::calculatePlayerStrength()
{
    qint32 ret = 0;
    spGameMap pMap = GameMap::getInstance();
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

void Player::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    quint32 color = m_Color.rgb();
    pStream << color;

    pStream << m_funds;
    pStream << m_fundsModifier;
    pStream << m_playerArmy;
    if (m_playerCOs[0].get() == nullptr)
    {
        pStream << false;
    }
    else
    {
        pStream << true;
        m_playerCOs[0]->serializeObject(pStream);
    }
    if (m_playerCOs[1].get() == nullptr)
    {
        pStream << false;
    }
    else
    {
        pStream << true;
        m_playerCOs[1]->serializeObject(pStream);
    }
    pStream << m_team;
    pStream << m_isDefeated;
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
            pStream << static_cast<qint32>(std::get<0>(m_FogVisionFields[x][y]));
            pStream << std::get<1>(m_FogVisionFields[x][y]);
            pStream << std::get<2>(m_FogVisionFields[x][y]);
        }
    }
    pStream << m_BuildList;
    pStream << m_BuildlistChanged;
    m_Variables.serializeObject(pStream);

    width = m_colorTable.width();
    pStream << width;
    for (qint32 x = 0; x < width; x++)
    {
        pStream << m_colorTable.pixel(x, 0);
    }
    pStream << m_playerArmySelected;
}

void Player::deserializeObject(QDataStream& pStream)
{
    deserializer(pStream, false);
}

void Player::deserializer(QDataStream& pStream, bool fast)
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
        pStream >> m_funds;
        pStream >> m_fundsModifier;
        pStream >> m_playerArmy;
        bool hasC0 = false;
        pStream >> hasC0;
        if (hasC0)
        {
            m_playerCOs[0] = spCO::create("", this);
            m_playerCOs[0]->deserializer(pStream, fast);
            if (!m_playerCOs[0]->isValid())
            {
                m_playerCOs[0] = nullptr;
            }
        }
        bool hasC1 = false;
        pStream >> hasC1;
        if (hasC1)
        {
            m_playerCOs[1] = spCO::create("", this);
            m_playerCOs[1]->deserializer(pStream, fast);
            if (!m_playerCOs[1]->isValid())
            {
                m_playerCOs[1] = nullptr;
            }
        }
        if (version > 3)
        {
            pStream >> m_team;
        }
        if (version > 4)
        {
            pStream >> m_isDefeated;
            m_pBaseGameInput = BaseGameInputIF::deserializeInterface(pStream, version);
            if (m_pBaseGameInput.get() != nullptr)
            {
                m_pBaseGameInput->setPlayer(this);
            }
        }
        else
        {
            m_pBaseGameInput = spHumanPlayerInput::create();
            m_pBaseGameInput->setPlayer(this);
        }
        m_FogVisionFields.clear();
        if (version > 5)
        {
            qint32 width = 0;
            qint32 heigth = 0;
            pStream >> width;
            pStream >> heigth;
            for (qint32 x = 0; x < width; x++)
            {
                m_FogVisionFields.append(QVector<std::tuple<GameEnums::VisionType, qint32, bool>>());
                for (qint32 y = 0; y < heigth; y++)
                {
                    GameEnums::VisionType value = GameEnums::VisionType_Shrouded;
                    qint32 duration = 0;
                    bool directView = false;
                    if (version > 10)
                    {
                        qint32 buf = 0;
                        pStream >> buf;
                        value = static_cast<GameEnums::VisionType>(buf);
                    }
                    else if (version > 8)
                    {
                        bool value1 = false;
                        pStream >> value1;
                        if (value1)
                        {
                            value = GameEnums::VisionType_Clear;
                        }
                        else
                        {
                            value = GameEnums::VisionType_Fogged;
                        }
                    }
                    else
                    {
                        qint32 buf = 0;
                        pStream >> buf;
                        if (buf)
                        {
                            value = GameEnums::VisionType_Clear;
                        }
                        else
                        {
                            value = GameEnums::VisionType_Fogged;
                        }
                    }
                    pStream >> duration;
                    if (version > 8)
                    {
                        pStream >> directView;
                    }
                    m_FogVisionFields[x].append(std::tuple<GameEnums::VisionType, qint32, bool>(value, duration, directView));
                }
            }
        }
    }
    m_BuildList.clear();
    if (version > 6)
    {
        pStream >> m_BuildList;
    }
    if (version > 9)
    {
        pStream >> m_BuildlistChanged;
    }
    if (!m_BuildlistChanged)
    {
        m_BuildList.clear();
        // for older versions we allow all loaded units to be buildable
        UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
        for (qint32 i = 0; i < pUnitSpriteManager->getCount(); i++)
        {
            m_BuildList.append(pUnitSpriteManager->getID(i));
        }
    }
    if (version > 11)
    {
        m_Variables.deserializeObject(pStream);
    }

    if (version <= 5)
    {
        loadVisionFields();
    }
    if (version > 12)
    {
        qint32 width = 0;
        pStream >> width;
        m_colorTable = QImage(width, 1, QImage::Format_RGBA8888);
        QRgb rgb;
        for (qint32 x = 0; x < width; x++)
        {
            pStream >> rgb;
            m_colorTable.setPixel(x, 0, rgb);
        }
        if (!fast)
        {
            Console::print("Loading colortable", Console::eDEBUG);
            Mainapp::getInstance()->loadResAnim(m_ColorTableAnim.get(), m_colorTable);
        }
    }
    else
    {
        if (!colorToTable(m_Color))
        {
            createTable(m_Color.darker(160));
        }
        m_Color = m_colorTable.pixel(8, 0);
        if (!fast)
        {
            Console::print("Loading colortable", Console::eDEBUG);
            Mainapp::getInstance()->loadResAnim(m_ColorTableAnim.get(), m_colorTable);
        }
    }
    if (version > 13)
    {
        pStream >> m_playerArmySelected;
    }

}
