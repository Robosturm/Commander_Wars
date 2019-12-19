
#include "building.h"

#include "game/player.h"

#include "game/terrain.h"

#include "coreengine/console.h"

#include "game/gamemap.h"

#include "game/unit.h"
#include "game/co.h"

#include "resource_management/buildingspritemanager.h"

#include "coreengine/qmlvector.h"

#include "ai/coreai.h"

const float Building::animationSpeed = 4.0f;

Building::Building(QString BuildingID)
    : m_BuildingID(BuildingID),
      m_pOwner(nullptr),
      m_pTerrain(nullptr)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    if (m_BuildingID != "")
    {
        init();
    }
}

void Building::init()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function = "init";
    QJSValueList args;
    QJSValue objArg = pApp->getInterpreter()->newQObject(this);
    args << objArg;
    pApp->getInterpreter()->doFunction(m_BuildingID, function, args);
}

QPoint Building::getOffset(Terrain* pTerrain)
{
    return QPoint(m_pTerrain->getX() - pTerrain->getX(),
                  m_pTerrain->getY() - pTerrain->getY());
}

QStringList Building::getBaseTerrain()
{
    Mainapp* pApp = Mainapp::getInstance();
    QJSValueList args;
    QString function = "getBaseTerrain";
    QJSValue objArg = pApp->getInterpreter()->newQObject(this);
    args << objArg;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function, args);
    return ret.toVariant().toStringList();
}

QString Building::getDescription()
{
    Mainapp* pApp = Mainapp::getInstance();
    QJSValueList args;
    // load sprite of the base terrain
    QString function = "getDescription";
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function, args);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "";
    }
}

void Building::setUnitOwner(Unit* pUnit)
{
    setOwner(pUnit->getOwner());
}

void Building::setOwner(Player* pOwner)
{
    // change ownership
    m_pOwner = pOwner;
    // update sprites :)
    updateBuildingSprites(false);
}

Player* Building::getOwner()
{
    return m_pOwner;
}

qint32 Building::getOwnerID()
{
    if (m_pOwner != nullptr)
    {
        return m_pOwner->getPlayerID();
    }
    return -1;
}

void Building::loadSprite(QString spriteID, bool addPlayerColor)
{
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pBuildingSpriteManager->getResAnim(spriteID);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(static_cast<qint64>(pAnim->getTotalFrames() * GameMap::frameTime * animationSpeed)), -1);
            pSprite->addTween(tween);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }
        // repaint the building?
        if (addPlayerColor && m_pOwner != nullptr)
        {
            QColor color = m_pOwner->getColor();
            oxygine::Sprite::TweenColor tweenColor(color);
            oxygine::spTween tween = oxygine::createTween(tweenColor, oxygine::timeMS(1));
            pSprite->addTween(tween);
        }
        else if (addPlayerColor)
        {
            oxygine::Sprite::TweenColor tweenColor(QColor(150, 150, 150, 255));
            oxygine::spTween tween = oxygine::createTween(tweenColor, oxygine::timeMS(1));
            pSprite->addTween(tween);
        }
        qint32 width = getBuildingWidth();
        qint32 heigth = getBuildingHeigth();
        if (width == 1 && heigth == 1)
        {
            pSprite->setScale((GameMap::Imagesize) / pAnim->getWidth());
            pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::Imagesize) / 2, -(pSprite->getScaledHeight() - GameMap::Imagesize));
        }
        else
        {
            pSprite->setScale(((GameMap::Imagesize ) * width) / pAnim->getWidth());
            pSprite->setPosition(-pSprite->getScaledWidth() + GameMap::Imagesize, -pSprite->getScaledHeight() + GameMap::Imagesize);
        }
        this->addChild(pSprite);
        m_pBuildingSprites.append(pSprite);
        m_addPlayerColor.append(addPlayerColor);
    }
    else
    {
        Console::print("Unable to load terrain sprite: " + spriteID, Console::eERROR);
    }
}

void Building::updatePlayerColor(bool visible)
{
    if (m_pOwner != nullptr)
    {
        if (neutralLoaded && (visible || alwaysVisble))
        {
            updateBuildingSprites(false);
        }
        else
        {
            if (visible || alwaysVisble)
            {
                for (qint32 i = 0; i < m_pBuildingSprites.size(); i++)
                {
                    if (m_addPlayerColor[i])
                    {
                        QColor color = m_pOwner->getColor();
                        m_pBuildingSprites[i]->setColor(color);
                    }
                }
            }
            else if (!neutralLoaded)
            {
                updateBuildingSprites(true);
            }
        }
    }
    else if (!neutralLoaded)
    {
        updateBuildingSprites(true);
    }
}

qint32 Building::getVision()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getVision";
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

