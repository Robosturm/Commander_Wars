#include "coreengine/globalutils.h"
#include "coreengine/audiomanager.h"
#include "coreengine/filesupport.h"

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

oxygine::spResAnim Player::m_neutralTableAnim;
QImage Player::m_neutralTableImage;

void Player::releaseStaticData()
{
    m_neutralTableAnim.reset();
}

Player::Player(GameMap* pMap)
    : m_pMap{pMap}
{
#ifdef GRAPHICSUPPORT
    setObjectName("Player");
#endif
    Interpreter::setCppOwnerShip(this);
    m_pBaseGameInput.reset();
    // for older versions we allow all loaded units to be buildable
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    for (qint32 i = 0; i < pUnitSpriteManager->getCount(); i++)
    {
        m_BuildList.append(pUnitSpriteManager->getID(i));
    }
}

void Player::init()
{
    m_team = getPlayerID();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "loadDefaultPlayerColor";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction("PLAYER", function, args);
}

BaseGameInputIF* Player::getBaseGameInput()
{
    return m_pBaseGameInput.get();
}

QString Player::getPlayerNameId() const
{
    return m_playerNameId;
}

void Player::setPlayerNameId(const QString & newDisplayName)
{
    m_playerNameId = newDisplayName;
}

QString Player::getUniqueIdentifier() const
{
    return m_uniqueIdentifier;
}

void Player::setUniqueIdentifier(const QString & newUniqueIdentifier)
{
    m_uniqueIdentifier = newUniqueIdentifier;
}

qreal Player::getUnitBuildValue(const QString & unitID)
{
    qreal modifier = 0.0;
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            modifier += pCO->getUnitBuildValue(unitID);
        }
    }
    return modifier;
}

void Player::loadVisionFields()
{
    CONSOLE_PRINT("Player::loadVisionFields()", GameConsole::eDEBUG);
    
    qint32 width = m_pMap->getMapWidth();
    qint32 heigth = m_pMap->getMapHeight();
    GameEnums::Fog mode = m_pMap->getGameRules()->getFogMode();
    m_FogVisionFields.clear();
    m_FogVisionFields.reserve(width);
    for (qint32 x = 0; x < width; x++)
    {
        switch (mode)
        {
            case GameEnums::Fog::Fog_Off:
            {
                m_FogVisionFields.push_back(std::vector<VisionFieldInfo>(heigth, VisionFieldInfo(GameEnums::VisionType_Clear, 0, false)));
                break;
            }
            case GameEnums::Fog::Fog_OfWar:
            {
                m_FogVisionFields.push_back(std::vector<VisionFieldInfo>(heigth, VisionFieldInfo(GameEnums::VisionType_Fogged, 0, false)));
                break;
            }
            case GameEnums::Fog::Fog_OfShroud:
            {
                m_FogVisionFields.push_back(std::vector<VisionFieldInfo>(heigth, VisionFieldInfo(GameEnums::VisionType_Shrouded, 0, false)));
                break;
            }
            case GameEnums::Fog::Fog_OfMist:
            {
                m_FogVisionFields.push_back(std::vector<VisionFieldInfo>(heigth, VisionFieldInfo(GameEnums::VisionType_Mist, 0, false)));
                break;
            }
        }
    }
}

void Player::loadCOMusic()
{
    bool hasCo = false;
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            pCO->loadCOMusic();
            hasCo = true;
        }
    }
    if (!hasCo)
    {
        Mainapp* pApp = Mainapp::getInstance();
        qint32 count = GlobalUtils::randIntBase(0, 1);
        if (count == 0)
        {
            pApp->getAudioManager()->addMusic("resources/music/cos/no_co0.mp3", 4726, 58364);
        }
        else
        {
            pApp->getAudioManager()->addMusic("resources/music/cos/no_co1.mp3", 4726, 58364);
        }
    }
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
        if (m_pMenu != nullptr)
        {
            m_pMenu->updatePlayerinfo();
        }
    }
}

void Player::setColor(QColor color, qint32 table)
{
    CONSOLE_PRINT("Setting player color", GameConsole::eDEBUG);
    m_Color = color;
    bool loaded = false;
    if (table >= 0)
    {
        loaded = loadTable(table);
    }
    else
    {
        loaded = colorToTable(color);
    }
    if (!loaded)
    {
        createTable(m_Color);
    }
#ifdef GRAPHICSUPPORT
    m_ColorTableAnim = MemoryManagement::create<oxygine::SingleResAnim>();
    Mainapp::getInstance()->loadResAnim(m_ColorTableAnim, m_colorTable, 1, 1, 1);
#endif
}

