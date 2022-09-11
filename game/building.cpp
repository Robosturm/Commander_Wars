#include "game/building.h"
#include "game/player.h"
#include "game/terrain.h"
#include "game/gamemap.h"
#include "game/unit.h"
#include "game/co.h"

#include "coreengine/console.h"
#include "coreengine/qmlvector.h"

#include "resource_management/buildingspritemanager.h"

#include "ai/coreai.h"

Building::Building(QString BuildingID, GameMap* pMap)
    : m_BuildingID(BuildingID),
      m_pOwner(nullptr),
      m_pTerrain(nullptr),
      m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("Building");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    setSize(GameMap::getImageSize(),
            GameMap::getImageSize());
    if (m_BuildingID != "")
    {
        init();
    }
}

void Building::init()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "init";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_BuildingID, function, args);
}

bool Building::getShowInEditor(QString buildingId)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getShowInEditor";

    QJSValue ret = pInterpreter->doFunction(buildingId, function1);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    return true;
}

void Building::setTerrain(Terrain* pTerrain)
{
    m_pTerrain = pTerrain;
}

bool Building::isValid()
{
    return BuildingSpriteManager::getInstance()->exists(m_BuildingID);
}

QPoint Building::getOffset(Terrain* pTerrain)
{
    if (pTerrain != nullptr)
    {
        return QPoint(m_pTerrain->Terrain::getX() - pTerrain->Terrain::getX(),
                      m_pTerrain->Terrain::getY() - pTerrain->Terrain::getY());
    }
    else
    {
        return QPoint(0, 0);
    }
}

QStringList Building::getBaseTerrain()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "getBaseTerrain";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function, args);
    QStringList retList = ret.toVariant().toStringList();
    if (retList.size() == 0)
    {
        retList.append(GameMap::PLAINS);
    }
    return retList;
}

bool Building::usesMapLayer()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args;
    QString function = "usesMapLayer";

    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function, args);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    return false;
}

void Building::scaleAndShowOnSingleTile()
{
    qint32 width = getBuildingWidth();
    qint32 heigth = getBuildingHeigth();
    if (width > 0 && heigth > 0)
    {
        setScaleX(1.0f / static_cast<float>(width));
        setScaleY(1.0f / static_cast<float>(heigth));
        setX(GameMap::getImageSize() * (width - 1) / (width));
        setY(GameMap::getImageSize() * (heigth - 1) / (heigth));
    }
}
QString Building::getDescription()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    // load sprite of the base terrain
    QString function = "getDescription";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function, args);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "";
    }
}

bool Building::getVisionHide()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getVisionHide";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    return false;
}

void Building::setUnitOwner(Unit* pUnit)
{
    if (pUnit != nullptr)
    {
        setOwner(pUnit->getOwner());
    }
}