void Building::updateBuildingSprites(bool neutral)
{
    Mainapp* pApp = Mainapp::getInstance();
    for (qint32 i = 0; i < m_pBuildingSprites.size(); i++)
    {
        this->removeChild(m_pBuildingSprites[i]);
    }
    // call the js loader function to do the rest
    m_pBuildingSprites.clear();
    m_addPlayerColor.clear();
    QString function1 = "loadSprites";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    args1 << neutral;
    pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    neutralLoaded = neutral;
}

bool Building::canBuildingBePlaced(Terrain* pTerrain)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "canBuildingBePlaced";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pTerrain);
    args1 << obj1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    else
    {
        return false;
    }
}

QString Building::getName()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getName";
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "";
    }
}

quint32 Building::getBaseIncome() const
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getBaseIncome";
    QJSValueList args1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (ret.isNumber())
    {
        return ret.toUInt();
    }
    else
    {
        return 0;
    }
}

qint32 Building::getIncome()
{
    qint32 income = static_cast<qint32>(getBaseIncome());
    if (m_pOwner != nullptr)
    {
        income = income * m_pOwner->getFundsModifier();
        qint32 modifier = 0;
        CO* pCO = m_pOwner->getCO(0);
        if (pCO != nullptr)
        {
            modifier += pCO->getBonusIncome(this, income);
        }
        pCO = m_pOwner->getCO(1);
        if (pCO != nullptr)
        {
            modifier += pCO->getBonusIncome(this, income);
        }
        income = static_cast<qint32>(income) + modifier;

        modifier = 0;
        GameMap* pMap = GameMap::getInstance();
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = pMap->getPlayer(i);
            if (pPlayer != nullptr &&
                m_pOwner->isEnemy(pPlayer))
            {
                modifier -= pPlayer->getIncomeReduction(this, income);
            }
        }
        income += modifier;
    }
    if (income < 0)
    {
        income = 0;
    }
    return income;
}

QString Building::getMinimapIcon()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getMiniMapIcon";
    QJSValueList args1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "";
    }
}

qint32 Building::getX() const
{
    return m_pTerrain->getX();
}

qint32 Building::getY() const
{
    return m_pTerrain->getY();
}

QStringList Building::getActionList()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getActions";
    QJSValueList args1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (ret.isString())
    {
        QStringList retList = ret.toString().split(",");
        QStringList actionModifierList;
        if (m_pOwner != nullptr)
        {
            CO* pCO = m_pOwner->getCO(0);
            if (pCO != nullptr)
            {
                QString result = pCO->getAdditionalBuildingActions(this);
                if (!result.isEmpty())
                {
                    actionModifierList += result.split(",");
                }
            }
            pCO = m_pOwner->getCO(1);
            if (pCO != nullptr)
            {
                QString result = pCO->getAdditionalBuildingActions(this);
                if (!result.isEmpty())
                {
                    actionModifierList += result.split(",");
                }
            }


            GameMap* pMap = GameMap::getInstance();
            for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
            {
                Player* pPlayer = pMap->getPlayer(i);
                if (pPlayer != nullptr &&
                    m_pOwner->isEnemy(pPlayer))
                {
                    pCO = pPlayer->getCO(0);
                    if (pCO != nullptr)
                    {
                        QString result = pCO->getAdditionalBuildingActions(this);
                        if (!result.isEmpty())
                        {
                            actionModifierList += result.split(",");
                        }
                    }
                    pCO = pPlayer->getCO(1);
                    if (pCO != nullptr)
                    {
                        QString result = pCO->getAdditionalBuildingActions(this);
                        if (!result.isEmpty())
                        {
                            actionModifierList += result.split(",");
                        }
                    }
                }
            }
        }
        for (qint32 i = 0; i < actionModifierList.size(); i++)
        {
            QString action = actionModifierList[i];
            if (!action.startsWith("-"))
            {
                if (!retList.contains(action))
                {
                    retList.append(action);
                }
            }
        }
        for (qint32 i = 0; i < actionModifierList.size(); i++)
        {
            QString action = actionModifierList[i];
            if (action.startsWith("-"))
            {
                qint32 index = retList.indexOf(action.replace("-", ""));
                if (index >= 0)
                {
                    retList.removeAt(index);
                }
            }
        }
        return retList;
    }
    else
    {
        return QStringList();
    }
}

QStringList  Building::getConstructionList()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getConstructionList";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    QStringList buildList = ret.toVariant().toStringList();
    QStringList coUnits;
    if (m_pOwner != nullptr)
    {
        coUnits = m_pOwner->getCOUnits(this);
    }
    if (coUnits.size() > 0)
    {
        for (qint32 i = 0; i < coUnits.size(); i++)
        {
            if (coUnits.startsWith("-"))
            {
                buildList.removeAll(coUnits[i].replace("-", ""));
            }
            else
            {
                buildList.append(coUnits[i]);
            }
        }
    }
    QStringList returnList;
    if (m_pOwner != nullptr)
    {
        QStringList playerBuildList = m_pOwner->getBuildList();
        for (qint32 i = 0; i < buildList.size(); i++)
        {
            QString unitID = buildList[i];
            if (playerBuildList.contains(unitID))
            {
                returnList.append(unitID);
            }

        }
        return returnList;
    }
    return buildList;
}

