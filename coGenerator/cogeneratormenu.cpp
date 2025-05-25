#include "coGenerator/cogeneratormenu.h"

#include "coreengine/gameconsole.h"
#include "coreengine/mainapp.h"
#include "coreengine/audiomanager.h"
#include "coreengine/memorymanagement.h"

#include "resource_management/backgroundmanager.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "ui_reader/uifactory.h"

#include "objects/base/panel.h"
#include "objects/base/dropdownmenu.h"

#include "menue/mainwindow.h"

const char* const CoGeneratorMenu::CO_GENERATOR_MENU_JSNAME = "COGENERATORMENU";
const char* const CoGeneratorMenu::CO_GENERATOR_MENU_BASEPATH = "ui/coGenerator/";
const char* const CoGeneratorMenu::CO_GENERATOR_MENU_XML = "cogeneratormenu.xml";
CoGeneratorMenu::CoGeneratorMenu()
    : GeneratorMenu("", false)
{
#ifdef GRAPHICSUPPORT
    setObjectName("CoGeneratorMenu");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    CONSOLE_PRINT("Entering CoGeneratorMenu", GameConsole::eDEBUG);

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = MemoryManagement::create<oxygine::Sprite>();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("cogeneratormenu");
    if (pBackground != nullptr &&
        pBackground->getWidth() > 0 &&
        pBackground->getHeight() > 0)
    {
        sprite->setResAnim(pBackground);
        // background should be last to draw
        sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
        sprite->setScaleX(static_cast<float>(oxygine::Stage::getStage()->getWidth()) / static_cast<float>(pBackground->getWidth()));
        sprite->setScaleY(static_cast<float>(oxygine::Stage::getStage()->getHeight()) / static_cast<float>(pBackground->getHeight()));
    }

    pApp->getAudioManager()->clearPlayList();
    pApp->getAudioManager()->loadFolder("resources/music/coGenerator");
    pApp->getAudioManager()->playRandom();

    setupJsThis(this);
    UiFactory::getInstance().createUi(QString(CO_GENERATOR_MENU_BASEPATH) + CO_GENERATOR_MENU_XML, this);

    pApp->continueRendering();
}

void CoGeneratorMenu::generateCo(const QString & modDir, const QString & coid)
{
    QFile file(Settings::getInstance()->getUserPath() + "/" + modDir + "/" + coid + ".js");
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        writeCoPrologue(stream);
        writeCoContent(stream);
        writeCoEpilogue(stream);
    }
}

void CoGeneratorMenu::writeCoPrologue(QTextStream & stream)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "writeCoPrologue";
    QJSValueList args({m_jsThis});
    QJSValue erg = pInterpreter->doFunction(CO_GENERATOR_MENU_JSNAME, function1, args);
    stream << erg.toString();
}

void CoGeneratorMenu::writeCoContent(QTextStream & stream)
{
    QStringList doneAbilities;
    for (qint32 i = 0; i < m_coAbilities.size(); ++i)
    {
        auto & ability = m_coAbilities[i];
        QString coAbilityName = ability->getFunctionNameContext();
        if (!doneAbilities.contains(coAbilityName))
        {
            doneAbilities.append(coAbilityName);
            ability->writeFuncPrologue(stream);
            ability->writeCoAbilityContent(stream);
            for (qint32 i2 = 0; i2 < m_coAbilities.size(); ++i2)
            {
                if (i2 != i)
                {
                    auto & otherAbility = m_coAbilities[i2];
                    QString otherCoAbilityName = otherAbility->getFunctionNameContext();
                    if (otherCoAbilityName == coAbilityName)
                    {
                        otherAbility->writeCoAbilityContent(stream);
                    }
                }
            }
            ability->writeFuncEpilogue(stream);
        }
    }
}

void CoGeneratorMenu::writeCoEpilogue(QTextStream & stream)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "writeCoEpilogue";
    QJSValueList args({m_jsThis});
    QJSValue erg = pInterpreter->doFunction(CO_GENERATOR_MENU_JSNAME, function1, args);
    stream << erg.toString();
}

qint32 CoGeneratorMenu::getCoAbilityCount() const
{
    return m_coAbilities.size();
}

CoAbility* CoGeneratorMenu::getCoAbility(qint32 index) const
{
    if (index >= 0 && index < m_coAbilities.size())
    {
        return m_coAbilities[index].get();
    }
    return nullptr;
}

QString CoGeneratorMenu::getCoId() const
{
    return m_coId;
}

void CoGeneratorMenu::setCoId(const QString & newCoId)
{
    m_coId = newCoId;
}

void CoGeneratorMenu::addCoAbility(const QString & coAbilityId, GameEnums::PowerMode powerMode)
{
    spCoAbility ability = MemoryManagement::create<CoAbility>(this, coAbilityId);
    ability->setActiveCondition(powerMode);
    m_coAbilities.append(ability);
}

