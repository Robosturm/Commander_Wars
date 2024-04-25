#include "game/building.h"
#include "game/player.h"
#include "game/terrain.h"
#include "game/gamemap.h"
#include "game/unit.h"
#include "game/co.h"

#include "coreengine/interpreter.h"
#include "coreengine/gameconsole.h"
#include "coreengine/qmlvector.h"

#include "resource_management/buildingspritemanager.h"
#include "resource_management/terrainmanager.h"

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
    Interpreter::setCppOwnerShip(this);
    setupJsThis(this);
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
    pInterpreter->doFunction(m_BuildingID, function, args);
}

qint32 Building::getBuildingGroup()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBuildingGroup";
    QJSValueList args({JsThis::getJsThis(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    return 0;
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

void Building::setMapForExtending(GameMap * newMap)
{
    m_pMap = newMap;
    if (m_pOwner != nullptr)
    {
        auto id = m_pOwner->getPlayerID();
        if (id < m_pMap->getPlayerCount())
        {
            setOwner(m_pMap->getPlayer(id));
        }
        else
        {
            m_pOwner = nullptr;
        }
    }
}

QStringList Building::getBaseTerrain()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "getBaseTerrain";
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
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

void Building::loadSpriteV2(const QString & spriteID, GameEnums::Recoloring mode, qint32 frameTime, QPoint pos, const QString & forcedPalette, bool forceNeutral)
{
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pBuildingSpriteManager->getResAnim(spriteID);
    if (pAnim != nullptr)
    {
        TerrainManager* pTerrainManager = TerrainManager::getInstance();
        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
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
        else if ((mode == GameEnums::Recoloring_Table || matrixMode) && m_pOwner != nullptr && !forceNeutral)
        {
            if (forcedPalette.isEmpty())
            {
                pSprite->setColorTable(m_pOwner->getColorTableAnim(), matrixMode);
            }
            else
            {
                oxygine::spResAnim pPaletteAnim = pTerrainManager->getSpResAnim(forcedPalette, oxygine::error_policy::ep_ignore_error);
                if (pPaletteAnim.get() != nullptr)
                {
                    pSprite->setColorTable(pPaletteAnim, true);
                }
            }
        }
        else if (mode == GameEnums::Recoloring_Table || matrixMode)
        {
            if (forcedPalette.isEmpty())
            {
                pSprite->setColorTable(Player::getNeutralTableAnim(), matrixMode);
            }
            else
            {
                oxygine::spResAnim pPaletteAnim = pTerrainManager->getSpResAnim(forcedPalette, oxygine::error_policy::ep_ignore_error);
                if (pPaletteAnim.get() != nullptr)
                {
                    pSprite->setColorTable(pPaletteAnim, true);
                }
            }
        }
        else
        {
            pSprite->setPriority(static_cast<qint32>(DrawPriority::NoneMask));
        }
        qint32 width = getBuildingWidth();
        qint32 heigth = getBuildingHeigth();
        if (width == 1 && heigth == 1)
        {
            pSprite->setScale(static_cast<float>(GameMap::getImageSize()) / static_cast<float>(pAnim->getWidth()));
            pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::getImageSize()) / 2 + pos.x(), -(pSprite->getScaledHeight() - GameMap::getImageSize()) + pos.y());
        }
        else
        {
            pSprite->setScale(static_cast<float>(GameMap::getImageSize() * width) / static_cast<float>(pAnim->getWidth()));
            pSprite->setPosition(-pSprite->getScaledWidth() + GameMap::getImageSize() + pos.x(), -pSprite->getScaledHeight() + GameMap::getImageSize() + pos.y());
        }
        addChild(pSprite);
        m_pBuildingSprites.append(pSprite);
        if (forcedPalette.isEmpty())
        {
            m_addPlayerColor.append(mode);
        }
        else
        {
            m_addPlayerColor.append(GameEnums::Recoloring_None);
        }
    }
    else
    {
        CONSOLE_PRINT_MODULE("Unable to load building sprite: " + spriteID, GameConsole::eDEBUG, GameConsole::eResources);
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(pWeather),
                       JsThis::getJsThis(m_pMap)});
    pInterpreter->doFunction(m_BuildingID, function1, args);
}

void Building::loadWeatherOverlaySpriteV2(const QString & spriteID, GameEnums::Recoloring mode, qint32 frameTime)
{
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pBuildingSpriteManager->getResAnim(spriteID);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
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
            pSprite->setScale(static_cast<float>(GameMap::getImageSize()) / static_cast<float>(pAnim->getWidth()));
            pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::getImageSize()) / 2, -(pSprite->getScaledHeight() - GameMap::getImageSize()));
        }
        else
        {
            pSprite->setScale(static_cast<float>(GameMap::getImageSize() * width) / static_cast<float>(pAnim->getWidth()));
            pSprite->setPosition(-pSprite->getScaledWidth() + GameMap::getImageSize(), -pSprite->getScaledHeight() + GameMap::getImageSize());
        }
        addChild(pSprite);
        m_pWeatherOverlaySprites.append(pSprite);
    }
    else
    {
        CONSOLE_PRINT_MODULE("Unable to load weather overlay sprite: " + spriteID, GameConsole::eDEBUG, GameConsole::eResources);
    }
}

