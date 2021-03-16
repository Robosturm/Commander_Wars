#include <QFileInfo>
#include <QFile>

#include "coreengine/console.h"
#include "coreengine/mainapp.h"

#include "resource_management/terrainmanager.h"

#include "game/terrain.h"
#include "game/gamemap.h"
#include "game/terrainfindingsystem.h"
#include "game/player.h"
#include "game/co.h"

spTerrain Terrain::createTerrain(QString terrainID, qint32 x, qint32 y, QString  currentTerrainID)
{
    spTerrain pTerrain = new Terrain(terrainID, x, y);
    pTerrain->setDestRecModifier(oxygine::RectF(0.5f, 0.5f, 0.5f, 0.5f));
    pTerrain->setSize(GameMap::getImageSize(), GameMap::getImageSize());
    if (terrainID != "")
    {
        pTerrain->createBaseTerrain(currentTerrainID);
    }
    // check if the js-script was loaded already
    // otherwise do load it
    bool terrainExists = true;
    if (terrainID != "")
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue obj = pInterpreter->getGlobal(terrainID);
        if (!obj.isObject())
        {
            terrainExists = false;
        }
        if (terrainExists)
        {
            pTerrain->init();
        }
        else
        {
            Console::print("Unable to load Terrain " + terrainID, Console::eFATAL);
        }
    }
    return pTerrain;
}

Terrain::Terrain(QString terrainID, qint32 x, qint32 y)
    : terrainID(terrainID),
      x(x),
      y(y),
      m_Building{nullptr}
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain));
}

bool Terrain::getHasStartOfTurn() const
{
    return m_hasStartOfTurn;
}

void Terrain::setHasStartOfTurn(bool hasStartOfTurn)
{
    m_hasStartOfTurn = hasStartOfTurn;
}

QString Terrain::getTerrainDescription() const
{
    return m_terrainDescription;
}

void Terrain::setTerrainDescription(const QString &terrainDescription)
{
    m_terrainDescription = terrainDescription;
}

qint32 Terrain::getVisionHigh() const
{
    return m_VisionHigh;
}

void Terrain::setVisionHigh(const qint32 &VisionHigh)
{
    m_VisionHigh = VisionHigh;
}

qint32 Terrain::getTotalVisionHigh()
{
    qint32 high = m_VisionHigh;
    if (m_Building.get() != nullptr)
    {
        high += m_Building->getVisionHigh();
    }
    if (high < 0)
    {
        high = 0;
    }
    return high;
}

Terrain::~Terrain()
{
}

bool Terrain::isValid()
{
    return TerrainManager::getInstance()->exists(terrainID);
}

void Terrain::setSpriteVisibility(bool value)
{
    if (m_pBaseTerrain.get() != nullptr)
    {
        m_pBaseTerrain->setSpriteVisibility(value);
    }
    m_pTerrainSprite->setVisible(value);
    for (auto& sprite : m_pOverlaySprites)
    {
        sprite->setVisible(value);
    }
}

QString Terrain::getTerrainSpriteName() const
{
    return m_terrainSpriteName;
}

void Terrain::setTerrainSpriteName(const QString &terrainSpriteName)
{
    m_terrainSpriteName = terrainSpriteName;
}

bool Terrain::getFixedSprite() const
{
    return m_FixedSprite;
}

void Terrain::setFixedSprite(bool FixedSprite)
{
    m_FixedSprite = FixedSprite;
}