bool Player::loadTable(qint32 table)
{
    CONSOLE_PRINT("Player::loadTable", GameConsole::eDEBUG);
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({table,
                      pInterpreter->newQObject(m_pMap)});
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

bool Player::loadTableFromFile(const QString tablename)
{
    CONSOLE_PRINT("Player::loadTableFromFile " + tablename, GameConsole::eDEBUG);
    bool found = false;
    QStringList searchPaths;
    for (qint32 i = 0; i < Settings::getInstance()->getMods().size(); i++)
    {
        searchPaths.append(Settings::getInstance()->getUserPath() + Settings::getInstance()->getMods().at(i) + "/images/colortables/");
        searchPaths.append(oxygine::Resource::RCC_PREFIX_PATH + Settings::getInstance()->getMods().at(i) + "/images/colortables/");
    }
    searchPaths.append("resources/images/colortables/");
    searchPaths.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/images/colortables/");
    for (auto & path : searchPaths)
    {
        if (QFile::exists(path + tablename + ".png"))
        {
#ifdef GRAPHICSUPPORT
            m_colorTable.load(path + tablename + ".png");
            if (m_colorTable.height() > 0)
            {
                found = true;
            }
#else
            found = true;
#endif
            break;
        }
    }
    return found;
}

bool Player::colorToTable(QColor baseColor)
{
    CONSOLE_PRINT("Player::colorToTable", GameConsole::eDEBUG);
    bool found = colorToTableInTable(baseColor);
    if (!found)
    {
        if (baseColor == QColor(0xff, 0x5a, 0x00) ||
            baseColor == QColor(0xf8, 0x58, 0x00) ||
            baseColor == QColor(0xf0, 0x00, 0x08))
        {
            loadTableFromFile("orange_star");
            found = true;
        }
        else if (baseColor == QColor(0x00, 0x68, 0xe8) ||
                 baseColor == QColor(0x00, 0x98, 0xf8))
        {
            loadTableFromFile("blue_moon");
            found = true;
        }
        else if (baseColor == QColor(0x00, 0xc0, 0x10) ||
                 baseColor == QColor(0x00, 0xc0, 0x10))
        {
            loadTableFromFile("green_earth");
            found = true;
        }
        else if (baseColor == QColor(0xf8, 0xc0, 0x00) ||
                 baseColor == QColor(0xd0, 0x80, 0x00))
        {
            loadTableFromFile("yellow_comet");
            found = true;
        }
        else if (baseColor == QColor(0x5f, 0x11, 0xb7) ||
                 baseColor == QColor(0x60, 0x38, 0xa0))
        {
            loadTableFromFile("black_hole");
            found = true;
        }
        else if (baseColor == QColor(0x2d, 0x2d, 0xd5) ||
                 baseColor == QColor(0x48, 0x3d, 0x8b) ||
                 baseColor == QColor(0x5c, 0x5c, 0x5c) ||
                 baseColor == QColor(0x5c, 0x56, 0x63))
        {
            loadTableFromFile("bolt_guard");
            found = true;
        }
        else if (baseColor == QColor("lightsteelblue") ||
                 baseColor == QColor(0x79, 0x7b, 0x78))
        {
            loadTableFromFile("metal_army");
            found = true;
        }
        else if (baseColor == QColor("coral") ||
                 baseColor == QColor(0xe8, 0x86, 0x13))
        {
            loadTableFromFile("amber_corona");
            found = true;
        }
        else if (baseColor == QColor("peru") ||
                 baseColor == QColor(0xbc, 0x82, 0x48))
        {
            loadTableFromFile("brown_desert");
            found = true;
        }
        else if (baseColor == QColor("goldenrod") ||
                 baseColor == QColor(0x80, 0x80, 0x00) ||
                 baseColor == QColor(0xbf, 0x90, 0x1c))
        {
            loadTableFromFile("golden_sun");
            found = true;
        }
        else if (baseColor == QColor("magenta") ||
                 baseColor == QColor(0xff, 0x33, 0xcc))
        {
            loadTableFromFile("pink_frontier");
            found = true;
        }
        else if (baseColor == QColor("teal") ||
                 baseColor == QColor(0x17, 0xa1, 0x95))
        {
            loadTableFromFile("teal_isle");
            found = true;
        }
        else if (baseColor == QColor("purple") ||
                 baseColor == QColor(0x80, 0x00, 0x80))
        {
            loadTableFromFile("dark_matter");
            found = true;
        }
        else if (baseColor == QColor("#cyan") ||
                 baseColor == QColor(0x00, 0xff, 0xff) ||
                 baseColor == QColor(0x01, 0xcb, 0xff))
        {
            loadTableFromFile("cyan");
            found = true;
        }
        else if (baseColor == QColor(0x00, 0xFF, 0x00) ||
                 baseColor == QColor(0x00, 0x64, 0x00))
        {
            loadTableFromFile("dark_green");
            found = true;
        }
        else if (baseColor == QColor(0xFF, 0x00, 0x00))
        {
            loadTableFromFile("red");
            found = true;
        }
        else if (baseColor == QColor("firebrick") ||
                 baseColor == QColor(0xc4, 0x44, 0x3d))
        {
            loadTableFromFile("red_fire");
            found = true;
        }
        else if (baseColor == QColor(0xFF, 0xFF, 0x00) ||
                 baseColor == QColor(0xa2, 0x9d, 0xb9))
        {
            loadTableFromFile("light_grey");
            found = true;
        }
        else if (baseColor == QColor("#olive") ||
                 baseColor == QColor(0x61, 0x7c, 0x0e))
        {
            loadTableFromFile("olive");
            found = true;
        }
        else if (baseColor == QColor(0x00, 0x00, 0xFF) ||
                 baseColor == QColor(0x23, 0x42, 0xba))
        {
            loadTableFromFile("cobalt_ice");
            found = true;
        }
        else if (baseColor == QColor("silver") ||
                 baseColor == QColor(0x85, 0x92, 0x7b))
        {
            loadTableFromFile("silver");
            found = true;
        }
    }
    return found;
}

bool Player::colorToTableInTable(QColor baseColor)
{
    CONSOLE_PRINT("Player::colorToTableInTable", GameConsole::eDEBUG);
    bool found = false;
    QStringList searchPaths;
    for (qint32 i = 0; i < Settings::getInstance()->getMods().size(); i++)
    {
        searchPaths.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + Settings::getInstance()->getMods().at(i) + "/images/colortables/");
        searchPaths.append(Settings::getInstance()->getUserPath() + Settings::getInstance()->getMods().at(i) + "/images/colortables/");
    }
    searchPaths.append(Settings::getInstance()->getUserPath() + "resources/images/colortables/");
    searchPaths.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/images/colortables/");
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString path = searchPaths[i];
        QStringList filter;
        filter << "*.png";
        QDirIterator dirIter(path, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter.hasNext())
        {
            dirIter.next();
            QString path = dirIter.fileInfo().filePath();
            QImage img(path);
            if (img.width() > 255 &&
                img.height() > 255 &&
                QColor(img.pixel(255, 255)) == baseColor)
            {
                CONSOLE_PRINT("load table " + path, GameConsole::eDEBUG);
#ifdef GRAPHICSUPPORT
                m_colorTable.load(path);
                if (m_colorTable.height() > 0)
                {
                    found = true;
                }
#else
                found = true;
#endif
            }
            if (found)
            {
                break;
            }
        }
        if (found)
        {
            break;
        }
    }
    return found;
}

void Player::createTable(QColor baseColor)
{
#ifdef GRAPHICSUPPORT
    CONSOLE_PRINT("Player::createTable " + baseColor.name(), GameConsole::eDEBUG);
    constexpr qint32 imageSize = 256;
    m_colorTable = QImage(imageSize, imageSize, QImage::Format_RGBA8888);
    m_colorTable.fill(QColor(0, 0, 0, 0));
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue erg = pInterpreter->doFunction("PLAYER", "getColorTableCount");
    qint32 size = 0;
    if (erg.isNumber())
    {
        size = erg.toInt();
    }
    for (qint32 i = 0; i < size; i++)
    {
        QJSValueList args({i,
                           pInterpreter->newQObject(m_pMap)});
        QJSValue erg = pInterpreter->doFunction("PLAYER", "getColorForTable", args);
        qint32 value = 100;
        QColor color;
        if (erg.isNumber())
        {
            value = erg.toInt();
            if (value >= 100)
            {
                color = baseColor.lighter(value);
            }
            else
            {
                color = baseColor.darker(200 - value);
            }
        }
        else if (erg.isString())
        {
            color = QColor(erg.toString());
        }


        erg = pInterpreter->doFunction("PLAYER", "getPositionForTable", args);

        QPoint pos = erg.toVariant().toPoint();
        for (qint32 x = -1; x <= 1; ++x)
        {
            qint32 pixelX = x + pos.x();
            if (pixelX >= 0 && pixelX < imageSize)
            {
                for (qint32 y = -1; y <= 1; ++y)
                {
                    qint32 pixelY = y + pos.y();
                    if (pixelY >= 0 && pixelY < imageSize)
                    {
                        m_colorTable.setPixelColor(pixelX, pixelY, color);
                    }
                }
            }
        }
    }
#endif
}

