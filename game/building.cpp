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

const float Building::animationSpeed = 4.0f;

Building::Building(QString BuildingID)
    : m_BuildingID(BuildingID),
      m_pOwner(nullptr),
      m_pTerrain(nullptr)
{
    setObjectName("Building");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
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
    QJSValueList args;
    QJSValue objArg = pInterpreter->newQObject(this);
    args << objArg;
    pInterpreter->doFunction(m_BuildingID, function, args);
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
    QJSValueList args;
    QString function = "getBaseTerrain";
    QJSValue objArg = pInterpreter->newQObject(this);
    args << objArg;
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function, args);
    return ret.toVariant().toStringList();
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
    QJSValueList args;
    QJSValue objArg = pInterpreter->newQObject(this);
    args << objArg;
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
    QJSValueList args;
    QJSValue objArg = pInterpreter->newQObject(this);
    args << objArg;
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
    spGameMap pMap = GameMap::getInstance();
    qint32 x = Building::getX();
    qint32 y = Building::getY();

    if (pMap.get() != nullptr &&
        pMap->getCurrentViewPlayer() != nullptr &&
        pMap->onMap(x, y))
    {
        visible = pMap->getCurrentViewPlayer()->getFieldVisible(x, y);
    }
    if (m_pBuildingSprites.size() == 0 ||
        prevOwner == nullptr)
    {
        updateBuildingSprites(false);
        updatePlayerColor(visible);
    }
    else
    {
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

void Building::loadSprite(QString spriteID, bool addPlayerColor)
{
    if (addPlayerColor)
    {
        loadSpriteV2(spriteID, GameEnums::Recoloring_Mask);
    }
    else
    {
        loadSpriteV2(spriteID, GameEnums::Recoloring_None);
    }
}

void Building::loadSpriteV2(QString spriteID, GameEnums::Recoloring mode)
{
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pBuildingSpriteManager->getResAnim(spriteID);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = oxygine::spSprite::create();
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
        if (mode == GameEnums::Recoloring_Mask && m_pOwner != nullptr)
        {
            QColor color = m_pOwner->getColor();
            pSprite->setColor(color);
        }
        else if (mode == GameEnums::Recoloring_Mask)
        {
            pSprite->setColor(QColor(150, 150, 150, 255));
        }
        else if (mode == GameEnums::Recoloring_Table && m_pOwner != nullptr)
        {
            pSprite->setColorTable(m_pOwner->getColorTableAnim());
        }
        else if (mode == GameEnums::Recoloring_Table)
        {
            pSprite->setColorTable(Player::getNeutralTableAnim());
        }
        else
        {
            pSprite->setPriority(1);
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
        this->addChild(pSprite);
        m_pBuildingSprites.append(pSprite);
        m_addPlayerColor.append(mode);
    }
    else
    {
        Console::print("Unable to load terrain sprite: " + spriteID, Console::eERROR);
    }
}

void Building::syncAnimation(oxygine::timeMS syncTime)
{
    for (auto & sprite : m_pBuildingSprites)
    {
        oxygine::spTween pTween = sprite->getFirstTween();
        while (pTween.get() != nullptr)
        {
            pTween->setElapsed(syncTime);
            pTween = pTween->getNextSibling();
        }
    }
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
                    else if (m_addPlayerColor[i] == GameEnums::Recoloring_Table)
                    {
                        m_pBuildingSprites[i]->setColorTable(m_pOwner->getColorTableAnim());
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
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
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
    for (qint32 i = 0; i < m_pBuildingSprites.size(); i++)
    {
        m_pBuildingSprites[i]->detach();
    }
    // call the js loader function to do the rest
    m_pBuildingSprites.clear();
    m_addPlayerColor.clear();
    QString function1 = "loadSprites";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    args1 << neutral;
    pInterpreter->doFunction(m_BuildingID, function1, args1);
    m_neutralLoaded = neutral;
}

bool Building::canBuildingBePlaced(Terrain* pTerrain)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "canBuildingBePlaced";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(pTerrain);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args1);
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
        QJSValueList args1;
        QJSValue obj = pInterpreter->newQObject(this);
        args1 << obj;
        QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args1);
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
    QJSValueList args1;
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args1);
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
        spGameMap pMap = GameMap::getInstance();
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = pMap->getPlayer(i);
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
    QJSValueList args1;
    QJSValue obj = pInterpreter->newQObject(this);
    args1 << obj;
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args1);
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
    QJSValueList args1;
    QJSValue obj = pInterpreter->newQObject(this);
    args1 << obj;
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args1);
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


        spGameMap pMap = GameMap::getInstance();
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = pMap->getPlayer(i);
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
    for (qint32 i = 0; i < actionModifierList.size(); i++)
    {
        QString action = actionModifierList[i];
        if (!action.startsWith("-") && !action.isEmpty())
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
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args1);
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
    QJSValueList args;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args << obj1;
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
        for (qint32 i = 0; i < coUnits.size(); i++)
        {
            if (coUnits[i].startsWith("-"))
            {
                buildList.removeAll(coUnits[i].replace("-", ""));
            }
            else
            {
                buildList.append(coUnits[i]);
            }
        }
    }

    spGameMap pMap = GameMap::getInstance();
    QStringList returnList;
    if (m_pOwner != nullptr && pMap.get() != nullptr)
    {
     bool coUnits = pMap->getGameRules()->getCoUnits();
        QStringList playerBuildList = m_pOwner->getBuildList();
        for (qint32 i = 0; i < buildList.size(); i++)
        {
            QString unitID = buildList[i];
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

void Building::startOfTurn()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "startOfTurn";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    pInterpreter->doFunction(m_BuildingID, function1, args1);
}

qint32 Building::getOffensiveBonus()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getOffensiveBonus";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args1);
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
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args1);
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
    QJSValueList args1;
    QJSValue obj = pInterpreter->newQObject(this);
    args1 << obj;
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Building::getOffensiveFieldBonus(GameAction* pAction, Unit* pAttacker, QPoint atkPosition,Unit* pDefender,  QPoint defPosition, bool isDefender)
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
    QJSValue obj4 = pInterpreter->newQObject(pAction);
    args1 << obj4;
    qint32 ergValue = 0;
    QJSValue erg = pInterpreter->doFunction(m_BuildingID, function1, args1);
    if (erg.isNumber())
    {
        ergValue += erg.toInt();
    }
    return ergValue;
}

