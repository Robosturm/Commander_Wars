#include "3rd_party/oxygine-framework/oxygine/TextStyle.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

#include "objects/perkselection.h"

#include "coreengine/globalutils.h"

#include "resource_management/coperkmanager.h"
#include "resource_management/fontmanager.h"

#include "game/gamemap.h"
#include "objects/base/label.h"

PerkSelection::PerkSelection(CO* pCO, qint32 width, qint32 maxPerkCosts, qint32 maxPerkCount, bool banning, QStringList hiddenList, GameMap* pMap)
    : m_pCO(pCO),
      m_maxPerkCosts(maxPerkCosts),
      m_maxPerkCount(maxPerkCount),
      m_banning(banning),
      m_hiddenPerks(hiddenList),
      m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("PerkSelection");
#endif
    Interpreter::setCppOwnerShip(this);
    setWidth(width);
    connect(this, &PerkSelection::sigUpdatePerkCount, this, &PerkSelection::updatePerkCount, Qt::QueuedConnection);
    updatePerksView(pCO);
}

void PerkSelection::updatePerksView(CO* pCO)
{    
    m_pCO = pCO;
    removeChildren();
    if (m_banning)
    {
        m_perks = m_pMap->getGameRules()->getAllowedPerks();
    }
    else
    {
        m_perks =  pCO->getPerkList();
    }

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    oxygine::TextStyle largeStyle = oxygine::TextStyle(FontManager::getMainFont48());
    largeStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    qint32 y = 0;
    qint32 x = 0;
    COPerkManager* pCOPerkManager = COPerkManager::getInstance();
    
    const qint32 width = 470;
    auto perkGroups = getPerksGrouped();
    for (const auto & group : perkGroups)
    {
        spLabel textField = spLabel::create(getWidth() - 40);
        textField->setStyle(largeStyle);
        textField->setHtmlText(group.name);
        textField->setPosition(getWidth() / 2 - textField->getTextRect().width() / 2, y);
        addChild(textField);
        y += textField->getHeight() + 10;
        x = 0;
        for (qint32 i = 0; i < group.perks.size(); ++i)
        {
            qint32 index = group.perks[i];
            QString id = pCOPerkManager->getID(index);
            QString name = pCOPerkManager->getName(index);
            QString icon = pCOPerkManager->getIcon(index);
            oxygine::ResAnim* pAnim = pCOPerkManager->getResAnim(icon, oxygine::error_policy::ep_ignore_error);
            qint32 cost = getPerkScore(id);
            QString description = pCOPerkManager->getDescription(index) + "\n" + tr("Cost: %0").arg(cost);

            spCheckbox pCheckbox = spCheckbox::create();
            pCheckbox->setPosition(x, y + 15);
            pCheckbox->setTooltipText(description);
            pCheckbox->setChecked(m_perks.contains(id));
            connect(pCheckbox.get(), &Checkbox::checkChanged, this, [this, id](bool value)
            {
                if (value)
                {
                    m_perks.append(id);
                }
                else
                {
                    m_perks.removeAll(id);
                }
                if (!m_banning)
                {
                    if (value)
                    {
                        m_pCO->addPerk(id);
                    }
                    else
                    {
                        m_pCO->removePerk(id);
                    }
                    emit sigUpdatePerkCount();
                }
            });
            m_Checkboxes.append(pCheckbox);
            m_perkIds.append(id);
            addChild(pCheckbox);

            spTooltip pSprite = spTooltip::create();
            pSprite->setResAnim(pAnim);
            if (pAnim != nullptr)
            {
                pSprite->setScale(static_cast<float>(GameMap::getImageSize() * 2) / static_cast<float>(pAnim->getWidth()));
            }
            pSprite->setPosition(x + 45, y);
            addChild(pSprite);

            spLabel pLabel = spLabel::create(350);
            pLabel->setStyle(style);
            pLabel->setText(name + "\n" + tr("Cost: %0").arg(cost));
            pLabel->setHeight(pLabel->getTextRect().height());
            pLabel->setPosition(x + GameMap::getImageSize() * 2 + 50, y);
            addChild(pLabel);

            x += width;
            if (x + width > getWidth() &&
                i < group.perks.size() - 1)
            {
                x = 0;
                y += GameMap::getImageSize() * 2 + 10;
            }
        }
        y += GameMap::getImageSize() * 2 + 20;
    }
    y += GameMap::getImageSize() * 2 + 10;
    setHeight(y);
    updatePerkCount();
}