void Terrain::init()
{
    if (m_pBaseTerrain.get() != nullptr)
    {
        m_pBaseTerrain->init();
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "init";
    QJSValueList args;
    QJSValue objArg = pInterpreter->newQObject(this);
    args << objArg;
    pInterpreter->doFunction(terrainID, function, args);
}

qint32 Terrain::getHp() const
{
    return hp;
}

void Terrain::setHp(const qint32 &value)
{
    hp = value;
}

QString Terrain::getTerrainName() const
{
    return terrainName;
}

void Terrain::setTerrainName(const QString &value)
{
    terrainName = value;
}

void Terrain::syncAnimation()
{
    if (m_pTerrainSprite.get() != nullptr)
    {
        oxygine::spTween pTween = m_pTerrainSprite->getFirstTween();
        while (pTween.get() != nullptr)
        {
            pTween->reset();
            pTween->start(*m_pTerrainSprite);
            pTween = pTween->getNextSibling();
        }
    }
    if (m_pBaseTerrain.get() != nullptr)
    {
        m_pBaseTerrain->syncAnimation();
    }
}

Unit* Terrain::getUnit()
{
    return m_Unit.get();
}

Building* Terrain::getBuilding()
{
    return m_Building.get();
}

spBuilding Terrain::getSpBuilding()
{
    return m_Building;
}

void Terrain::createBaseTerrain(QString  currentTerrainID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    args << currentTerrainID;
    // load sprite of the base terrain
    QString function = "loadBaseTerrain";
    pInterpreter->doFunction(terrainID, function, args);
    if (m_pBaseTerrain.get() != nullptr)
    {
        m_pBaseTerrain->createBaseTerrain(currentTerrainID);
    }
}

qint32 Terrain::getTerrainGroup()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainGroup";
    QJSValue ret = pInterpreter->doFunction(terrainID, function1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    return 0;
}

QString Terrain::getDescription()
{
    if (m_terrainDescription.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args;
        // load sprite of the base terrain
        QString function = "getDescription";
        QJSValue ret = pInterpreter->doFunction(terrainID, function, args);
        if (ret.isString())
        {
            return ret.toString();
        }
    }
    return m_terrainDescription;
}

void Terrain::setBaseTerrain(spTerrain terrain)
{
    if (m_pBaseTerrain.get() != nullptr)
    {
        m_pBaseTerrain->detach();
        m_pBaseTerrain = nullptr;
    }
    m_pBaseTerrain = terrain;
    m_pBaseTerrain->setPriority(static_cast<qint16>(DrawPriority::Terrain));
    m_pBaseTerrain->setPosition(0, 0);
    this->addChild(m_pBaseTerrain);
}

void Terrain::loadSprites()
{
    // unload old stuff
    if (m_pTerrainSprite.get() != nullptr)
    {
        m_pTerrainSprite->detach();
        m_pTerrainSprite = nullptr;
    }
    if (m_pOverlaySprites.size() > 0)
    {
        for (qint32 i = 0; i < m_pOverlaySprites.size(); i++)
        {
            m_pOverlaySprites[i]->detach();
        }
        m_pOverlaySprites.clear();
    }
    // load sub terrain
    if (m_pBaseTerrain.get() != nullptr)
    {
        m_pBaseTerrain->loadSprites();
    }
    // load main terrain
    Interpreter* pInterpreter = Interpreter::getInstance();
    if (m_FixedSprite)
    {
        loadBaseSprite(m_terrainSpriteName);
    }
    else
    {
        QString function1 = "loadBaseSprite";
        QJSValueList args1;
        QJSValue obj1 = pInterpreter->newQObject(this);
        args1 << obj1;
        pInterpreter->doFunction(terrainID, function1, args1);
    }
    // ony load this for valid positions
    if (x >= 0 && y >= 0)
    {
        // next call starting by 0 again
        QString function2 = "loadOverlaySprite";
        QJSValueList args2;
        QJSValue obj2 = pInterpreter->newQObject(this);
        args2 << obj2;
        pInterpreter->doFunction(terrainID, function2, args2);
    }
}

void Terrain::loadBaseTerrain(QString terrainID)
{
    m_pBaseTerrain = new Terrain(terrainID, x, y);
    m_pBaseTerrain->setPriority(static_cast<qint16>(DrawPriority::Terrain));
    m_pBaseTerrain->setPosition(0, 0);
    this->addChild(m_pBaseTerrain);
}

void Terrain::loadBaseSprite(QString spriteID)
{
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    oxygine::ResAnim* pAnim = pTerrainManager->getResAnim(spriteID, oxygine::error_policy::ep_ignore_error);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
            pSprite->addTween(tween);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }
        pSprite->setScale((GameMap::getImageSize()) / pAnim->getWidth());
        setSize(pAnim->getWidth(),
                pAnim->getHeight());

        pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::getImageSize()) / 2, -(pSprite->getScaledHeight() - GameMap::getImageSize()));
        this->addChild(pSprite);
        m_terrainSpriteName = spriteID;
        m_pTerrainSprite = pSprite;
    }
    else if (QFile::exists(m_terrainSpriteName))
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();
        pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::getImageSize()) / 2, -(pSprite->getScaledHeight() - GameMap::getImageSize()));
        this->addChild(pSprite);
        m_terrainSpriteName = spriteID;
        m_pTerrainSprite = pSprite;
        loadSprite = true;
    }
    else
    {
        Console::print("Unable to load terrain sprite: " + spriteID, Console::eERROR);
    }
}