void Building::setOwner(Player* pOwner)
{
    Player* prevOwner = m_pOwner;
    // change ownership
    m_pOwner = pOwner;
    // update sprites :)
    bool visible = true;
    
    qint32 x = Building::getX();
    qint32 y = Building::getY();

    if (m_pMap != nullptr &&
        m_pMap->getCurrentViewPlayer() != nullptr &&
        m_pMap->onMap(x, y))
    {
        visible = m_pMap->getCurrentViewPlayer()->getFieldVisible(x, y);
    }
    if (m_pBuildingSprites.size() == 0 ||
        prevOwner == nullptr)
    {
        updateBuildingSprites(false);
        updatePlayerColor(visible);
    }
    else
    {
        if (m_BuildingID == CoreAI::BUILDING_HQ)
        {
            updateBuildingSprites(false);
        }
        updatePlayerColor(visible);
    }
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

void Building::loadSprite(const QString & spriteID, bool addPlayerColor, qint32 frameTime, QPoint pos)
{
    if (addPlayerColor)
    {
        loadSpriteV2(spriteID, GameEnums::Recoloring_Mask, frameTime, pos);
    }
    else
    {
        loadSpriteV2(spriteID, GameEnums::Recoloring_None, frameTime, pos);
    }
}

void Building::loadSpriteV2(const QString & spriteID, GameEnums::Recoloring mode, qint32 frameTime, QPoint pos)
{
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pBuildingSpriteManager->getResAnim(spriteID);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(static_cast<qint64>(pAnim->getTotalFrames() * frameTime)), -1);
            pSprite->addTween(tween);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }
        // repaint the building?
        bool matrixMode = mode == GameEnums::Recoloring_Matrix;
        pSprite->setPriority(static_cast<qint32>(DrawPriority::Mask));
        if (mode == GameEnums::Recoloring_Mask && m_pOwner != nullptr)
        {
            QColor color = m_pOwner->getColor();
            pSprite->setColor(color);
        }
        else if (mode == GameEnums::Recoloring_Mask)
        {
            pSprite->setColor(QColor(150, 150, 150, 255));
        }
        else if ((mode == GameEnums::Recoloring_Table || matrixMode) && m_pOwner != nullptr)
        {
            pSprite->setColorTable(m_pOwner->getColorTableAnim(), matrixMode);
        }
        else if (mode == GameEnums::Recoloring_Table || matrixMode)
        {
            pSprite->setColorTable(Player::getNeutralTableAnim(), matrixMode);
        }
        else
        {
            pSprite->setPriority(static_cast<qint32>(DrawPriority::NoneMask));
        }
        qint32 width = getBuildingWidth();
        qint32 heigth = getBuildingHeigth();
        if (width == 1 && heigth == 1)
        {
            pSprite->setScale((GameMap::getImageSize()) / pAnim->getWidth());
            pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::getImageSize()) / 2 + pos.x(), -(pSprite->getScaledHeight() - GameMap::getImageSize()) + pos.y());
        }
        else
        {
            pSprite->setScale(((GameMap::getImageSize() ) * width) / pAnim->getWidth());
            pSprite->setPosition(-pSprite->getScaledWidth() + GameMap::getImageSize() + pos.x(), -pSprite->getScaledHeight() + GameMap::getImageSize() + pos.y());
        }
        addChild(pSprite);
        m_pBuildingSprites.append(pSprite);
        m_addPlayerColor.append(mode);
    }
    else
    {
        CONSOLE_PRINT("Unable to load building sprite: " + spriteID, Console::eDEBUG);
    }
}

void Building::unloadSprites()
{
    for (auto & sprite : m_pBuildingSprites)
    {
        sprite->detach();
    }
    m_pBuildingSprites.clear();
}

/**
 * @brief onWeatherChanged
 */
void Building::onWeatherChanged(Weather* pWeather)
{
    for (auto & weatherOverlay : m_pWeatherOverlaySprites)
    {
        weatherOverlay->detach();
    }
    m_pWeatherOverlaySprites.clear();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "onWeatherChanged";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pWeather),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_BuildingID, function1, args);
}

void Building::loadWeatherOverlaySpriteV2(const QString & spriteID, GameEnums::Recoloring mode, qint32 frameTime)
{
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pBuildingSpriteManager->getResAnim(spriteID);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(static_cast<qint64>(pAnim->getTotalFrames() * frameTime)), -1);
            pSprite->addTween(tween);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }
        // repaint the building?
        bool matrixMode = mode == GameEnums::Recoloring_Matrix;
        pSprite->setPriority(static_cast<qint32>(DrawPriority::Overlay));
        if (mode == GameEnums::Recoloring_Mask && m_pOwner != nullptr)
        {
            QColor color = m_pOwner->getColor();
            pSprite->setColor(color);
        }
        else if (mode == GameEnums::Recoloring_Mask)
        {
            pSprite->setColor(QColor(150, 150, 150, 255));
        }
        else if ((mode == GameEnums::Recoloring_Table || matrixMode) && m_pOwner != nullptr)
        {
            pSprite->setColorTable(m_pOwner->getColorTableAnim(), matrixMode);
        }
        else if (mode == GameEnums::Recoloring_Table || matrixMode)
        {
            pSprite->setColorTable(Player::getNeutralTableAnim(), matrixMode);
        }
        else
        {
            pSprite->setPriority(static_cast<qint32>(DrawPriority::OverlayNoneMask));
        }
        qint32 width = getBuildingWidth();
        qint32 heigth = getBuildingHeigth();
        if (width == 1 && heigth == 1)
        {
            pSprite->setScale((GameMap::getImageSize()) / pAnim->getWidth());
            pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::getImageSize()) / 2, -(pSprite->getScaledHeight() - GameMap::getImageSize()));
        }
        else
        {
            pSprite->setScale(((GameMap::getImageSize() ) * width) / pAnim->getWidth());
            pSprite->setPosition(-pSprite->getScaledWidth() + GameMap::getImageSize(), -pSprite->getScaledHeight() + GameMap::getImageSize());
        }
        addChild(pSprite);
        m_pWeatherOverlaySprites.append(pSprite);
    }
    else
    {
        CONSOLE_PRINT("Unable to load weather overlay sprite: " + spriteID, Console::eDEBUG);
    }
}

