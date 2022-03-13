#include "objects/perkselection.h"

#include "coreengine/globalutils.h"

#include "resource_management/coperkmanager.h"
#include "resource_management/fontmanager.h"

#include "game/gamemap.h"
#include "objects/base/label.h"

PerkSelection::PerkSelection(CO* pCO, qint32 width, qint32 maxPerks, bool banning, QStringList hiddenList, GameMap* pMap)
    : m_pCO(pCO),
      m_maxPerks(maxPerks),
      m_banning(banning),
      m_hiddenPerks(hiddenList),
      m_pMap(pMap)
{
    setObjectName("PerkSelection");
    Interpreter::setCppOwnerShip(this);
    setWidth(width);
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
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
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    oxygine::TextStyle largeStyle = oxygine::TextStyle(FontManager::getMainFont32());
    largeStyle.color = FontManager::getFontColor();
    largeStyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    largeStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    qint32 y = 0;
    qint32 x = 0;
    COPerkManager* pCOPerkManager = COPerkManager::getInstance();
    
    const qint32 width = 370;
    auto perkGroups = getPerksGrouped();
    for (const auto & group : perkGroups)
    {
        spLabel textField = spLabel::create(getWidth() - 40);
        textField->setStyle(largeStyle);
        textField->setHtmlText(group.name);
        textField->setPosition(getWidth() / 2 - textField->getTextRect().getWidth() / 2, y);
        addChild(textField);
        y += 45;
        x = 0;
        for (qint32 i = 0; i < group.perks.size(); ++i)
        {
            qint32 index = group.perks[i];
            QString id = pCOPerkManager->getID(index);
            QString name = pCOPerkManager->getName(index);
            QString icon = pCOPerkManager->getIcon(index);
            oxygine::ResAnim* pAnim = pCOPerkManager->getResAnim(icon, oxygine::error_policy::ep_ignore_error);
            QString description = pCOPerkManager->getDescription(index);

            spCheckbox pCheckbox = spCheckbox::create();
            pCheckbox->setPosition(x, y + 5);
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

            oxygine::spSprite pSprite = oxygine::spSprite::create();
            pSprite->setResAnim(pAnim);
            if (pAnim != nullptr)
            {
                pSprite->setScale((GameMap::getImageSize() * 2) / pAnim->getWidth());
            }
            pSprite->setPosition(x + 45, y);
            addChild(pSprite);
            spLabel pLabel = spLabel::create(250);
            pLabel->setStyle(style);
            pLabel->setText(name);
            pLabel->setPosition(x + GameMap::getImageSize() * 2 + 50, y + 10);
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

void PerkSelection::updatePerkCount()
{
    if (!m_banning)
    {
        bool enable = (m_pCO->getPerkList().size() < m_maxPerks);
        for (qint32 i = 0; i < m_Checkboxes.size(); ++i)
        {
            bool selectable = getPerkEnabled(m_perkIds[i]);
            if (selectable)
            {
                if (enable || m_Checkboxes[i]->getChecked())
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

QStringList PerkSelection::getPerks() const
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
    qint32 count = pCOPerkManager->getCount();
    
    for (qint32 i = 0; i < count; i++)
    {
        QString id = pCOPerkManager->getID(i);
        if (pCOPerkManager->isSelectable(i) &&
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
    while (selectedPerks.size() < m_maxPerks &&
           perks.size() > 0)
    {
        qint32 index = GlobalUtils::randInt(0, perks.size() - 1);
        selectedPerks.append(perks[index]);
        perks.removeAt(index);
    }
    setPerks(selectedPerks);
}
