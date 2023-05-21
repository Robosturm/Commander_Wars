#include "game/ui/customcoboostinfo.h"

#include "coreengine/interpreter.h"

CustomCoBoostInfo::CustomCoBoostInfo()
{
    Interpreter::setCppOwnerShip(this);
}

const QString &CustomCoBoostInfo::getIconId() const
{
    return m_iconId;
}

void CustomCoBoostInfo::setIconId(const QString &newIconId)
{
    m_iconId = newIconId;
}

qint32 CustomCoBoostInfo::getOffensiveBoost() const
{
    return m_offensiveBoost;
}

void CustomCoBoostInfo::setOffensiveBoost(qint32 newOffensiveBoost)
{
    m_offensiveBoost = newOffensiveBoost;
}

qint32 CustomCoBoostInfo::getDefensiveBoost() const
{
    return m_defensiveBoost;
}

void CustomCoBoostInfo::setDefensiveBoost(qint32 newDefensiveBoost)
{
    m_defensiveBoost = newDefensiveBoost;
}

const QStringList &CustomCoBoostInfo::getIconBonus() const
{
    return m_iconBonus;
}

const QStringList &CustomCoBoostInfo::getBonusIcons() const
{
    return m_bonusIcons;
}

void CustomCoBoostInfo::addBonusIcon(const QString & id, const QString & value)
{
    m_bonusIcons.append(id);
    m_iconBonus.append(value);
}

const QString &CustomCoBoostInfo::getLink() const
{
    return m_link;
}

void CustomCoBoostInfo::setLink(const QString &newLink)
{
    m_link = newLink;
}
