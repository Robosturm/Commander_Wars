#include "terrain.h"

#include "coreengine/console.h"

#include "coreengine/mainapp.h"

#include "resource_management/terrainmanager.h"

#include "game/gamemap.h"

#include "game/terrainfindingsystem.h"

#include <QFileInfo>

#include "game/building.h"

#include "game/unit.h"

#include "game/player.h"
#include "game/co.h"

spTerrain Terrain::createTerrain(const QString& terrainID, qint32 x, qint32 y)
{
    spTerrain pTerrain = new Terrain(terrainID, x, y);
    if (terrainID != "")
    {
        pTerrain->createBaseTerrain();
    }
    return pTerrain;
}

Terrain::Terrain(const QString& terrainID, qint32 x, qint32 y)
    : terrainID(terrainID),
      x(x),
      y(y),
      m_Building{nullptr}
{
    Interpreter::setCppOwnerShip(this);
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Terrain));
    Mainapp* pApp = Mainapp::getInstance();
    QJSValue obj = pApp->getInterpreter()->getGlobal(terrainID);
    // check if the js-script was loaded already
    // otherwise do load it
    bool terrainExists = true;
    if (terrainID != "")
    {
        if (!obj.isObject())
        {
            terrainExists = TerrainManager::getInstance()->loadTerrain(terrainID);
        }
        if (terrainExists)
        {
            init();
        }
        else
        {
            Console::print(tr("Unable to load Terrain ") + terrainID, Console::eFATAL);
        }
    }
}

void Terrain::init()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function = "init";
    QJSValueList args;
    QJSValue objArg = pApp->getInterpreter()->newQObject(this);
    args << objArg;
    pApp->getInterpreter()->doFunction(terrainID, function, args);
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

Terrain::~Terrain()
{

}

void Terrain::syncAnimation()
{
    if (m_pTerrainSprite.get() != nullptr)
    {
        oxygine::spTween pTween = m_pTerrainSprite->getFirstTween();
        while (pTween.get() != nullptr)
        {
            pTween->reset();
            pTween->init(pTween->getDuration(), pTween->getLoops());
            // remove it
            m_pTerrainSprite->removeTween(pTween);
            // restart it
            m_pTerrainSprite->addTween(pTween);
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

void Terrain::createBaseTerrain()
{
    Mainapp* pApp = Mainapp::getInstance();
    QJSValueList args;
    QJSValue obj = pApp->getInterpreter()->newQObject(this);
    args << obj;
    // load sprite of the base terrain
    QString function = "loadBaseTerrain";
    pApp->getInterpreter()->doFunction(terrainID, function, args);
    if (m_pBaseTerrain.get() != nullptr)
    {
        m_pBaseTerrain->createBaseTerrain();
    }
}

void Terrain::setBaseTerrain(spTerrain terrain)
{
    if (m_pBaseTerrain.get() != nullptr)
    {
        this->removeChild(m_pBaseTerrain);
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
        this->removeChild(m_pTerrainSprite);
        m_pTerrainSprite = nullptr;
    }
    if (m_pOverlaySprites.size() > 0)
    {
        for (qint32 i = 0; i < m_pOverlaySprites.size(); i++)
        {
            this->removeChild(m_pOverlaySprites[i]);
        }
        m_pOverlaySprites.clear();
    }
    // load sub terrain
    if (m_pBaseTerrain.get() != nullptr)
    {
        m_pBaseTerrain->loadSprites();
    }
    // load main terrain
    Mainapp* pApp = Mainapp::getInstance();
    if (m_FixedSprite)
    {
        loadBaseSprite(m_terrainSpriteName);
    }
    else
    {
        QString function1 = "loadBaseSprite";
        QJSValueList args1;
        QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
        args1 << obj1;
        pApp->getInterpreter()->doFunction(terrainID, function1, args1);
    }
    // next call starting by 0 again
    QString function2 = "loadOverlaySprite";
    QJSValueList args2;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args2 << obj2;
    pApp->getInterpreter()->doFunction(terrainID, function2, args2);
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
    oxygine::ResAnim* pAnim = pTerrainManager->getResAnim(spriteID.toStdString());
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
            pSprite->addTween(tween);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }
        pSprite->setScale(GameMap::Imagesize / pAnim->getWidth());

        pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::Imagesize) / 2, -(pSprite->getScaledHeight() - GameMap::Imagesize));
        this->addChild(pSprite);
        m_terrainSpriteName = spriteID;
        m_pTerrainSprite = pSprite;
    }
    else
    {
        Console::print("Unable to load terrain sprite: " + spriteID, Console::eERROR);
    }
}

QString Terrain::getSurroundings(QString list, bool useBaseTerrainID, bool blacklist, qint32 searchType, bool useMapBorder, bool useBuildingID)
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
        if (pGameMap->onMap(curX, curY))
        {
            QString neighbourID = "";
            if (useBuildingID)
            {
                neighbourID = pGameMap->getTerrain(curX, curY)->getID();
            }
            else if (useBaseTerrainID)
            {
                neighbourID = pGameMap->getTerrain(curX, curY)->getBaseTerrainID();
            }
            else
            {
                neighbourID = pGameMap->getTerrain(curX, curY)->getTerrainID();
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
    oxygine::ResAnim* pAnim = pTerrainManager->getResAnim(spriteID.toStdString());
    oxygine::spSprite pSprite = new oxygine::Sprite();
    if (pAnim->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnim);
    }
    pSprite->setScale(GameMap::Imagesize / pAnim->getWidth());
    pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::Imagesize) / 2, -(pSprite->getScaledHeight() - GameMap::Imagesize));
    this->addChild(pSprite);
    m_pOverlaySprites.append(pSprite);
}