void Building::startOfTurn()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "startOfTurn";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
}

qint32 Building::getOffensiveBonus()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getOffensiveBonus";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Building::getDefensiveBonus()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getDefensiveBonus";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Building::getVisionBonus()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getVisionBonus";
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Building::getBuildingWidth()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getBuildingWidth";
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Building::getBuildingHeigth()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getBuildingHeigth";
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

bool Building::getIsAttackable(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getIsAttackable";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    args1 << x;
    args1 << y;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    else
    {
        return false;
    }
}
QmlVectorPoint* Building::getActionTargetFields()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getActionTargetFields";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (ret.isQObject())
    {
        return dynamic_cast<QmlVectorPoint*>(ret.toQObject());
    }
    else
    {
        return nullptr;
    }
}
QPoint Building::getActionTargetOffset()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getActionTargetOffset";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    return ret.toVariant().toPoint();
}

float Building::getDamage(Unit* pUnit)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getDamage";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj2;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (ret.isNumber())
    {
        return ret.toNumber();
    }
    return 0.0f;
}

GameEnums::BuildingTarget Building::getBuildingTargets()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getBuildingTargets";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (ret.isNumber())
    {
        return static_cast<GameEnums::BuildingTarget>(ret.toInt());
    }
    return GameEnums::BuildingTarget_All;
}

bool Building::getNeutralLoaded() const
{
    return neutralLoaded;
}

bool Building::getAlwaysVisble() const
{
    return alwaysVisble;
}

void Building::setAlwaysVisble(bool value)
{
    alwaysVisble = value;
}

QString Building::getTerrainAnimationBase()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getTerrainAnimationBase";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(nullptr);
    args1 << obj1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(m_pTerrain);
    args1 << obj2;
    QJSValue erg = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

QString Building::getTerrainAnimationForeground()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getTerrainAnimationForeground";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(nullptr);
    args1 << obj1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(m_pTerrain);
    args1 << obj2;
    QJSValue erg = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

bool Building::canRepair(Unit* pUnit)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getConstructionList";
    QJSValue erg = pApp->getInterpreter()->doFunction(m_BuildingID, function1);
    return erg.toVariant().toStringList().contains(pUnit->getUnitID());
}

bool Building::isCaptureOrMissileBuilding()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getCapturableBuildings";
    QJSValue erg = pApp->getInterpreter()->doFunction(CoreAI::ACTION_CAPTURE, function1);
    bool capturable = erg.toVariant().toStringList().contains(m_BuildingID);
    if (!capturable)
    {
        function1 = "getMissileBuildings";
        erg = pApp->getInterpreter()->doFunction(CoreAI::ACTION_MISSILE, function1);
        capturable = erg.toVariant().toStringList().contains(m_BuildingID);
    }
    return capturable;
}

bool Building::isProductionBuilding()
{
    if (getActionList().contains(CoreAI::ACTION_BUILD_UNITS))
    {
        return true;
    }
    return false;
}

QString Building::getTerrainAnimationBackground()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getTerrainAnimationBackground";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(nullptr);
    args1 << obj1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(m_pTerrain);
    args1 << obj2;
    QJSValue erg = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

Terrain* Building::getTerrain()
{
    return m_pTerrain;
}

qint32 Building::getFireCount() const
{
    return fireCount;
}

void Building::setFireCount(const qint32 &value)
{
    fireCount = value;
}

qint32 Building::getHp() const
{
    return m_Hp;
}

void Building::setHp(const qint32 &Hp)
{
    m_Hp = Hp;
}

void Building::serializeObject(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << m_BuildingID;
    if (m_pOwner == nullptr)
    {
        pStream << static_cast<qint32>(-1);
    }
    else
    {
        pStream << static_cast<qint32>(m_pOwner->getPlayerID());
    }
    pStream << m_Hp;
    pStream << fireCount;
    m_Variables.serializeObject(pStream);
}

void Building::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    if (version > 3)
    {
        pStream >> m_BuildingID;
    }
    else
    {
        char* id;
        pStream >> id;
        m_BuildingID = id;
    }
    init();
    qint32 playerID = -1;
    pStream >> playerID;
    m_pOwner = GameMap::getInstance()->getPlayer(playerID);
    if (version > 1)
    {
        qint32 newHp = 0;
        pStream >> newHp;
        if (newHp > 0 && m_Hp > 0)
        {
            m_Hp = newHp;
        }
        pStream >> fireCount;
    }
    if (version > 2)
    {
        m_Variables.deserializeObject(pStream);
    }
}
