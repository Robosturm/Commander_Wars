#include "coGenerator/coability.h"
#include "coGenerator/cogeneratormenu.h"

CoAbility::CoAbility(CoGeneratorMenu * pParent)
    : CustomDialog("", "", pParent),
    m_pParent(pParent)
{
#ifdef GRAPHICSUPPORT
    setObjectName("CoAbility");
#endif
    detach();
    setupJsThis(this);
}

CoAbility::CoAbility(CoGeneratorMenu * pParent, const QString & coAbilityId)
    : CustomDialog("", "", pParent),
    m_pParent(pParent),
    m_coAbilityId(coAbilityId)
{
#ifdef GRAPHICSUPPORT
    setObjectName("CoAbility");
#endif
    detach();
    setupJsThis(this);
}

void CoAbility::serializeObject(QDataStream& stream) const
{
    stream << getVersion();
    stream << m_activeCondition;
    stream << m_coAbilityId;
    stream << m_incoRange;
    m_Variables.serializeObject(stream);
    stream << static_cast<qint32>(m_coAbilities.size());
    for (const auto & ability : m_coAbilities)
    {
        ability->serializeObject(stream);
    }
}

void CoAbility::deserializeObject(QDataStream& stream)
{
    qint32 version;
    stream >> version;
    stream >> m_activeCondition;
    stream >> m_coAbilityId;
    stream >> m_incoRange;
    m_Variables.deserializeObject(stream);
    qint32 size = 0;
    stream >> size;
    for (qint32 i = 0; i < size; ++i)
    {
        spCoAbility ability = MemoryManagement::create<CoAbility>(m_pParent);
        ability->deserializeObject(stream);
        m_coAbilities.append(ability);
    }
}

void CoAbility::showCoAbility()
{
    loadXmlFile(QString(CoGeneratorMenu::CO_GENERATOR_MENU_BASEPATH) + "/" + m_coAbilityId + ".xml");
    m_pBaseMenu->addChild(oxygine::spActor(this));
}

GameEnums::PowerMode CoAbility::getActiveCondition() const
{
    return m_activeCondition;
}

void CoAbility::setActiveCondition(const GameEnums::PowerMode & newActiveCondition)
{
    m_activeCondition = newActiveCondition;
}

QString CoAbility::getFunctionNameContext() const
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getFunctionNameContext";
    QJSValueList args({m_jsThis,
                      JsThis::getJsThis(m_pBaseMenu)});
    QJSValue erg = pInterpreter->doFunction(m_coAbilityId, function1, args);
    return erg.toString();
}

void CoAbility::writeFuncPrologue(QTextStream & stream)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = getFunctionNameContext() + "writeFuncPrologue";
    QJSValueList args({JsThis::getJsThis(m_pBaseMenu)});
    QJSValue erg = pInterpreter->doFunction(CoGeneratorMenu::CO_GENERATOR_MENU_JSNAME, function1, args);
    stream << erg.toString();
}

void CoAbility::writeCoAbilityContent(QTextStream & stream)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = getFunctionNameContext() + "writeCoAbilityContent";
    QJSValueList args({m_jsThis, JsThis::getJsThis(m_pBaseMenu)});
    QJSValue erg = pInterpreter->doFunction(m_coAbilityId, function1, args);
    stream << erg.toString();
    for (auto & ability : m_coAbilities)
    {
        ability->writeCoAbilityContent(stream);
    }
}

void CoAbility::writeFuncEpilogue(QTextStream & stream)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = getFunctionNameContext() + "writeFuncEpilogue";
    QJSValueList args({JsThis::getJsThis(m_pBaseMenu)});
    QJSValue erg = pInterpreter->doFunction(CoGeneratorMenu::CO_GENERATOR_MENU_JSNAME, function1, args);
    stream << erg.toString();
}

bool CoAbility::getIncoRange() const
{
    return m_incoRange;
}

void CoAbility::setIncoRange(bool newIncoRange)
{
    m_incoRange = newIncoRange;
}