QVector<PerkSelection::PerkGroup> PerkSelection::getPerksGrouped()
{
    QVector<PerkSelection::PerkGroup> ret;
    
    COPerkManager* pCOPerkManager = COPerkManager::getInstance();
    qint32 count = pCOPerkManager->getCount();
    for (qint32 i = 0; i < count; i++)
    {
        QString id = pCOPerkManager->getID(i);
        if (pCOPerkManager->isSelectable(i) &&
            !m_hiddenPerks.contains(id) &&
            (m_banning || m_pMap->getGameRules()->getAllowedPerks().contains(id)))
        {
            QString groupName = pCOPerkManager->getGroup(i);
            bool found = false;
            for (auto & group : ret)
            {
                if (group.name == groupName)
                {
                    group.perks.append(i);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                PerkGroup newGroup;
                newGroup.name = groupName;
                newGroup.perks.append(i);
                ret.append(newGroup);
            }
        }
    }
    return ret;
}

qint32 PerkSelection::getPerkScore(const QStringList & perks) const
{
    qint32 perkScore = 0;
    for (const auto & perk : qAsConst(perks))
    {
        perkScore += getPerkScore(perk);
    }
    return perkScore;
}

qint32 PerkSelection::getPerkScore(const QString & perk) const
{
    qint32 perkScore = 0;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue erg = pInterpreter->doFunction(perk, "getCosts");
    if (erg.isNumber())
    {
        perkScore = erg.toInt();
    }
    else
    {
        CONSOLE_PRINT("Unable to get costs for perk " + perk, GameConsole::eERROR);
        perkScore = 1;
    }
    return perkScore;
}

void PerkSelection::updatePerkCount()
{
    if (!m_banning)
    {
        // make sure perk list is valid
        auto perks = m_pCO->getPerkList();
        qint32 maxPerkCost = m_pMap->getGameRules()->getMaxPerkCost();
        qint32 maxPerkCount = m_pMap->getGameRules()->getMaxPerkCount();
        while (getPerkScore(perks) > maxPerkCost ||
               perks.size() > maxPerkCount)
        {
            perks.removeLast();
        }
        m_pCO->setPerkList(perks);
        m_perks = perks;

        qint32 perkScore = getPerkScore(perks);
        bool enable = (m_pCO->getPerkList().size() < m_maxPerkCount);
        for (qint32 i = 0; i < m_Checkboxes.size(); ++i)
        {
            bool selectable = getPerkEnabled(m_perkIds[i]);
            m_Checkboxes[i]->setChecked(m_perks.contains(m_perkIds[i]));
            if (selectable)
            {
                if ((perkScore + getPerkScore(m_perkIds[i]) <= m_maxPerkCosts && enable) || m_Checkboxes[i]->getChecked())
                {
                    m_Checkboxes[i]->setEnabled(true);
                }
                else
                {
                    m_Checkboxes[i]->setEnabled(false);
                }
            }
            else
            {
                m_Checkboxes[i]->setEnabled(false);
            }
        }
        emit sigViewPerkUpdate();
    }
}

bool PerkSelection::getPerkEnabled(QString perkId)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(m_pCO)});
    QJSValue value = pInterpreter->doFunction(perkId, "getPerkEnabled", args);
    if (value.isBool())
    {
        return value.toBool();
    }
    return true;
}

void PerkSelection::toggleAll(bool toggle)
{
    for (auto & checkbox : m_Checkboxes)
    {
        checkbox->setChecked(toggle);
    }
    if (toggle)
    {
        COPerkManager* pCOPerkManager = COPerkManager::getInstance();
        m_perks = pCOPerkManager->getLoadedRessources();
    }
    else
    {
       m_perks.clear();
    }
}

QStringList PerkSelection::getHiddenPerks() const
{
    return m_hiddenPerks;
}

void PerkSelection::setHiddenPerks(const QStringList &hiddenPerks)
{
    m_hiddenPerks = hiddenPerks;
}

const QStringList & PerkSelection::getPerks() const
{
    return m_perks;
}

void PerkSelection::setPerks(const QStringList &perks)
{
    m_perks = perks;
    if (!m_banning)
    {
        m_pCO->setPerkList(perks);
    }
    COPerkManager* pCOPerkManager = COPerkManager::getInstance();    
    qint32 count = m_perkIds.size();
    for (qint32 i = 0; i < count; ++i)
    {
        QString id = m_perkIds[i];
        if (pCOPerkManager->isSelectable(id) &&
            (m_banning || m_pMap->getGameRules()->getAllowedPerks().contains(id)))
        {
            if (m_perks.contains(id))
            {
                m_Checkboxes[i]->setChecked(true);
            }
            else
            {
                m_Checkboxes[i]->setChecked(false);
            }
        }
    }
    updatePerkCount();
}

void PerkSelection::selectRandomPerks(bool fill)
{
    QStringList perks = m_pMap->getGameRules()->getAllowedPerks();
    QStringList selectedPerks;
    if (fill)
    {
        selectedPerks = m_pCO->getPerkList();
    }
    for (const auto & perk : qAsConst(selectedPerks))
    {
        perks.removeAll(perk);
    }
    qint32 failCount = 0;
    while (getPerkScore(selectedPerks) <= m_maxPerkCosts &&
           selectedPerks.size() < m_maxPerkCount &&
           perks.size() > 0 &&
           failCount < 10000)
    {
        qint32 index = GlobalUtils::randInt(0, perks.size() - 1);
        QString perk = perks[index];
        selectedPerks.append(perk);
        if (getPerkScore(selectedPerks) <= m_maxPerkCosts)
        {
            perks.removeAt(index);
            failCount = 0;
        }
        else
        {
            selectedPerks.removeAll(perk);
            ++failCount;
        }
    }
    setPerks(selectedPerks);
}