void Terrain::update(const oxygine::UpdateState& us)
{
    if (loadSprite)
    {
        oxygine::SingleResAnim* pAnim = new oxygine::SingleResAnim();
        pAnim->setResPath(m_terrainSpriteName);
        pAnim->init(m_terrainSpriteName, 1, 1, 1.0f);
        m_SpriteAnim = pAnim;
        m_pTerrainSprite->setResAnim(pAnim);
        m_pTerrainSprite->setScale((GameMap::getImageSize()) / pAnim->getWidth() );
        setSize(pAnim->getWidth(),
                pAnim->getHeight());
        m_pTerrainSprite->setPosition(-(m_pTerrainSprite->getScaledWidth() - GameMap::getImageSize()) / 2, -(m_pTerrainSprite->getScaledHeight() - GameMap::getImageSize()));
        loadSprite = false;
    }
    oxygine::Actor::update(us);
}

QString Terrain::getSurroundings(QString list, bool useBaseTerrainID, bool blacklist, qint32 searchType, bool useMapBorder, bool useBuildingID, qint32 recursionCount)
{
    QStringList searchList = list.split(",");
    QString ret = "";
    for (qint32 i = 0; i < 8; i++)
    {
        qint32 curX = x;
        qint32 curY = y;
        // get our x, y coordinates
        GameMap::getField(curX, curY, static_cast<GameEnums::Directions>(i));
        GameMap* pGameMap = GameMap::getInstance();
        bool found = false;
        QString addString = "";
        // load compare value
        GameEnums::Directions compareValue = GameEnums::Directions_None;
        if (searchType == GameEnums::Directions_All)
        {
            compareValue = static_cast<GameEnums::Directions>(i);
        }
        else if (searchType == GameEnums::Directions_Direct)
        {
            switch (i)
            {
                case GameEnums::Directions_North:
                case GameEnums::Directions_East:
                case GameEnums::Directions_West:
                case GameEnums::Directions_South:
                {
                    compareValue = static_cast<GameEnums::Directions>(i);
                    break;
                }
                default:
                {
                    // do nothing
                    compareValue = GameEnums::Directions_None;
                    break;
                }
            }
        }
        else if (searchType == GameEnums::Directions_Diagnonal)
        {
            switch (i)
            {
                case GameEnums::Directions_NorthEast:
                case GameEnums::Directions_NorthWest:
                case GameEnums::Directions_SouthWest:
                case GameEnums::Directions_SouthEast:
                {
                    compareValue = static_cast<GameEnums::Directions>(i);
                    break;
                }
                default:
                {
                    compareValue = GameEnums::Directions_None;
                    // do nothing
                    break;
                }
            }
        }
        else if (searchType == i)
        {
            compareValue = static_cast<GameEnums::Directions>(i);
        }
        else
        {
            // you asshole reached unreachable code :D
        }
        // check for compare value to find string
        if (compareValue == GameEnums::Directions_North)
        {
            addString = "+N";
        }
        else if (compareValue == GameEnums::Directions_East)
        {
            addString = "+E";
        }
        else if (compareValue == GameEnums::Directions_South)
        {
            addString = "+S";
        }
        else if (compareValue == GameEnums::Directions_West)
        {
            addString = "+W";
        }
        else if (compareValue == GameEnums::Directions_NorthEast)
        {
            addString = "+NE";
        }
        else if (compareValue == GameEnums::Directions_SouthEast)
        {
            addString = "+SE";
        }
        else if (compareValue == GameEnums::Directions_SouthWest)
        {
            addString = "+SW";
        }
        else if (compareValue == GameEnums::Directions_NorthWest)
        {
            addString = "+NW";
        }
        else
        {
            // do nothing
        }
        if (pGameMap != nullptr && pGameMap->onMap(curX, curY))
        {
            QString neighbourID = "";
            Terrain* pTerrain = pGameMap->getTerrain(curX, curY);
            if (useBuildingID && pTerrain->getBuilding() != nullptr)
            {
                neighbourID = pTerrain->getID();
            }
            else if (useBaseTerrainID)
            {
                if (recursionCount > 0)
                {
                    neighbourID = pTerrain->getBaseTerrainIDOfLevel(recursionCount);
                }
                else
                {
                    neighbourID = pTerrain->getBaseTerrainID();
                }
            }
            else
            {
                neighbourID = pTerrain->getTerrainID();
            }
            if (blacklist)
            {
                if (!searchList.contains(neighbourID))
                {
                    found = true;
                }
            }
            else
            {
                if (searchList.contains(neighbourID))
                {
                    found = true;
                }
            }
        }
        else if (useMapBorder)
        {
            found = true;
        }
        else
        {
            found = false;
        }
        if (found)
        {
            ret += addString;
        }
    }
    return ret;
}