void Player::setPlayerArmy(const QString & value)
{
    m_playerArmy = value;
}

QString Player::getPlayerArmy() const
{
    return m_playerArmy;
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
#ifdef GRAPHICSUPPORT
    return m_ColorTableAnim;
#else
    return oxygine::spResAnim();
#endif
}

oxygine::spResAnim Player::getNeutralTableAnim()
{
    if (m_neutralTableAnim.get() == nullptr)
    {
        m_neutralTableAnim = MemoryManagement::create<oxygine::SingleResAnim>();
        QStringList searchPaths;
        for (qint32 i = 0; i < Settings::getInstance()->getMods().size(); i++)
        {
            searchPaths.append(Settings::getInstance()->getUserPath() + Settings::getInstance()->getMods().at(i) + "/images/colortables/");
            searchPaths.append(oxygine::Resource::RCC_PREFIX_PATH + Settings::getInstance()->getMods().at(i) + "/images/colortables/");
        }
        searchPaths.append("resources/images/colortables/");
        searchPaths.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/images/colortables/");
        for (auto & path : searchPaths)
        {
            if (QFile::exists(path + "neutral.png"))
            {
                m_neutralTableImage = QImage(path + "neutral.png");
                Mainapp::getInstance()->loadResAnim(m_neutralTableAnim, m_neutralTableImage, 1, 1, 1);
                break;
            }
        }
    }
    return m_neutralTableAnim;
}

qint32 Player::getPlayerID() const
{    
    if (m_pMap != nullptr)
    {
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            if (m_pMap->getPlayer(i) == this)
            {
                return i;
            }
        }
    }
    return 0;
}

bool Player::getFlipUnitSprites() const
{
    
    if (m_pMap != nullptr)
    {
        if (m_pMap->getGameRules()->getTeamFacingUnits())
        {
            return !GlobalUtils::isEven(m_team);
        }
        else
        {
            for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
            {
                if (m_pMap->getPlayer(i) == this)
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
        QJSValueList args({pInterpreter->newQObject(this),
                           pInterpreter->newQObject(m_pMap)});
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

bool Player::isPlayerIdEnemy(qint32 playerId)
{
    
    if (m_pMap != nullptr)
    {
        if (playerId < 0 || playerId >= m_pMap->getPlayerCount())
        {
            oxygine::handleErrorPolicy(oxygine::error_policy::ep_show_error, "Player::isPlayerIdEnemy playerId outside player range");
        }
        else
        {
            return (checkAlliance(m_pMap->getPlayer(playerId)) == GameEnums::Alliance_Enemy);
        }
    }
    return true;
}

bool Player::isPlayerIdAlly(qint32 playerId)
{
    
    if (m_pMap != nullptr)
    {
        if (playerId < 0 || playerId >= m_pMap->getPlayerCount())
        {
            oxygine::handleErrorPolicy(oxygine::error_policy::ep_show_error, "Player::isPlayerIdEnemy playerId outside player range");
        }
        else
        {
            return (checkAlliance(m_pMap->getPlayer(playerId)) == GameEnums::Alliance_Friend);
        }
    }
    return true;
}

bool Player::isEnemy(Player* pOwner)
{
    return (checkAlliance(pOwner) == GameEnums::Alliance_Enemy);
}

bool Player::isAlly(Player* pOwner)
{
    return (checkAlliance(pOwner) == GameEnums::Alliance_Friend);
}

void Player::setFunds(const qint32 value)
{
    m_funds = value;
    if (m_pMenu != nullptr)
    {
        m_pMenu->updatePlayerinfo();
    }
}

void Player::addFunds(const qint32 value)
{
    setFunds(m_funds + value);
}

qint32 Player::getFunds() const
{
    return m_funds;
}

qint32 Player::getBuildingCount(const QString & buildingID)
{
    qint32 ret = 0;
    if (m_pMap != nullptr)
    {
        ret = m_pMap->getPlayerBuildingCount(buildingID, this);
    }
    return ret;
}

qint32 Player::getBuildingListCount(const QStringList & list, bool whitelist)
{
    qint32 ret = 0;
    
    if (m_pMap != nullptr)
    {
        for (qint32 y = 0; y < m_pMap->getMapHeight(); y++)
        {
            for (qint32 x = 0; x < m_pMap->getMapWidth(); x++)
            {
                spBuilding pBuilding = m_pMap->getSpTerrain(x, y)->getSpBuilding();
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

qint32 Player::getUnitCount(const QString & unitID) const
{
    qint32 ret = 0;
    
    for (qint32 y = 0; y < m_pMap->getMapHeight(); y++)
    {
        for (qint32 x = 0; x < m_pMap->getMapWidth(); x++)
        {
            spUnit pUnit = m_pMap->getSpTerrain(x, y)->getSpUnit();
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

qint32 Player::getUnitCount(Unit* pUnit, const QString & unitID) const
{
    qint32 ret = 0;
    for (qint32 i = 0; i < pUnit->getLoadedUnitCount(); i++)
    {
        Unit* pLoadedUnit = pUnit->getLoadedUnit(i);
        if (pLoadedUnit->getOwner() == this)
        {
            if (unitID.isEmpty() ||pLoadedUnit->getUnitID() == unitID )
            {
                ret++;
            }
        }
        ret += getUnitCount(pLoadedUnit, unitID);
    }
    return ret;
}

qint32 Player::getCoBonus(QPoint position, Unit* pUnit, const QString & function)
{
    qint32 ret = 0;
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            ret += pCO->getCoBonus(position, pUnit, function);
        }
    }
    return ret;
}

qint32 Player::getTeam() const
{
    return m_team;
}

void Player::setTeam(const qint32 value)
{
    m_team = value;
}

void Player::defeatPlayer(Player* pPlayer, bool units)
{
    
    QVector<GameAnimation*> pAnimations;
    qint32 counter = 0;
    m_isDefeated = true;
    for (qint32 y = 0; y < m_pMap->getMapHeight(); y++)
    {
        for (qint32 x = 0; x < m_pMap->getMapWidth(); x++)
        {
            spBuilding pBuilding = m_pMap->getSpTerrain(x, y)->getSpBuilding();
            spUnit pUnit = m_pMap->getSpTerrain(x, y)->getSpUnit();
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
    spQmlVectorUnit pUnits = getSpUnits();
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
    if (m_pMenu != nullptr)
    {
        m_pMenu->updatePlayerinfo();
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
        for(auto & pCO : m_playerCOs)
        {
            if (pCO.get() != nullptr)
            {
                reduction += pCO->getIncomeReduction(pBuilding, income);
            }
        }
    }
    return reduction;
}

void Player::onUnitDeath(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "onUnitDeath";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction("PLAYER", function1, args);
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            pCO->onUnitDeath(pUnit);
        }
    }
}

void Player::postAction(GameAction* pAction)
{
    if (!m_isDefeated)
    {
        for(auto & pCO : m_playerCOs)
        {
            if (pCO.get() != nullptr)
            {
                pCO->postAction(pAction);
            }
        }
    }
}

qint32 Player::calcIncome(qreal modifier) const
{
    qint32 ret = 0;
    
    for (qint32 y = 0; y < m_pMap->getMapHeight(); y++)
    {
        for (qint32 x = 0; x < m_pMap->getMapWidth(); x++)
        {
            spBuilding pBuilding = m_pMap->getSpTerrain(x, y)->getSpBuilding();
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
    spQmlVectorUnit pUnits = m_pMap->getSpUnits(this);
    qint32 armyValue = 0;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        armyValue += pUnits->at(i)->getCoUnitValue();
    }
    return armyValue;
}

void Player::earnMoney(qreal modifier)
{
    setFunds(m_funds + calcIncome(modifier));
}

qint32 Player::getCostModifier(const QString & id, qint32 baseCost, QPoint position)
{
    
    qint32 costModifier = 0;
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            costModifier += pCO->getCostModifier(id, baseCost, position);
        }
    }
    if (m_pMap != nullptr)
    {
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            if (pPlayer != nullptr &&
                isEnemy(pPlayer) &&
                !pPlayer->getIsDefeated())
            {
                for(auto & pCO : pPlayer->m_playerCOs)
                {
                    if (pCO.get() != nullptr)
                    {
                        costModifier += pCO->getEnemyCostModifier(id, baseCost, position);
                    }
                }
            }
        }
    }
    return costModifier;
}

void Player::postBattleActions(Unit* pAttacker, qreal atkDamage, Unit* pDefender, bool gotAttacked, qint32 weapon, GameAction* pAction)
{
    if (!m_isDefeated)
    {
        for(auto & pCO : m_playerCOs)
        {
            if (pCO.get() != nullptr)
            {
                pCO->postBattleActions(pAttacker, atkDamage, pDefender, gotAttacked, weapon, pAction);
            }
        }
    }
}

void Player::buildedUnit(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "buildedUnit";
    QJSValueList args({pInterpreter->newQObject(pUnit),
                       pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(pUnit->getUnitID(), function1, args);
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            pCO->buildedUnit(pUnit);
        }
    }    
}

bool Player::getWeatherImmune()
{
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr &&
            pCO->getWeatherImmune())
        {
            return true;
        }
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
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            ret.append(pCO->getCOUnits(pBuilding));
        }
    }
    return ret;
}

QStringList Player::getTransportUnits(Unit* pUnit)
{
    QStringList ret;
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            ret.append(pCO->getTransportUnits(pUnit));
        }
    }
    return ret;
}

void Player::setBuildList(const QStringList & BuildList)
{
    m_BuildList = BuildList;
    m_BuildlistChanged = true;
}

void Player::changeBuildlist(const QString & unitID, bool remove)
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
    
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = m_pMap->getPlayer(i);
        if (isAlly(pPlayer))
        {
            pPlayer->addVisionFieldInternal(x, y, duration - 1, directView);
        }
    }
}