qint32 Building::getDeffensiveFieldBonus(GameAction* pAction, Unit* pAttacker, QPoint atkPosition, Unit* pDefender, QPoint defPosition, bool isDefender)
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
    QJSValue obj4 = pInterpreter->newQObject(pAction);
    args1 << obj4;
    qint32 ergValue = 0;
    QJSValue erg = pInterpreter->doFunction(m_BuildingID, function1, args1);
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
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1);
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
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1);
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
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    args1 << x;
    args1 << y;
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args1);
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
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args1);
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
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args1);
    return ret.toVariant().toPoint();
}

float Building::getDamage(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDamage";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(pUnit);
    args1 << obj2;
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args1);
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
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue ret = pInterpreter->doFunction(m_BuildingID, function1, args1);
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
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(nullptr);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(m_pTerrain);
    args1 << obj2;
    QJSValue erg = pInterpreter->doFunction(m_BuildingID, function1, args1);
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
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(nullptr);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(m_pTerrain);
    args1 << obj2;
    QJSValue erg = pInterpreter->doFunction(m_BuildingID, function1, args1);
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
    QJSValueList args;
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
    QJSValue erg = pInterpreter->doFunction(m_BuildingID, function1);
    return erg.toVariant().toStringList().contains(pUnit->getUnitID());
}

bool Building::isCaptureOrMissileBuilding(bool hasSiloTarget)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getCapturableBuildings";
    QJSValue erg = pInterpreter->doFunction(CoreAI::ACTION_CAPTURE, function1);
    bool capturable = erg.toVariant().toStringList().contains(m_BuildingID);
    if (!capturable && hasSiloTarget)
    {
        function1 = "getMissileBuildings";
        erg = pInterpreter->doFunction(CoreAI::ACTION_MISSILE, function1);
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
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainAnimationBackground";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(nullptr);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(m_pTerrain);
    args1 << obj2;
    QJSValue erg = pInterpreter->doFunction(m_BuildingID, function1, args1);
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
    m_pOwner = GameMap::getInstance()->getPlayer(playerID);
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