void Terrain::loadOverlaySprite(QString spriteID)
{
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    oxygine::ResAnim* pAnim = pTerrainManager->getResAnim(spriteID);
    oxygine::spSprite pSprite = new oxygine::Sprite();
    if (pAnim != nullptr)
    {
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
            pSprite->addTween(tween);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }
    }
    pSprite->setScale((GameMap::getImageSize()) / pAnim->getWidth());
    pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::getImageSize()) / 2, -(pSprite->getScaledHeight() - GameMap::getImageSize()));
    this->addChild(pSprite);
    m_pOverlaySprites.append(pSprite);
}

qint32 Terrain::getBaseDefense()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDefense";
    QJSValueList args1;
    qint32 defense = 0;
    if (m_Building.get() == nullptr)
    {
        QJSValue ret = pInterpreter->doFunction(terrainID, function1, args1);
        if (ret.isNumber())
        {
            defense = ret.toInt();
        }
    }
    else
    {
        QJSValue ret = pInterpreter->doFunction(m_Building->getBuildingID(), function1, args1);
        if (ret.isNumber())
        {
            defense = ret.toInt();
        }
    }
    return defense;
}

QString Terrain::getMinimapIcon()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMiniMapIcon";
    QJSValueList args1;
    QJSValue ret = pInterpreter->doFunction(terrainID, function1, args1);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "";
    }
}