void Player::addVisionFieldInternal(qint32 x, qint32 y, qint32 duration, bool directView)
{
    m_FogVisionFields[x][y].m_visionType = GameEnums::VisionType_Clear;
    if (duration > m_FogVisionFields[x][y].m_duration)
    {
        m_FogVisionFields[x][y].m_duration = duration;
    }
    if (m_FogVisionFields[x][y].m_directView == false)
    {
        m_FogVisionFields[x][y].m_directView = directView;
    }
}

#ifdef GRAPHICSUPPORT
const QImage &Player::getNeutralTableImage()
{
    return m_neutralTableImage;
}

const QImage &Player::getColorTable() const
{
    return m_colorTable;
}
#endif

void Player::updatePlayerVision(bool reduceTimer)
{
    
    // only update visual stuff if needed
    if (reduceTimer ||
        m_pMap->getCurrentPlayer() == this ||
        m_pMap->getCurrentViewPlayer() == this)
    {
        qint32 width = m_pMap->getMapWidth();
        qint32 heigth = m_pMap->getMapHeight();
        for (qint32 x = 0; x < width; x++)
        {
            for (qint32 y = 0; y < heigth; y++)
            {
                if (reduceTimer)
                {
                    m_FogVisionFields[x][y].m_duration -= 1;
                }
                qint32 duration = m_FogVisionFields[x][y].m_duration;
                if (duration <= 0)
                {
                    if (m_FogVisionFields[x][y].m_visionType == GameEnums::VisionType_Clear)
                    {
                        m_FogVisionFields[x][y].m_visionType = GameEnums::VisionType_Fogged;
                    }
                    m_FogVisionFields[x][y].m_duration = 0;
                    m_FogVisionFields[x][y].m_directView = false;
                }
            }
        }
        bool visionBlock = m_pMap->getGameRules()->getVisionBlock();
        // create vision for all units and terrain
        for (qint32 x = 0; x < width; x++)
        {
            for (qint32 y = 0; y < heigth; y++)
            {
                // check terrain vision
                Terrain* pTerrain = m_pMap->getTerrain(x, y);
                qint32 visionRange = pTerrain->getVision(this);
                if (visionRange >= 0)
                {
                    spQmlVectorPoint pPoints;
                    if (visionBlock)
                    {
                        pPoints = m_pMap->getSpVisionCircle(x, y, 0, visionRange, pTerrain->getTotalVisionHigh());
                    }
                    else
                    {
                        pPoints = GlobalUtils::getSpCircle(0, visionRange);
                    }
                    for (auto & point : pPoints->getVector())
                    {
                        if (m_pMap->onMap(point.x() + x, point.y() + y))
                        {
                            Terrain* visionField = m_pMap->getTerrain(point.x() + x,point.y() + y);
                            Unit* pUnit = visionField->getUnit();
                            bool visionHide = visionField->getVisionHide(this);
                            if ((!visionHide) ||
                                ((pUnit != nullptr) && visionHide &&
                                 !pUnit->useTerrainHide() && !pUnit->isStatusStealthed()))
                            {
                                m_FogVisionFields[point.x() + x][point.y() + y].m_visionType = GameEnums::VisionType_Clear;
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
                    m_FogVisionFields[x][y].m_visionType = GameEnums::VisionType_Clear;
                    qint32 visionRange = pBuilding->getVision();
                    if (visionRange >= 0)
                    {
                        spQmlVectorPoint pPoints;
                        if (visionBlock)
                        {
                            pPoints = m_pMap->getSpVisionCircle(x, y, 0, visionRange, pBuilding->getTotalVisionHigh());
                        }
                        else
                        {
                            pPoints = GlobalUtils::getSpCircle(0, visionRange);
                        }
                        for (auto & point : pPoints->getVector())
                        {
                            if (m_pMap->onMap(point.x() + x, point.y() + y))
                            {
                                Terrain* visionField = m_pMap->getTerrain(point.x() + x,point.y() + y);
                                Unit* pUnit = visionField->getUnit();
                                bool visionHide = visionField->getVisionHide(this);
                                if ((!visionHide) ||
                                    ((pUnit != nullptr) && visionHide &&
                                     !pUnit->useTerrainHide() && !pUnit->isStatusStealthed()))
                                {
                                    m_FogVisionFields[point.x() + x][point.y() + y].m_visionType = GameEnums::VisionType_Clear;
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
                            pPoints = m_pMap->getSpVisionCircle(x, y, 0, visionRange,  pUnit->getTotalVisionHigh());
                        }
                        else
                        {
                            pPoints = m_pMap->getSpVisionCircle(x, y, 0, visionRange,  pUnit->getVisionHigh() + pTerrain->getVisionHigh());
                        }
                    }
                    else
                    {
                        pPoints = GlobalUtils::getSpCircle(0, visionRange);
                    }
                    for (auto & point : pPoints->getVector())
                    {
                        if (m_pMap->onMap(point.x() + x, point.y() + y))
                        {
                            Terrain* visionField = m_pMap->getTerrain(point.x() + x,point.y() + y);
                            Unit* pUnit = visionField->getUnit();
                            bool visionHide = visionField->getVisionHide(this);
                            if ((!visionHide) ||
                                ((pUnit != nullptr) && visionHide &&
                                 !pUnit->useTerrainHide() && !pUnit->isStatusStealthed()))
                            {
                                m_FogVisionFields[point.x() + x][point.y() + y].m_visionType = GameEnums::VisionType_Clear;
                            }
                            // terrain hides are visible if we're near it.
                            else if (((qAbs(point.x()) + qAbs(point.y())) <= 1))
                            {
                                m_FogVisionFields[point.x() + x][point.y() + y].m_visionType = GameEnums::VisionType_Clear;
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
    
    switch (m_pMap->getGameRules()->getFogMode())
    {
        case GameEnums::Fog_OfMist:
        case GameEnums::Fog_Off:
        {
            return true;
        }
        case GameEnums::Fog_OfShroud:
        case GameEnums::Fog_OfWar:
        {
            if (m_FogVisionFields.size() > 0)
            {
                return (m_FogVisionFields[x][y].m_visionType == GameEnums::VisionType_Clear);
            }
            else
            {
                return true;
            }
        }
    }
    return false;
}

GameEnums::VisionType Player::getFieldVisibleType(qint32 x, qint32 y) const
{
    
    switch (m_pMap->getGameRules()->getFogMode())
    {
        case GameEnums::Fog_Off:
        {
            return GameEnums::VisionType_Clear;
        }
        case GameEnums::Fog_OfShroud:
        case GameEnums::Fog_OfMist:
        case GameEnums::Fog_OfWar:
        {
            if (m_FogVisionFields.size() > 0)
            {
                if (m_pMap->onMap(x, y))
                {
                    if (m_FogVisionFields.size() > 0)
                    {
                        return m_FogVisionFields[x][y].m_visionType;
                    }
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
        
        if (m_pMap->onMap(x, y))
        {
            return m_FogVisionFields[x][y].m_directView;
        }
        else
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Player::getFieldDirectVisible trying to read not existing field");
            return false;
        }
    }
    else
    {
        return true;
    }
}

qint32 Player::getCosts(const QString & id, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue ret = pInterpreter->doFunction(id, "getBaseCost");
    qint32 costs = 0;
    if (ret.isNumber())
    {
        costs = ret.toInt();
    }

    qint32 costModifier = 0;
    if (m_pMap != nullptr)
    {
        qint32 mapHeigth = m_pMap->getMapHeight();
        qint32 mapWidth = m_pMap->getMapWidth();
        for (qint32 x = 0; x < mapWidth; x++)
        {
            for (qint32 y = 0; y < mapHeigth; y++)
            {
                Building* pBuilding = m_pMap->getTerrain(x, y)->getBuilding();
                if ((pBuilding != nullptr) && pBuilding->getOwner() == this)
                {
                    costModifier += pBuilding->getCostModifier(id, costs, position);
                }
            }
        }
    }

    costModifier += getCostModifier(id, costs, position);
    costs += costModifier;
    if (costs < 0)
    {
        costs = 0;
    }
    return costs;
}

void Player::gainPowerstar(qint32 fundsDamage, QPoint position, qint32 hpDamage, bool defender, bool counterAttack)
{
    qreal speed = m_pMap->getGameRules()->getPowerGainSpeed();
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            pCO->gainPowerstar(fundsDamage * speed, position, hpDamage * speed, defender, counterAttack);
        }
    }
    if (m_pMenu != nullptr)
    {
        m_pMenu->updatePlayerinfo();
    }
}

qint32 Player::getMovementcostModifier(Unit* pUnit, QPoint position)
{
    qint32 modifier = 0;
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            modifier += pCO->getMovementcostModifier(pUnit, position);
        }
    }
    return modifier;
}

qint32 Player::getBonusMisfortune(Unit* pUnit, QPoint position)
{
    qint32 modifier = 0;
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            modifier += pCO->getBonusMisfortune(pUnit, position);
        }
    }
    return modifier;
}

qint32 Player::getBonusLuck(Unit* pUnit, QPoint position)
{
    qint32 modifier = 0;
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            modifier += pCO->getBonusLuck(pUnit, position);
        }
    }
    return modifier;
}

qint32 Player::getEnemyBonusLuck(Unit* pUnit, QPoint position)
{
    qint32 modifier = 0;
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            modifier += pCO->getEnemyBonusLuck(pUnit, position);
        }
    }
    return modifier;
}

qint32 Player::getEnemyBonusMisfortune(Unit* pUnit, QPoint position)
{
    qint32 modifier = 0;
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            modifier += pCO->getEnemyBonusMisfortune(pUnit, position);
        }
    }
    return modifier;
}

qreal Player::getEnemyRepairCostModifier(Unit* pUnit)
{
    qreal modifier = 0;
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            modifier += pCO->getEnemyRepairCostModifier(pUnit);
        }
    }
    return modifier;
}