GameMap *Building::getMap() const
{
    return m_pMap;
}

qint32 Building::getImageSize()
{
    return GameMap::getImageSize();
}

void Building::syncAnimation(oxygine::timeMS syncTime)
{
#ifdef GRAPHICSUPPORT
    for (auto & sprite : m_pBuildingSprites)
    {
        auto & tweens = sprite->getTweens();
        for (auto & pTween : tweens)
        {
            pTween->setElapsed(syncTime);
        }
    }
    for (auto & sprite : m_pWeatherOverlaySprites)
    {
        auto & tweens = sprite->getTweens();
        for (auto & pTween : tweens)
        {
            pTween->setElapsed(syncTime);
        }
    }
#endif
}

void Building::updatePlayerColor(bool visible)
{
    if (m_pOwner != nullptr)
    {
        if (m_neutralLoaded && (visible || m_alwaysVisble))
        {
            updateBuildingSprites(false);
        }
        else
        {
            if (visible || m_alwaysVisble)
            {
                for (qint32 i = 0; i < m_pBuildingSprites.size(); i++)
                {
                    if (m_addPlayerColor[i] == GameEnums::Recoloring_Mask)
                    {
                        QColor color = m_pOwner->getColor();
                        m_pBuildingSprites[i]->setColor(color);
                    }
                    else if (m_addPlayerColor[i] == GameEnums::Recoloring_Table ||
                             m_addPlayerColor[i] == GameEnums::Recoloring_Matrix)
                    {
                        bool matrixMode = m_addPlayerColor[i] == GameEnums::Recoloring_Matrix;
                        m_pBuildingSprites[i]->setColorTable(m_pOwner->getColorTableAnim(), matrixMode);
                    }
                }
            }
            else if (!m_neutralLoaded)
            {
                updateBuildingSprites(true);
            }
        }
    }
    else if (!m_neutralLoaded)
    {
        updateBuildingSprites(true);
    }
}

qint32 Building::getVision()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getVision";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
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
    Interpreter* pInterpreter = Interpreter::getInstance();
    for (auto & sprite : m_pBuildingSprites)
    {
        sprite->detach();
    }
    // call the js loader function to do the rest
    m_pBuildingSprites.clear();
    m_addPlayerColor.clear();
    QString function1 = "loadSprites";
    QJSValueList args({pInterpreter->newQObject(this),
                       neutral,
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_BuildingID, function1, args);
    m_neutralLoaded = neutral;
    
    if (m_pMap != nullptr)
    {
        auto* pWeather = m_pMap->getGameRules()->getCurrentWeather();
        if (pWeather != nullptr)
        {
            onWeatherChanged(pWeather);
        }
    }
}