qint32 Terrain::getFirerangeModifier(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getFirerangeModifier";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(pUnit);
    args1 << obj2;
    QJSValue erg = pInterpreter->doFunction(terrainID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Terrain::getMinFirerangeModifier(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMinFirerangeModifier";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(pUnit);
    args1 << obj2;
    QJSValue erg = pInterpreter->doFunction(terrainID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Terrain::getDefense(Unit* pUnit)
{
    qint32 defense = getBaseDefense();
    if (pUnit != nullptr)
    {
        if (!pUnit->useTerrainDefense())
        {
            return 0;
        }
        defense += pUnit->getTerrainDefenseModifier(QPoint(x, y));
        spGameMap pMap = GameMap::getInstance();
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = pMap->getPlayer(i);
            if (pPlayer->isEnemyUnit(pUnit) && !pPlayer->getIsDefeated())
            {
                CO* pCO = pPlayer->getCO(0);
                if (pCO != nullptr)
                {
                    defense += pCO->getEnemyTerrainDefenseModifier(pUnit, QPoint(x, y));
                }
                pCO = pPlayer->getCO(1);
                if (pCO != nullptr)
                {
                    defense += pCO->getEnemyTerrainDefenseModifier(pUnit, QPoint(x, y));
                }
            }
        }
    }
    // defense can't be negative
    if (defense < 0)
    {
        defense = 0;
    }
    return defense;
}

void Terrain::setBuilding(Building* pBuilding)
{
    if (m_Building.get() != nullptr)
    {
        removeBuilding();
    }
    if (pBuilding != nullptr)
    {
        m_Building = pBuilding;
        pBuilding->setPriority(static_cast<qint16>(DrawPriority::Building));
        if (x >= 0 && y >= 0)
        {
            pBuilding->setTerrain(GameMap::getInstance()->getTerrain(Terrain::x, Terrain::y));
        }
        this->addChild(pBuilding);
        if (x >= 0 && y >= 0)
        {
            createBuildingDownStream();
        }
    }
    // remove current unit to avoid strange impact :)
    setUnit(nullptr);
}

void Terrain::removeBuilding()
{
    if (m_Building.get() != nullptr)
    {
        if (m_Building->getTerrain() == this)
        {
            // remove it
            m_Building->detach();
            qint32 width = m_Building->getBuildingWidth();
            qint32 heigth = m_Building->getBuildingHeigth();
            spGameMap pMap = GameMap::getInstance();
            // remove pointers
            for (qint32 x1 = 0; x1 < width; x1++)
            {
                for (qint32 y1 = 0; y1 < heigth; y1++)
                {
                    // remove down stream on all other fields
                    if (!((x1 == 0) && (y1 == 0)) &&
                        pMap->onMap(x - x1, y - y1))
                    {
                        pMap->getTerrain(x - x1, y - y1)->removeDownstream();
                    }
                }
            }
            m_Building = nullptr;
        }
        else
        {
            // remove building from base terrain instead of us
            m_Building->getTerrain()->removeBuilding();
        }
    }
}

void Terrain::setSpBuilding(spBuilding pBuilding, bool OnlyDownStream)
{
    if (m_Building.get() != nullptr)
    {
        removeBuilding();
    }
    if (pBuilding.get() != nullptr)
    {
        m_Building = pBuilding;
        pBuilding->setPriority(static_cast<qint16>(DrawPriority::Building));
        if (!OnlyDownStream)
        {
            pBuilding->setTerrain(GameMap::getInstance()->getTerrain(Terrain::x, Terrain::y));
            this->addChild(pBuilding);
        }
    }
    // remove current unit to avoid strange impact :)
    setUnit(nullptr);
}

void Terrain::loadBuilding(QString buildingID)
{
    if (m_Building.get() != nullptr)
    {
        removeBuilding();
    }
    m_Building = new Building(buildingID);
    m_Building->updateBuildingSprites(false);
    m_Building->setPriority(static_cast<qint16>(DrawPriority::Building));
    m_Building->setTerrain(GameMap::getInstance()->getTerrain(Terrain::x, Terrain::y));
    this->addChild(m_Building);
    createBuildingDownStream();
}

void Terrain::setUnit(spUnit pUnit)
{
    // remove current unit on this field
    if (m_Unit.get() != nullptr)
    {
        m_Unit->setTerrain(nullptr);
        m_Unit->detach();
        m_Unit = nullptr;
    }
    if (pUnit.get() != nullptr)
    {
        // add unit to this terrain
        m_Unit = pUnit;
        // remove it from last terrain
        Terrain* pTerrain = m_Unit->getTerrain();
        if (pTerrain != nullptr)
        {
            pTerrain->setUnit(nullptr);
        }
        // add Terrain to unit and unit to drawing actor
        pUnit->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + static_cast<qint32>(Terrain::y) + 2);
        pUnit->setTerrain(GameMap::getInstance()->getTerrain(Terrain::x, Terrain::y));
        pUnit->setPosition(Terrain::x * GameMap::getImageSize(), Terrain::y * GameMap::getImageSize());

        spGameMap pMap = GameMap::getInstance();
        if (pMap.get())
        {
            Player* pPlayer = pMap->getCurrentViewPlayer();
            Player* pCurrentPlayer = pMap->getCurrentPlayer();
            if (pPlayer != pCurrentPlayer &&
                pPlayer != nullptr &&
                pPlayer->getFieldVisibleLoaded() &&
                !pPlayer->getFieldVisible(Terrain::x, Terrain::y))
            {
                pUnit->setVisible(false);
            }
            pMap->addChild(pUnit);
        }
    }
}

spUnit Terrain::getSpUnit()
{
    return m_Unit;
}

QString Terrain::getID()
{
    if (m_Building.get() == nullptr)
    {
        return terrainID;
    }
    else
    {
        return m_Building->getBuildingID();
    }
}

QString Terrain::getTerrainAnimationBase()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainAnimationBase";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(nullptr);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue erg = pInterpreter->doFunction(terrainID, function1, args1);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

QString Terrain::getTerrainAnimationForeground()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainAnimationForeground";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(nullptr);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue erg = pInterpreter->doFunction(terrainID, function1, args1);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

QString Terrain::getTerrainAnimationBackground()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainAnimationBackground";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(nullptr);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue erg = pInterpreter->doFunction(terrainID, function1, args1);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

float Terrain::getTerrainAnimationMoveSpeed()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainAnimationMoveSpeed";
    QJSValueList args;
    QJSValue erg = pInterpreter->doFunction(terrainID, function1, args);
    if (erg.isNumber())
    {
        return erg.toNumber();
    }
    else
    {
        return 0.0f;
    }
}

QStringList Terrain::getTerrainSprites()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainSprites";
    QJSValue erg = pInterpreter->doFunction(terrainID, function1);
    return erg.toVariant().toStringList();
}