qreal Player::getRepairCostModifier(Unit* pUnit)
{
    qreal modifier = 0;
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            modifier += pCO->getRepairCostModifier(pUnit);
        }
    }
    return modifier;
}

qint32 Player::getWeatherMovementCostModifier(Unit* pUnit, QPoint position)
{
    qint32 modifier = 0;
    
    if (m_pMap != nullptr && !pUnit->getWeatherImmune())
    {
        modifier += m_pMap->getGameRules()->getCurrentWeather()->getMovementCostModifier(pUnit, m_pMap->getTerrain(position.x(), position.y()));
    }
    return modifier;
}

qint32 Player::getBonusMovementpoints(Unit* pUnit, QPoint position)
{
    qint32 movementModifier = 0;
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            movementModifier += pCO->getMovementpointModifier(pUnit, position);
        }
    }
    if (pUnit->getOwner() == this)
    {
        
        if (m_pMap != nullptr && !pUnit->getWeatherImmune())
        {
            movementModifier += m_pMap->getGameRules()->getCurrentWeather()->getMovementpointModifier(pUnit, m_pMap->getTerrain(position.x(), position.y()));
        }
    }
    return movementModifier;
}

void Player::startOfTurn()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "startOfTurn";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction("PLAYER", function1, args);
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            pCO->setPowerMode(GameEnums::PowerMode_Off);
            pCO->setCoRangeEnabled(true);
            pCO->startOfTurn();
        }
    }
}