bool Building::isHq()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "isHq";
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_BuildingID, function1, args);
    if (erg.isBool())
    {
        return erg.toBool();
    }
    return false;
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
        sprite->syncAllTweens(syncTime);
    }
    for (auto & sprite : m_pWeatherOverlaySprites)
    {
        sprite->syncAllTweens(syncTime);
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({m_jsThis,
                       neutral,
                       JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({JsThis::getJsThis(pTerrain),
                       m_jsThis,
                       JsThis::getJsThis(m_pMap)});
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
        QJSValueList args({m_jsThis,
                           JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
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

QStringList Building::getActionList()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getActions";
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
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
            QStringList result = pCO->getAdditionalBuildingActions(this);
            if (result.size() > 0)
            {
                actionModifierList.append(result);
            }
        }
        pCO = m_pOwner->getCO(1);
        if (pCO != nullptr)
        {
            QStringList result = pCO->getAdditionalBuildingActions(this);
            if (result.size() > 0)
            {
                actionModifierList.append(result);
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
                    QStringList result = pCO->getAdditionalBuildingActions(this);
                    if (result.size() > 0)
                    {
                        actionModifierList.append(result);
                    }
                }
                pCO = pPlayer->getCO(1);
                if (pCO != nullptr)
                {
                    QStringList result = pCO->getAdditionalBuildingActions(this);
                    if (result.size() > 0)
                    {
                        actionModifierList.append(result);
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

QVector<qint32> Building::getRepairTypes()
{

    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getRepairTypes";
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
    auto list = ret.toVariant().toList();
    QVector<qint32> retList;
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
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
        returnList.removeDuplicates();
        return returnList;
    }
    buildList.removeDuplicates();
    return buildList;
}

void Building::endOfTurn()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "endOfTurn";
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
    pInterpreter->doFunction(m_BuildingID, function1, args);
}

void Building::startOfTurn()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "startOfTurn";
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
    pInterpreter->doFunction(m_BuildingID, function1, args);
}

qint32 Building::getOffensiveBonus()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getOffensiveBonus";
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({m_jsThis,
                       id,
                       baseCost,
                       position.x(),
                       position.y(),
                       JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(pAttacker),
                       atkPosition.x(),
                       atkPosition.y(),
                       JsThis::getJsThis(pDefender),
                       defPosition.x(),
                       defPosition.y(),
                       isDefender,
                       JsThis::getJsThis(pAction),
                       luckMode,
                       JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(pAttacker),
                       atkPosition.x(),
                       atkPosition.y(),
                       JsThis::getJsThis(pDefender),
                       defPosition.x(),
                       defPosition.y(),
                       isDefender,
                       JsThis::getJsThis(pAction),
                       luckMode,
                       JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({m_jsThis,
                       x,
                       y,
                       JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
    return ret.toVariant().toPoint();
}

qreal Building::getDamage(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDamage";
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(pUnit),
                       JsThis::getJsThis(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
    if (ret.isNumber())
    {
        return ret.toNumber();
    }
    return 0.0;
}

GameEnums::BuildingTarget Building::getBuildingTargets()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBuildingTargets";
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
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

void Building::setBuildingName(const QString & BuildingName)
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
    QJSValueList args({JsThis::getJsThis(nullptr),
                       JsThis::getJsThis(m_pTerrain),
                       JsThis::getJsThis(nullptr),
                       JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({JsThis::getJsThis(nullptr),
                       JsThis::getJsThis(m_pTerrain),
                       JsThis::getJsThis(nullptr),
                       JsThis::getJsThis(m_pMap)});
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
    QJSValueList args({JsThis::getJsThis(m_pMap)});
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

bool Building::canRepair(Unit* pUnit, bool always = false)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "canRepair";
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(pUnit),
                       JsThis::getJsThis(m_pMap),
                       always});
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    return false;
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
    QJSValueList args({JsThis::getJsThis(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(CoreAI::ACTION_CAPTURE, function1, args);
    bool capturable = erg.toVariant().toStringList().contains(m_BuildingID);
    return capturable;
}

bool Building::isMissile()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMissileBuildings";
    QJSValueList args({JsThis::getJsThis(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(CoreAI::ACTION_MISSILE, function1, args);
    bool capturable = erg.toVariant().toStringList().contains(m_BuildingID);
    return capturable;
}

bool Building::isProductionBuilding()
{
    auto actionList = getActionList();
    if (actionList.contains(CoreAI::ACTION_BUILD_UNITS) ||
        actionList.contains(CoreAI::ACTION_BLACKHOLEFACTORY_DOOR1) ||
        actionList.contains(CoreAI::ACTION_BLACKHOLEFACTORY_DOOR2) ||
        actionList.contains(CoreAI::ACTION_BLACKHOLEFACTORY_DOOR3) ||
        actionList.contains(CoreAI::ACTION_NEST_FACTORY_DOOR))
    {
        return true;
    }
    return false;
}

QString Building::getTerrainAnimationBackground()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainAnimationBackground";
    QJSValueList args({JsThis::getJsThis(nullptr),
                      JsThis::getJsThis(m_pTerrain),
                      JsThis::getJsThis(nullptr),
                      JsThis::getJsThis(m_pMap)});
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
    serializeObject(pStream, false);
}

void Building::serializeObject(QDataStream& pStream, bool forHash) const
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
    m_Variables.serializeObject(pStream, forHash);
    if (!forHash)
    {
        pStream << m_BuildingName;
    }
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