QString Terrain::getTerrainID() const
{
    return terrainID;
}

qint32 Terrain::getX() const
{
    return x;
}

void Terrain::setX(const qint32 &value)
{
    x = value;
}

qint32 Terrain::getY() const
{
    return y;
}

void Terrain::setY(const qint32 &value)
{
    y = value;
}

qint32 Terrain::getVision(Player* pPlayer)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getVision";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(pPlayer);
    args1 << obj1;
    QJSValue ret = pInterpreter->doFunction(terrainID, function1, args1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

TerrainFindingSystem* Terrain::createTerrainFindingSystem()
{
    TerrainFindingSystem* pPfs = new TerrainFindingSystem(getID(), getX(), getY());
    pPfs->explore();
    return pPfs;
}

bool Terrain::getVisionHide(Player* pPlayer)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getVisionHide";
    QJSValue ret = pInterpreter->doFunction(terrainID, function1);
    spGameMap pMap = GameMap::getInstance();
    if (ret.isBool())
    {
        bool value = ret.toBool();
        if (!value &&
            m_Building.get() != nullptr &&
            pPlayer != nullptr &&
            pPlayer->isEnemy(m_Building->getOwner()) &&
            pMap->getGameRules()->getBuildingVisionHide())
        {
            value = m_Building->getVisionHide();
        }
        if (value && pPlayer != nullptr)
        {
            CO* pCO = pPlayer->getCO(0);
            if (pCO != nullptr)
            {
                if (pCO->getPerfectVision())
                {
                    return false;
                }
            }
            pCO = pPlayer->getCO(1);
            if (pCO != nullptr)
            {
                if (pCO->getPerfectVision())
                {
                    return false;
                }
            }
        }
        return value;
    }
    else
    {
        return false;
    }
}

qint32 Terrain::getBonusVision(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBonusVision";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    QJSValue ret = pInterpreter->doFunction(terrainID, function1, args1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

void Terrain::startOfTurn()
{
    if (m_hasStartOfTurn)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "startOfTurn";
        QJSValueList args1;
        QJSValue obj1 = pInterpreter->newQObject(this);
        args1 << obj1;
        pInterpreter->doFunction(terrainID, function1, args1);
    }
}

qint32 Terrain::getOffensiveFieldBonus(Unit* pAttacker, QPoint atkPosition,Unit* pDefender,  QPoint defPosition, bool isDefender)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getOffensiveFieldBonus";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pInterpreter->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkPosition.x();
    args1 << atkPosition.y();
    QJSValue obj2 = pInterpreter->newQObject(pDefender);
    args1 << obj2;
    args1 << defPosition.x();
    args1 << defPosition.y();
    args1 << isDefender;
    qint32 ergValue = 0;
    QJSValue erg = pInterpreter->doFunction(terrainID, function1, args1);
    if (erg.isNumber())
    {
        ergValue += erg.toInt();
    }
    return ergValue;
}

qint32 Terrain::getDeffensiveFieldBonus(Unit* pAttacker, QPoint atkPosition, Unit* pDefender, QPoint defPosition, bool isDefender)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDeffensiveFieldBonus";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pInterpreter->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkPosition.x();
    args1 << atkPosition.y();
    QJSValue obj2 = pInterpreter->newQObject(pDefender);
    args1 << obj2;
    args1 << defPosition.x();
    args1 << defPosition.y();
    args1 << isDefender;
    qint32 ergValue = 0;
    QJSValue erg = pInterpreter->doFunction(terrainID, function1, args1);
    if (erg.isNumber())
    {
        ergValue += erg.toInt();
    }
    return ergValue;
}