void CoGeneratorMenu::removeCoAbility(qint32 index)
{
    if (index >= 0 && index < m_coAbilities.size())
    {
        m_coAbilities.removeAt(index);
    }
}

QString CoGeneratorMenu::getCoLongDescription() const
{
    return m_coLongDescription;
}

void CoGeneratorMenu::setCoLongDescription(const QString & newCoLongDescription)
{
    m_coLongDescription = newCoLongDescription;
}

QString CoGeneratorMenu::getCoName() const
{
    return m_coName;
}

void CoGeneratorMenu::setCoName(const QString & newCoName)
{
    m_coName = newCoName;
}

qint32 CoGeneratorMenu::getPowerStars() const
{
    return m_powerStars;
}

void CoGeneratorMenu::setPowerStars(qint32 newPowerStars)
{
    m_powerStars = newPowerStars;
}

QString CoGeneratorMenu::getCoPowerDescription() const
{
    return m_coPowerDescription;
}

void CoGeneratorMenu::setCoPowerDescription(const QString & newCoPowerDescription)
{
    m_coPowerDescription = newCoPowerDescription;
}

QString CoGeneratorMenu::getCoPowerName() const
{
    return m_coPowerName;
}

void CoGeneratorMenu::setCoPowerName(const QString & newCoPowerName)
{
    m_coPowerName = newCoPowerName;
}

qint32 CoGeneratorMenu::getSuperpowerStars() const
{
    return m_superpowerStars;
}

void CoGeneratorMenu::setSuperpowerStars(qint32 newSuperpowerStars)
{
    m_superpowerStars = newSuperpowerStars;
}

QString CoGeneratorMenu::getCoSuperpowerDescription() const
{
    return m_coSuperpowerDescription;
}

void CoGeneratorMenu::setCoSuperpowerDescription(const QString & newCoSuperpowerDescription)
{
    m_coSuperpowerDescription = newCoSuperpowerDescription;
}

QString CoGeneratorMenu::getCoSuperpowerName() const
{
    return m_coSuperpowerName;
}

void CoGeneratorMenu::setCoSuperpowerName(const QString & newCoSuperpowerName)
{
    m_coSuperpowerName = newCoSuperpowerName;
}

QString CoGeneratorMenu::getCoBio() const
{
    return m_coBio;
}

void CoGeneratorMenu::setCoBio(const QString & newCoBio)
{
    m_coBio = newCoBio;
}

QString CoGeneratorMenu::getCoHits() const
{
    return m_coHits;
}

void CoGeneratorMenu::setCoHits(const QString & newCoHits)
{
    m_coHits = newCoHits;
}

QString CoGeneratorMenu::getCoMiss() const
{
    return m_coMiss;
}

void CoGeneratorMenu::setCoMiss(const QString & newCoMiss)
{
    m_coMiss = newCoMiss;
}

QString CoGeneratorMenu::getCoDescription() const
{
    return m_coDescription;
}

void CoGeneratorMenu::setCoDescription(const QString & newCoDescription)
{
    m_coDescription = newCoDescription;
}

QString CoGeneratorMenu::getCoArmy() const
{
    return m_coArmy;
}

void CoGeneratorMenu::setCoArmy(const QString & newCoArmy)
{
    m_coArmy = newCoArmy;
}

qint32 CoGeneratorMenu::getCoUnitRange() const
{
    return m_coUnitRange;
}

void CoGeneratorMenu::setCoUnitRange(qint32 newCoUnitRange)
{
    m_coUnitRange = newCoUnitRange;
}

QStringList CoGeneratorMenu::getCoPowerSentences() const
{
    return m_coPowerSentences;
}

void CoGeneratorMenu::setCoPowerSentences(const QStringList & newCoPowerSentences)
{
    m_coPowerSentences = newCoPowerSentences;
}

void CoGeneratorMenu::setCoPowerSentence(const QString & newCoPowerSentence, qint32 idx)
{
    if (idx >= 0 && idx < m_coPowerSentences.size())
    {
        m_coPowerSentences[idx] = newCoPowerSentence;
    }
}

QStringList CoGeneratorMenu::getCoVictorySentences() const
{
    return m_coVictorySentences;
}

void CoGeneratorMenu::setCoVictorySentences(const QStringList & newCoVictorySentences)
{
    m_coVictorySentences = newCoVictorySentences;
}

void CoGeneratorMenu::setCoVictorySentence(const QString & newCoVictorySentence, qint32 idx)
{
    if (idx >= 0 && idx < m_coVictorySentences.size())
    {
        m_coVictorySentences[idx] = newCoVictorySentence;
    }
}