void Player::endOfTurn()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "endOfTurn";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction("PLAYER", function1, args);
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            pCO->endOfTurn();
        }
    }
}

spQmlVectorUnit Player::getSpUnits()
{
    spQmlVectorUnit ret;
    if (m_pMap != nullptr)
    {
        ret = m_pMap->getSpUnits(this);
    }
    else
    {
        ret = MemoryManagement::create<QmlVectorUnit>();
    }
    return ret;
}

QmlVectorUnit* Player::getUnits()
{
    auto ret = getSpUnits();
    Interpreter* pInterpreter = Interpreter::getInstance();
    Q_ASSERT(pInterpreter->getInJsCall());
    pInterpreter->trackJsObject(ret);
    return ret.get();
}

qint32 Player::getEnemyCount()
{
    
    qint32 ret = 0;
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        if (isEnemy(m_pMap->getPlayer(i)))
        {
            ret++;
        }
    }
    return ret;
}

spQmlVectorUnit Player::getSpEnemyUnits()
{
    spQmlVectorUnit ret = MemoryManagement::create<QmlVectorUnit>();
    if (m_pMap)
    {
        qint32 heigth = m_pMap->getMapHeight();
        qint32 width = m_pMap->getMapWidth();
        for (qint32 y = 0; y < heigth; y++)
        {
            for (qint32 x = 0; x < width; x++)
            {
                Unit* pUnit = m_pMap->getTerrain(x, y)->getUnit();
                if (pUnit != nullptr)
                {
                    if ((isEnemyUnit(pUnit)))
                    {
                        ret->append(pUnit);
                    }
                }
            }
        }
    }
    return ret;
}

QmlVectorUnit* Player::getEnemyUnits()
{
    auto ret = getSpEnemyUnits();
    Interpreter* pInterpreter = Interpreter::getInstance();
    Q_ASSERT(pInterpreter->getInJsCall());
    pInterpreter->trackJsObject(ret);
    return ret.get();
}

spQmlVectorUnit Player::getSpAlliedUnits()
{
    spQmlVectorUnit ret = MemoryManagement::create<QmlVectorUnit>();
    if (m_pMap)
    {
        qint32 heigth = m_pMap->getMapHeight();
        qint32 width = m_pMap->getMapWidth();
        for (qint32 y = 0; y < heigth; y++)
        {
            for (qint32 x = 0; x < width; x++)
            {
                Unit* pUnit = m_pMap->getTerrain(x, y)->getUnit();
                if (pUnit != nullptr)
                {
                    if (pUnit->getOwner() != this &&
                        isAlly(pUnit->getOwner()))
                    {
                        ret->append(pUnit);
                    }
                }
            }
        }
    }
    return ret;
}

QmlVectorUnit* Player::getAlliedUnits()
{
    auto ret = getSpEnemyUnits();
    Interpreter* pInterpreter = Interpreter::getInstance();
    Q_ASSERT(pInterpreter->getInJsCall());
    pInterpreter->trackJsObject(ret);
    return ret.get();
}

spQmlVectorBuilding Player::getSpEnemyBuildings()
{
    spQmlVectorBuilding ret = MemoryManagement::create<QmlVectorBuilding>();
    if (m_pMap)
    {
        qint32 heigth = m_pMap->getMapHeight();
        qint32 width = m_pMap->getMapWidth();
        for (qint32 y = 0; y < heigth; y++)
        {
            for (qint32 x = 0; x < width; x++)
            {
                Building* pBuilding = m_pMap->getTerrain(x, y)->getBuilding();
                if (pBuilding != nullptr &&
                    pBuilding->getTerrain() == m_pMap->getTerrain(x, y) &&
                    pBuilding->isEnemyBuilding(this))
                {
                    ret->append(pBuilding);
                }
            }
        }
    }
    return ret;
}

QmlVectorBuilding* Player::getEnemyBuildings()
{
    auto ret = getSpEnemyBuildings();
    Interpreter* pInterpreter = Interpreter::getInstance();
    Q_ASSERT(pInterpreter->getInJsCall());
    pInterpreter->trackJsObject(ret);
    return ret.get();
}

spQmlVectorBuilding Player::getSpBuildings(const QString & id)
{
    return m_pMap->getSpBuildings(this, id);
}

QmlVectorBuilding* Player::getBuildings(const QString & id)
{
    return m_pMap->getBuildings(this, id);
}