qint32 Terrain::getBaseDefense()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getDefense";
    QJSValueList args1;
    qint32 defense = 0;
    if (m_Building.get() == nullptr)
    {
        QJSValue ret = pApp->getInterpreter()->doFunction(terrainID, function1, args1);
        if (ret.isNumber())
        {
            defense = ret.toInt();
        }
    }
    else
    {
        QJSValue ret = pApp->getInterpreter()->doFunction(m_Building->getBuildingID(), function1, args1);
        if (ret.isNumber())
        {
            defense = ret.toInt();
        }
    }
    return defense;
}

QString Terrain::getMinimapIcon()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getMiniMapIcon";
    QJSValueList args1;
    QJSValue ret = pApp->getInterpreter()->doFunction(terrainID, function1, args1);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "";
    }
}

qint32 Terrain::getDefense(Unit* pUnit)
{
    qint32 defense = getBaseDefense();
    if (pUnit != nullptr)
    {
        defense += pUnit->getTerrainDefenseModifier(QPoint(x, y));
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
        pBuilding->setTerrain(GameMap::getInstance()->getTerrain(Terrain::x, Terrain::y));
        this->addChild(pBuilding);
        createBuildingDownStream();
    }
    // delete current unit to avoid strange impact :)
    setUnit(nullptr);
}

void Terrain::removeBuilding()
{
    if (m_Building->getTerrain() == this)
    {
        // delete it
        this->removeChild(m_Building);
        qint32 width = m_Building->getBuildingWidth();
        qint32 heigth = m_Building->getBuildingHeigth();
        GameMap* pMap = GameMap::getInstance();
        // delete pointers
        for (qint32 x1 = 0; x1 < width; x1++)
        {
            for (qint32 y1 = 0; y1 < heigth; y1++)
            {
                // delete down stream on all other fields
                if (!((x1 == 0) && (y1 == 0)))
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
    // delete current unit to avoid strange impact :)
    setUnit(nullptr);
}

void Terrain::loadBuilding(QString buildingID)
{
    if (m_Building.get() != nullptr)
    {
        removeBuilding();
    }
    m_Building = new Building(buildingID);
    m_Building->updateBuildingSprites();
    m_Building->setPriority(static_cast<qint16>(DrawPriority::Building));
    m_Building->setTerrain(GameMap::getInstance()->getTerrain(Terrain::x, Terrain::y));
    this->addChild(m_Building);
    createBuildingDownStream();
}

void Terrain::setUnit(spUnit pUnit)
{
    if (m_Unit.get() != nullptr)
    {
        // delete it
        this->removeChild(m_Unit);
        m_Unit = nullptr;
    }
    if (pUnit.get() != nullptr)
    {
        m_Unit = pUnit;
        pUnit->setPriority(static_cast<qint16>(DrawPriority::Unit));
        pUnit->setTerrain(GameMap::getInstance()->getTerrain(Terrain::x, Terrain::y));
        this->addChild(pUnit);
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

qint32 Terrain::getVision()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getVision";
    QJSValue ret = pApp->getInterpreter()->doFunction(terrainID, function1);
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

bool Terrain::getVisionHide()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getVisionHide";
    QJSValue ret = pApp->getInterpreter()->doFunction(terrainID, function1);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    else
    {
        return false;
    }
}

qint32 Terrain::getBonusVision(Unit* pUnit)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getBonusVision";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    QJSValue ret = pApp->getInterpreter()->doFunction(terrainID, function1, args1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

void Terrain::serialize(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << terrainID.toStdString().c_str();
    if (m_pBaseTerrain.get() == nullptr)
    {
        pStream << false;
    }
    else
    {
        pStream << true;
        m_pBaseTerrain->serialize(pStream);
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
            m_Building->serialize(pStream);
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
        m_Unit->serialize(pStream);
    }
    pStream << hp;
}

void Terrain::deserialize(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    char* id;
    pStream >> id;
    terrainID = id;
    init();
    bool hasBaseTerrain = false;
    pStream >> hasBaseTerrain;
    if (hasBaseTerrain)
    {
        m_pBaseTerrain = createTerrain("", x, y);
        m_pBaseTerrain->deserialize(pStream);
        m_pBaseTerrain->setPriority(static_cast<qint16>(DrawPriority::Terrain));
        m_pBaseTerrain->setPosition(0, 0);
        this->addChild(m_pBaseTerrain);
    }
    bool hasBuilding = false;
    pStream >> hasBuilding;
    if (hasBuilding)
    {
        m_Building = new Building("");
        m_Building->deserialize(pStream);
        m_Building->setPriority(static_cast<qint16>(DrawPriority::Building));
        m_Building->setTerrain(GameMap::getInstance()->getTerrain(Terrain::x, Terrain::y));
        this->addChild(m_Building);
        createBuildingDownStream();
    }
    bool hasUnit = false;
    pStream >> hasUnit;
    if (hasUnit)
    {
        m_Unit = new Unit("", nullptr);
        m_Unit->deserialize(pStream);
        m_Unit->setPriority(static_cast<qint16>(DrawPriority::Unit));
        m_Unit->setTerrain(GameMap::getInstance()->getTerrain(Terrain::x, Terrain::y));
        this->addChild(m_Unit);
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
}

void Terrain::createBuildingDownStream()
{
    qint32 width = m_Building->getBuildingWidth();
    qint32 heigth = m_Building->getBuildingHeigth();
    GameMap* pMap = GameMap::getInstance();
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