bool Building::canBuildingBePlaced(Terrain* pTerrain)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "canBuildingBePlaced";
    QJSValueList args({pInterpreter->newQObject(pTerrain),
                       pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
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
    if (m_BuildingName.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "getName";
        QJSValueList args({pInterpreter->newQObject(this),
                           pInterpreter->newQObject(m_pMap)});
        QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
        if (ret.isString())
        {
            return ret.toString();
        }
        else
        {
            return "";
        }
    }
    return m_BuildingName;
}

quint32 Building::getBaseIncome() const
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBaseIncome";
    QJSValueList args({pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
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
        
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            if (pPlayer != nullptr &&
                m_pOwner->isEnemy(pPlayer) &&
                !pPlayer->getIsDefeated())
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
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMiniMapIcon";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
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
    if (m_pTerrain != nullptr)
    {
        return m_pTerrain->Terrain::getX();
    }
    else
    {
        return -1;
    }
}

qint32 Building::getY() const
{
    if (m_pTerrain != nullptr)
    {
        return m_pTerrain->Terrain::getY();
    }
    else
    {
        return -1;
    }
}

QStringList Building::getActionList()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getActions";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
    QStringList retList;
    if (ret.isString())
    {
        retList = ret.toString().split(",");
    }
    else
    {
        retList = ret.toVariant().toStringList();
    }
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
        
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            if (pPlayer != nullptr &&
                m_pOwner->isEnemy(pPlayer) &&
                !pPlayer->getIsDefeated())
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
    for (auto & action : actionModifierList)
    {
        if (!action.startsWith("-") && !action.isEmpty())
        {
            if (!retList.contains(action))
            {
                retList.append(action);
            }
        }
    }
    for (auto & action : actionModifierList)
    {
        if (action.startsWith("-") && !action.isEmpty())
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

QList<qint32> Building::getRepairTypes()
{

    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getRepairTypes";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
    auto list = ret.toVariant().toList();
    QList<qint32> retList;
    for (auto & item : list)
    {
        retList.append(item.toInt());
    }
    return retList;
}

QStringList Building::getConstructionList()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getConstructionList";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
    QVariant var = ret.toVariant();
    QStringList buildList = var.toStringList();
    QStringList coUnits;
    if (m_pOwner != nullptr)
    {
        coUnits = m_pOwner->getCOUnits(this);
    }
    if (coUnits.size() > 0)
    {
        for (auto unitId : coUnits)
        {
            if (unitId.startsWith("-"))
            {
                buildList.removeAll(unitId.replace("-", ""));
            }
            else
            {
                buildList.append(unitId);
            }
        }
    }

    
    QStringList returnList;
    if (m_pOwner != nullptr && m_pMap != nullptr)
    {
     bool coUnits = m_pMap->getGameRules()->getCoUnits();
        QStringList playerBuildList = m_pOwner->getBuildList();
        for (auto & unitID : buildList)
        {
            function1 = "getCOSpecificUnit";
            QJSValue erg = pInterpreter->doFunction(unitID, function1, args);
            bool isCoUnit = false;
            if (erg.isBool())
            {
                isCoUnit = erg.toBool();
            }
            if (playerBuildList.contains(unitID) &&
                (!isCoUnit || coUnits))
            {
                returnList.append(unitID);
            }
        }
        return returnList;
    }
    return buildList;
}

void Building::endOfTurn()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "endOfTurn";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_BuildingID, function1, args);
}

void Building::startOfTurn()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "startOfTurn";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_BuildingID, function1, args);
}