void Terrain::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();

    pStream << m_terrainSpriteName;
    pStream << m_FixedSprite;

    pStream << terrainID;
    if (m_pBaseTerrain.get() == nullptr)
    {
        pStream << false;
    }
    else
    {
        pStream << true;
        m_pBaseTerrain->serializeObject(pStream);
    }
    if (m_Building.get() == nullptr)
    {
        pStream << false;
    }
    else
    {
        if (m_Building->getTerrain() == this)
        {
            pStream << true;
            m_Building->serializeObject(pStream);
        }
        else
        {
            pStream << false;
        }
    }
    if (m_Unit.get() == nullptr)
    {
        pStream << false;
    }
    else
    {
        pStream << true;
        m_Unit->serializeObject(pStream);
    }
    pStream << hp;

    pStream << terrainName;
    pStream << m_terrainDescription;
    pStream << m_hasStartOfTurn;
    m_Variables.serializeObject(pStream);
}

void Terrain::deserializeObject(QDataStream& pStream)
{
    deserializer(pStream, false);
}

void Terrain::deserializer(QDataStream& pStream, bool fast)
{
    qint32 version = 0;
    pStream >> version;
    if (version > 3)
    {
        pStream >> m_terrainSpriteName;
        pStream >> m_FixedSprite;
    }
    if (version > 2)
    {
        pStream >> terrainID;
    }
    else
    {
        char* id;
        pStream >> id;
        terrainID = id;
    }
    if (!fast)
    {
        init();
    }
    bool hasBaseTerrain = false;
    pStream >> hasBaseTerrain;
    if (hasBaseTerrain)
    {
        m_pBaseTerrain = createTerrain("", x, y, "");
        m_pBaseTerrain->deserializer(pStream, fast);
        if (!fast)
        {
            m_pBaseTerrain->setPriority(static_cast<qint16>(DrawPriority::Terrain));
            m_pBaseTerrain->setPosition(0, 0);
            if (m_pBaseTerrain->isValid())
            {
                this->addChild(m_pBaseTerrain);
            }
            else
            {
                m_pBaseTerrain = nullptr;
            }
        }
    }
    bool hasBuilding = false;
    pStream >> hasBuilding;
    if (hasBuilding)
    {
        m_Building = new Building("");
        m_Building->deserializer(pStream, fast);

        if (m_Building->isValid())
        {
            if (!fast)
            {
                m_Building->setPriority(static_cast<qint16>(DrawPriority::Building));
                this->addChild(m_Building);
            }
            m_Building->setTerrain(GameMap::getInstance()->getTerrain(Terrain::x, Terrain::y));
            createBuildingDownStream();
        }
        else
        {
            m_Building = nullptr;
        }
    }
    bool hasUnit = false;
    pStream >> hasUnit;
    if (hasUnit)
    {
        m_Unit = new Unit("", nullptr, false);
        m_Unit->deserializer(pStream, fast);
        if (m_Unit->isValid())
        {
            if (!fast)
            {
                setUnit(m_Unit);
            }
        }
        else
        {
            m_Unit = nullptr;
        }
    }
    if (version > 1)
    {
        qint32 newHp = 0;
        pStream >> newHp;
        if (newHp > 0 && hp > 0)
        {
            hp = newHp;
        }
    }
    if (version > 4)
    {
        pStream >> terrainName;
        pStream >> m_terrainDescription;
    }
    if (version > 5)
    {
        pStream >> m_hasStartOfTurn;
    }
    if (version > 6)
    {
        m_Variables.deserializeObject(pStream);
    }
}

void Terrain::createBuildingDownStream()
{
    qint32 width = m_Building->getBuildingWidth();
    qint32 heigth = m_Building->getBuildingHeigth();
    spGameMap pMap = GameMap::getInstance();
    // recreate pointers
    for (qint32 x1 = 0; x1 < width; x1++)
    {
        for (qint32 y1 = 0; y1 < heigth; y1++)
        {
            // create down stream on all other fields
            if (!((x1 == 0) && (y1 == 0)))
            {
                pMap->getTerrain(x - x1, y - y1)->setSpBuilding(m_Building, true);
            }
        }
    }
}

void Terrain::removeDownstream()
{
    m_Building = nullptr;
}