QmlVectorBuilding* Player::getBuildingsListCount(const QStringList & ids)
{
    return m_pMap->getBuildingsListCount(this, ids);
}

void Player::updateVisualCORange()
{
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            Unit* pCoUnit = pCO->getCOUnit();
            if (pCoUnit != nullptr)
            {
                if (pCO->getPowerMode() == GameEnums::PowerMode_Off)
                {
                    pCoUnit->createCORange(pCO->getCORange());
                }
                else
                {
                    pCoUnit->createCORange(-1);
                }
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

spCO Player::getSpCO(quint8 id)
{
    if (id <= 1)
    {
        return m_playerCOs[id];
    }
    else
    {
        return spCO();
    }
}

CO* Player::getCO(quint8 id)
{
    if (id < m_playerCOs.max_size())
    {
        return m_playerCOs[id].get();
    }
    else
    {
        return nullptr;
    }
}

qint32 Player::getMaxCoCount() const
{
    return m_playerCOs.max_size();
}

void Player::setCO(QString coId, quint8 idx)
{
    if (idx < m_playerCOs.max_size())
    {
        if (coId.isEmpty())
        {
            m_playerCOs[idx].reset();
        }
        else
        {
            m_playerCOs[idx] = MemoryManagement::create<CO>(coId, this, m_pMap);
        }

    }
}

qint32 Player::getCoCount() const
{
    qint32 ret = 0;
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            ++ret;
        }
    }
    return ret;
}

qreal Player::getCoGroupModifier(QStringList unitIds, SimpleProductionSystem* system)
{
    qreal ret = 1.0;
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() != nullptr)
        {
            ret += pCO->getCoGroupModifier(unitIds, system);
        }
    }
    return ret;
}