qint32 Building::getOffensiveBonus()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getOffensiveBonus";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Building::getPowerChargeBonus()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getPowerChargeBonus";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Building::getCostModifier(const QString & id, qint32 baseCost, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getCostReduction";
    QJSValueList args({pInterpreter->newQObject(this),
                       id,
                       baseCost,
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
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
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDefensiveBonus";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
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
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getVisionBonus";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Building::getOffensiveFieldBonus(GameAction* pAction, Unit* pAttacker, QPoint atkPosition,Unit* pDefender,  QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getOffensiveFieldBonus";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pAttacker),
                       atkPosition.x(),
                       atkPosition.y(),
                       pInterpreter->newQObject(pDefender),
                       defPosition.x(),
                       defPosition.y(),
                       isDefender,
                       pInterpreter->newQObject(pAction),
                       luckMode,
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    QJSValue erg = pInterpreter->doFunction(m_BuildingID, function1, args);
    if (erg.isNumber())
    {
        ergValue += erg.toInt();
    }
    return ergValue;
}

qint32 Building::getDeffensiveFieldBonus(GameAction* pAction, Unit* pAttacker, QPoint atkPosition, Unit* pDefender, QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDeffensiveFieldBonus";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pAttacker),
                       atkPosition.x(),
                       atkPosition.y(),
                       pInterpreter->newQObject(pDefender),
                       defPosition.x(),
                       defPosition.y(),
                       isDefender,
                       pInterpreter->newQObject(pAction),
                       luckMode,
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    QJSValue erg = pInterpreter->doFunction(m_BuildingID, function1, args);
    if (erg.isNumber())
    {
        ergValue += erg.toInt();
    }
    return ergValue;
}
qint32 Building::getBuildingWidth()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBuildingWidth";
    QJSValueList args({pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
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
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBuildingHeigth";
    QJSValueList args({pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
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
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getIsAttackable";
    QJSValueList args({pInterpreter->newQObject(this),
                       x,
                       y,
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
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
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getActionTargetFields";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
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
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getActionTargetOffset";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
    return ret.toVariant().toPoint();
}

float Building::getDamage(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDamage";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
    if (ret.isNumber())
    {
        return ret.toNumber();
    }
    return 0.0f;
}

GameEnums::BuildingTarget Building::getBuildingTargets()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBuildingTargets";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
    if (ret.isNumber())
    {
        return static_cast<GameEnums::BuildingTarget>(ret.toInt());
    }
    return GameEnums::BuildingTarget_All;
}

QString Building::getBuildingName() const
{
    return m_BuildingName;
}

void Building::setBuildingName(const QString &BuildingName)
{
    m_BuildingName = BuildingName;
}

qint32 Building::getVisionHigh() const
{
    return m_VisionHigh;
}

void Building::setVisionHigh(qint32 VisionHigh)
{
    m_VisionHigh = VisionHigh;
}

qint32 Building::getTotalVisionHigh()
{
    return m_pTerrain->getTotalVisionHigh();
}

bool Building::getNeutralLoaded() const
{
    return m_neutralLoaded;
}

bool Building::getAlwaysVisble() const
{
    return m_alwaysVisble;
}

void Building::setAlwaysVisble(bool value)
{
    m_alwaysVisble = value;
}

QString Building::getTerrainAnimationBase()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainAnimationBase";
    QJSValueList args({pInterpreter->newQObject(nullptr),
                       pInterpreter->newQObject(m_pTerrain),
                       pInterpreter->newQObject(nullptr),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_BuildingID, function1, args);
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
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainAnimationForeground";
    QJSValueList args({pInterpreter->newQObject(nullptr),
                       pInterpreter->newQObject(m_pTerrain),
                       pInterpreter->newQObject(nullptr),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_BuildingID, function1, args);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

float Building::getTerrainAnimationMoveSpeed()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainAnimationMoveSpeed";
    QJSValueList args({pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_BuildingID, function1, args);
    if (erg.isNumber())
    {
        return erg.toNumber();
    }
    else
    {
        return 0.0f;
    }
}

bool Building::canRepair(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getConstructionList";
    QJSValueList args({pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_BuildingID, function1, args);
    return erg.toVariant().toStringList().contains(pUnit->getUnitID());
}

bool Building::isCaptureOrMissileBuilding(bool hasSiloTarget)
{
    bool capturable = isCaptureBuilding();
    if (!capturable && hasSiloTarget)
    {
        capturable = isMissile();
    }
    return capturable;
}

bool Building::isCaptureBuilding()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getCapturableBuildings";
    QJSValueList args({pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(CoreAI::ACTION_CAPTURE, function1, args);
    bool capturable = erg.toVariant().toStringList().contains(m_BuildingID);
    return capturable;
}

bool Building::isMissile()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMissileBuildings";
    QJSValueList args({pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(CoreAI::ACTION_MISSILE, function1, args);
    bool capturable = erg.toVariant().toStringList().contains(m_BuildingID);
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
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainAnimationBackground";
    QJSValueList args({pInterpreter->newQObject(nullptr),
                      pInterpreter->newQObject(m_pTerrain),
                      pInterpreter->newQObject(nullptr),
                      pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_BuildingID, function1, args);
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
    return m_fireCount;
}

void Building::setFireCount(const qint32 &value)
{
    m_fireCount = value;
}

qint32 Building::getHp() const
{
    return m_Hp;
}

void Building::setHp(const qint32 &Hp)
{
    m_Hp = Hp;
}

bool Building::isEnemyBuilding(Player* pPlayer)
{
    return (m_pOwner == nullptr || !m_pOwner->getIsDefeated()) &&
            pPlayer->isEnemy(m_pOwner);
}

void Building::serializeObject(QDataStream& pStream) const
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
    pStream << m_fireCount;
    m_Variables.serializeObject(pStream);
    pStream << m_BuildingName;
}

void Building::deserializeObject(QDataStream& pStream)
{
    deserializer(pStream, false);
}

void Building::deserializer(QDataStream& pStream, bool fast)
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
    if (!fast)
    {
        init();
    }
    qint32 playerID = -1;
    pStream >> playerID;
    m_pOwner = m_pMap->getPlayer(playerID);
    if (version > 1)
    {
        qint32 newHp = 0;
        pStream >> newHp;
        if (newHp > 0 && m_Hp > 0)
        {
            m_Hp = newHp;
        }
        pStream >> m_fireCount;
    }
    if (version > 2)
    {
        m_Variables.deserializeObject(pStream);
    }
    if (version > 4)
    {
        pStream >> m_BuildingName;
    }
}