QStringList CoGeneratorMenu::getCoDefeatSentences() const
{
    return m_coDefeatSentences;
}

void CoGeneratorMenu::setCoDefeatSentences(const QStringList & newCoDefeatSentences)
{
    m_coDefeatSentences = newCoDefeatSentences;
}

void CoGeneratorMenu::setCoDefeatSentence(const QString & newCoDefeatSentence, qint32 idx)
{
    if (idx >= 0 && idx < m_coDefeatSentences.size())
    {
        m_coDefeatSentences[idx] = newCoDefeatSentence;
    }
}

void CoGeneratorMenu::storeCoData(const QString & filename) const
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&file);
        serializeObject(stream);
    }
}

void CoGeneratorMenu::loadCoData(const QString & filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);
        deserializeObject(stream);
        reloadGeneratorUi();
    }
}

void CoGeneratorMenu::reloadGeneratorUi()
{
    static const char* const PANELNAME = "MAINPANEL";
    static const char* const ABILITYIDX = "ABILITYIDX";
    static const char* const POWERMODE = "POWERMODE";
    auto hScrollValue = getCastedObject<Panel>(PANELNAME)->getH_Scrollbar()->getScrollvalue();
    auto abilityIdx = getCastedObject<DropDownmenu>(ABILITYIDX)->getCurrentItem();
    auto powerMode = getCastedObject<DropDownmenu>(POWERMODE)->getCurrentItem();

    reloadUi(QString(CO_GENERATOR_MENU_BASEPATH) + CO_GENERATOR_MENU_XML);

    getCastedObject<Panel>(PANELNAME)->getH_Scrollbar()->changeScrollValue(hScrollValue);
    getCastedObject<DropDownmenu>(ABILITYIDX)->setCurrentItem(abilityIdx);
    getCastedObject<DropDownmenu>(POWERMODE)->setCurrentItem(powerMode);
}

void CoGeneratorMenu::serializeObject(QDataStream& stream) const
{
    stream << getVersion();
    stream << m_coId;
    m_Variables.serializeObject(stream);
    stream << static_cast<qint32>(m_coAbilities.size());
    for (const auto & ability : m_coAbilities)
    {
        ability->serializeObject(stream);
    }
    stream << m_coName;
    stream << m_powerStars;
    stream << m_coPowerDescription;
    stream << m_coPowerName;
    stream << m_superpowerStars;
    stream << m_coSuperpowerDescription;
    stream << m_coSuperpowerName;
    stream << m_coBio;
    stream << m_coHits;
    stream << m_coMiss;
    stream << m_coDescription;
    stream << m_coLongDescription;
    stream << m_coArmy;
    stream << m_coUnitRange;
    stream << static_cast<qint32>(m_coPowerSentences.size());
    for (const auto & content : m_coPowerSentences)
    {
        stream << content;
    }
    stream << static_cast<qint32>(m_coDefeatSentences.size());
    for (const auto & content : m_coDefeatSentences)
    {
        stream << content;
    }
    stream << static_cast<qint32>(m_coVictorySentences.size());
    for (const auto & content : m_coVictorySentences)
    {
        stream << content;
    }
}

void CoGeneratorMenu::deserializeObject(QDataStream& stream)
{
    m_coAbilities.clear();
    m_coPowerSentences.clear();
    m_coDefeatSentences.clear();
    m_coVictorySentences.clear();
    qint32 version;
    stream >> version;
    stream >> m_coId;
    m_Variables.deserializeObject(stream);
    qint32 size = 0;
    stream >> size;
    for (qint32 i = 0; i < size; ++i)
    {
        spCoAbility ability = MemoryManagement::create<CoAbility>(this);
        ability->deserializeObject(stream);
        m_coAbilities.append(ability);
    }
    stream >> m_coName;
    stream >> m_powerStars;
    stream >> m_coPowerDescription;
    stream >> m_coPowerName;
    stream >> m_superpowerStars;
    stream >> m_coSuperpowerDescription;
    stream >> m_coSuperpowerName;
    stream >> m_coBio;
    stream >> m_coHits;
    stream >> m_coMiss;
    stream >> m_coDescription;
    stream >> m_coLongDescription;
    stream >> m_coArmy;
    stream >> m_coUnitRange;
    stream >> size;
    for (qint32 i = 0; i < size; ++i)
    {
        QString content;
        stream >> content;
        m_coPowerSentences.append(content);
    }
    stream >> size;
    for (qint32 i = 0; i < size; ++i)
    {
        QString content;
        stream >> content;
        m_coDefeatSentences.append(content);
    }
    stream >> size;
    for (qint32 i = 0; i < size; ++i)
    {
        QString content;
        stream >> content;
        m_coVictorySentences.append(content);
    }
}