QPoint Player::getRockettarget(qint32 radius, qint32 damage, qreal ownUnitValue, GameEnums::RocketTarget targetType, QmlVectorPoint* pSearchArea)
{
    
    spQmlVectorPoint pPoints = GlobalUtils::getSpCircle(0, radius);
    qint32 highestDamage = -1;
    QVector<QPoint> targets;
    if (pSearchArea == nullptr)
    {
        for (qint32 x = 0; x < m_pMap->getMapWidth(); x++)
        {
            for (qint32 y = 0; y < m_pMap->getMapHeight(); y++)
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
    }
    else
    {
        const auto & vector = pSearchArea->getVector();
        for (auto iter = vector.begin(); iter != vector.end(); ++iter)
        {
            qint32 x = iter->x();
            qint32 y = iter->y();
            if (m_pMap->onMap(x, y))
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

QPoint Player::getSiloRockettarget(qint32 radius, qint32 damage, qint32 highestDamage, qreal ownUnitValue, GameEnums::RocketTarget targetType, QmlVectorPoint* pSearchArea)
{    
    spQmlVectorPoint pPoints = GlobalUtils::getSpCircle(0, radius);
    highestDamage = -1;
    QVector<QPoint> targets;
    if (pSearchArea == nullptr)
    {
        for (qint32 x = 0; x < m_pMap->getMapWidth(); x++)
        {
            for (qint32 y = 0; y < m_pMap->getMapHeight(); y++)
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
    }
    else
    {
        const auto & vector = pSearchArea->getVector();
        for (auto iter = vector.begin(); iter != vector.end(); ++iter)
        {
            qint32 x = iter->x();
            qint32 y = iter->y();
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
            QJSValueList args({pInterpreter->newQObject(m_pMap)});
            QJSValue erg = pInterpreter->doFunction(unitId, function1, args);
            if (erg.isNumber())
            {
                m_averageCosts += erg.toInt();
            }
        }
        m_averageCosts = m_averageCosts / pUnitSpriteManager->getCount();
    }
    return m_averageCosts;
}

qint32 Player::getRocketTargetDamage(qint32 x, qint32 y, QmlVectorPoint* pPoints, qint32 damage, qreal ownUnitValue, GameEnums::RocketTarget targetType, bool ignoreStealthed)
{
    qint32 averageCosts = getAverageCost();
    
    qint32 damageDone = 0;
    for (auto & point : pPoints->getVector())
    {
        qint32 x2 = x + point.x();
        qint32 y2 = y + point.y();
        // is there a unit?
        if ((m_pMap->onMap(x2, y2)) &&
            (m_pMap->getTerrain(x2, y2)->getUnit() != nullptr))
        {
            Unit* pUnit = m_pMap->getTerrain(x2, y2)->getUnit();
            if (!pUnit->isStealthed(this) || ignoreStealthed)
            {
                qreal modifier = 1.0f;
                if (!isEnemyUnit(pUnit))
                {
                    modifier = -ownUnitValue;
                }
                qreal damagePoints = damage;
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
                        damageDone += damagePoints / Unit::MAX_UNIT_HP * modifier * pUnit->getCoUnitValue();
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

qint32 Player::getPowerChargeBonus() const
{
    qint32 bonus = 0;
    qint32 mapHeigth = m_pMap->getMapHeight();
    qint32 mapWidth = m_pMap->getMapWidth();
    for (qint32 x = 0; x < mapWidth; x++)
    {
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            Building* pBuilding = m_pMap->getTerrain(x, y)->getBuilding();
            if (pBuilding != nullptr &&
                pBuilding->getOwner() == this)
            {
                bonus += pBuilding->getPowerChargeBonus();
            }
        }
    }
    return bonus;
}

qreal Player::getFundsModifier() const
{
    return m_fundsModifier;
}

void Player::setFundsModifier(qreal value)
{
    m_fundsModifier = value;
}

GameMap *Player::getMap() const
{
    return m_pMap;
}

qint32 Player::calculatePlayerStrength() const
{
    qint32 ret = 0;
    
    for (qint32 x = 0; x < m_pMap->getMapWidth(); x++)
    {
        for (qint32 y = 0; y < m_pMap->getMapHeight(); y++)
        {
            Terrain* pTerrain = m_pMap->getTerrain(x, y);
            Unit* pUnit = pTerrain->getUnit();
            if (pUnit != nullptr &&
                pUnit->getOwner() == this)
            {
                ret += pUnit->getCoUnitValue();
                ret += calculatePlayerStrength(pUnit);
            }
        }
    }
    return ret + calcIncome();
}

qint32 Player::calculatePlayerStrength(Unit* pUnit) const
{
    qint32 ret = 0;
    for (qint32 i = 0; i < pUnit->getLoadedUnitCount(); i++)
    {
        Unit* pLoadedUnit = pUnit->getLoadedUnit(i);
        if (pLoadedUnit->getOwner() == this)
        {
            ret += pLoadedUnit->getCoUnitValue();
        }
        ret += calculatePlayerStrength(pLoadedUnit);
    }
    return ret;
}

bool Player::getIsOnline() const
{
    return m_isOnline;
}

void Player::setIsOnline(bool newIsOnline)
{
    m_isOnline = newIsOnline;
}

GameEnums::AiTypes Player::getControlType() const
{
    return m_controlType;
}

void Player::setControlType(const GameEnums::AiTypes newControlType)
{
    m_controlType = newControlType;
}

void Player::setMenu(GameMenue *newMenu)
{
    m_pMenu = newMenu;
}

void Player::serializeObject(QDataStream& pStream) const
{
    serializeObject(pStream, false);
}

void Player::serializeObject(QDataStream& pStream, bool forHash) const
{
    CONSOLE_PRINT("storing player with control type " + QString::number(m_controlType) + " and name " + m_playerNameId, GameConsole::eDEBUG);
    pStream << getVersion();
    if (!forHash)
    {
        quint32 color = m_Color.rgb();
        pStream << color;
    }
    pStream << m_funds;
    pStream << m_fundsModifier;
    if (!forHash)
    {
        pStream << m_playerArmy;
    }
    for(auto & pCO : m_playerCOs)
    {
        if (pCO.get() == nullptr)
        {
            pStream << false;
        }
        else
        {
            pStream << true;
            pCO->serializeObject(pStream, forHash);
        }
    }
    pStream << m_team;
    pStream << m_isDefeated;
    if (!forHash)
    {
        BaseGameInputIF::serializeInterface(pStream, m_pBaseGameInput.get(), m_controlType);
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
                pStream << static_cast<qint32>(m_FogVisionFields[x][y].m_visionType);
                pStream << m_FogVisionFields[x][y].m_duration;
                pStream << m_FogVisionFields[x][y].m_directView;
            }
        }
    }
    Filesupport::writeVectorList(pStream, m_BuildList);
    pStream << m_BuildlistChanged;
    m_Variables.serializeObject(pStream, forHash);
    if (!forHash)
    {
        pStream << m_playerArmySelected;
        pStream << m_playerNameId;
        pStream << static_cast<qint32>(m_controlType);
    }
}

void Player::deserializeObject(QDataStream& pStream)
{
    deserializer(pStream, false);
}

void Player::deserializer(QDataStream& pStream, bool fast)
{
    CONSOLE_PRINT("reading player", GameConsole::eDEBUG);
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
        if (version > 18)
        {
            pStream >> m_fundsModifier;
        }
        else
        {
            float fundsModifier = 0;
            pStream >> fundsModifier;
            m_fundsModifier = fundsModifier;
        }
        pStream >> m_playerArmy;
        qint32 co = 0;
        for(auto & pCO : m_playerCOs)
        {
            bool hasC0 = false;
            pStream >> hasC0;
            if (hasC0)
            {
                m_playerCOs[co] = MemoryManagement::create<CO>("", this, m_pMap);
                m_playerCOs[co]->deserializer(pStream, fast);
                if (!m_playerCOs[co]->isValid())
                {
                    m_playerCOs[co].reset();
                }
            }
            ++co;
        }
        if (version > 3)
        {
            pStream >> m_team;
        }
        if (version > 4)
        {
            pStream >> m_isDefeated;
            m_pBaseGameInput = BaseGameInputIF::deserializeInterface(m_pMap, pStream, version);
            if (m_pBaseGameInput.get() != nullptr)
            {
                m_pBaseGameInput->setPlayer(this);
            }
        }
        else
        {
            m_pBaseGameInput = MemoryManagement::create<HumanPlayerInput>(m_pMap);
            m_pBaseGameInput->setPlayer(this);
        }
        m_FogVisionFields.clear();
        if (version > 5)
        {
            qint32 width = 0;
            qint32 heigth = 0;
            pStream >> width;
            pStream >> heigth;
            CONSOLE_PRINT("Loading player vision width " + QString::number(width) + " height " + QString::number(heigth), GameConsole::eDEBUG);
            m_FogVisionFields.reserve(width);
            for (qint32 x = 0; x < width; x++)
            {
                m_FogVisionFields.push_back(std::vector<VisionFieldInfo>(heigth, VisionFieldInfo()));
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
                    m_FogVisionFields[x][y] = VisionFieldInfo(value, duration, directView);
                }
            }
        }
    }
    m_BuildList.clear();
    if (version > 17)
    {
        m_BuildList = Filesupport::readVectorList<QString, QList>(pStream);
    }
    else if (version > 6)
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
        if (version > 14)
        {
            if (!colorToTable(m_Color))
            {
                createTable(m_Color);
            }
        }
        else
        {            
            qint32 width = 0;
            pStream >> width;
            QRgb rgb;
            for (qint32 x = 0; x < width; x++)
            {
                pStream >> rgb;
            }
            if (!colorToTable(m_Color))
            {
                createTable(m_Color);
            }
        }
        if (!fast)
        {
#ifdef GRAPHICSUPPORT
            CONSOLE_PRINT("Loading colortable", GameConsole::eDEBUG);
            m_ColorTableAnim = MemoryManagement::create<oxygine::SingleResAnim>();
            Mainapp::getInstance()->loadResAnim(m_ColorTableAnim, m_colorTable, 1, 1, 1);
#endif
        }
    }
    else
    {
        if (!colorToTable(m_Color))
        {
            createTable(m_Color.darker(160));
        }
        if (!fast)
        {
#ifdef GRAPHICSUPPORT
            CONSOLE_PRINT("Loading colortable", GameConsole::eDEBUG);
            m_ColorTableAnim = MemoryManagement::create<oxygine::SingleResAnim>();
            Mainapp::getInstance()->loadResAnim(m_ColorTableAnim, m_colorTable, 1, 1, 1);
#endif
        }
    }
    if (version > 13)
    {
        pStream >> m_playerArmySelected;
    }
    if (version > 15)
    {
        pStream >> m_playerNameId;
    }
    if (version > 16)
    {
        qint32 type;
        pStream >> type;
        m_controlType = static_cast<GameEnums::AiTypes>(type);
    }
    else
    {
        if (m_pBaseGameInput.get() != nullptr)
        {
            m_controlType = m_pBaseGameInput->getAiType();
        }
        else
        {
            m_controlType = GameEnums::AiTypes_Human;
        }
    }
    CONSOLE_PRINT("Loaded player " + m_playerNameId + " with control type " + QString::number(m_controlType), GameConsole::eDEBUG);
}
