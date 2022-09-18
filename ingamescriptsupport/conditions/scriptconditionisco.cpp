#include "ingamescriptsupport/conditions/scriptconditionisco.h"
#include "ingamescriptsupport/scriptdata.h"
#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "coreengine/console.h"

#include "objects/base/label.h"
#include "objects/base/spinbox.h"
#include "objects/base/dropdownmenusprite.h"

#include "resource_management/cospritemanager.h"
#include "resource_management/fontmanager.h"

ScriptConditionIsCo::ScriptConditionIsCo(GameMap* pMap)
    : ScriptCondition(pMap, ConditionType::buildingCaptured)
{
}

void ScriptConditionIsCo::writePreCondition(QTextStream& rStream)
{
    m_executed = ScriptData::getVariableName();
    rStream << "        var " << m_executed << " = " << ScriptData::variables << ".createVariable(\"" << m_executed << "\");\n";
    if (subCondition.get() != nullptr)
    {
        subCondition->writePreCondition(rStream);
    }
}

void ScriptConditionIsCo::writePostCondition(QTextStream& rStream)
{
    ScriptCondition::writePostCondition(rStream);
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    rStream << "            " << m_executed << ".writeDataBool(true);\n";
}

void ScriptConditionIsCo::writeCondition(QTextStream& rStream)
{
    CONSOLE_PRINT("Writing ConditionIsCo", Console::eDEBUG);
    rStream << "        if (map.getPlayer(" << QString::number(m_player) << ").getCO(" << QString::number(m_playerCo) << ").getCoID() === \"" << m_coid << "\" && " << m_executed << ".readDataBool() === false) {"
            << "// " << QString::number(getVersion()) << " "  << ConditionIsCo << "\n";
    if (subCondition.get() != nullptr)
    {
        subCondition->writeCondition(rStream);
    }
    else if (pParent != nullptr)
    {
        pParent->writePostCondition(rStream);
        for (qint32 i = 0; i < events.size(); i++)
        {
            events[i]->writeEvent(rStream);
        }
        rStream << "            " << m_executed << ".writeDataBool(true);\n";
    }
    else
    {
        for (qint32 i = 0; i < events.size(); i++)
        {
            events[i]->writeEvent(rStream);
        }
        rStream << "            " << m_executed << ".writeDataBool(true);\n";
    }
    rStream << "        } // " + ConditionIsCo + " End\n";
}

void ScriptConditionIsCo::readCondition(QTextStream& rStream, QString line)
{
    CONSOLE_PRINT("Reading ConditionIsCo", Console::eDEBUG);
    line = line.simplified();
    QStringList items = line.replace("if (map.getPlayer(", "")
                            .replace(").getCO(", ",")
                            .replace(").getCoID() === \"", ",")
                            .replace("\" && ", ",")
                            .split(",");
    if (items.size() >= 3)
    {
        m_player = items[0].toInt();
        m_playerCo = items[1].toInt();
        m_coid = items[2];
    }
    while (!rStream.atEnd())
    {
        if (readSubCondition(m_pMap, rStream, ConditionIsCo, line))
        {
            break;
        }
        spScriptEvent event = ScriptEvent::createReadEvent(m_pMap, rStream, line);
        if (event.get() != nullptr)
        {
            events.append(event);
        }
    }
}

void ScriptConditionIsCo::showEditCondition(spScriptEditor pScriptEditor)
{
    spGenericBox pBox = spGenericBox::create();

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;

    spLabel pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Player: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(300, 0, 99999);
    spinBox->setTooltipText(tr("Player of which a co should be checked."));
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(m_player);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this,
            [this](qreal value)
    {
        m_player = static_cast<qint32>(value);
    });
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("CO Position: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 1, 2);
    spinBox->setTooltipText(tr("CO of the player which should be checked."));
    spinBox->setPosition(width, 70);
    spinBox->setCurrentValue(m_playerCo + 1);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this,
            [this](qreal value)
    {
        m_playerCo = value - 1;
    });
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("CO: "));
    pText->setPosition(30, 110);
    pBox->addItem(pText);

    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    auto coCreator = [this, pCOSpriteManager](QString id)
    {
        oxygine::ResAnim* pAnim = nullptr;
        if (id.isEmpty())
        {
            pAnim = pCOSpriteManager->getResAnim("no_co+info");
        }
        else
        {
            pAnim = pCOSpriteManager->getResAnim(id + "+info");
        }
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        pSprite->setResAnim(pAnim);
        pSprite->setScale(pAnim->getWidth() / 32.0f);
        pSprite->setSize(pAnim->getSize());
        return pSprite;
    };
    QStringList coIds;
    pCOSpriteManager->getCoGroups(coIds);
    coIds.push_front("");
    spDropDownmenuSprite pCoSelector = spDropDownmenuSprite::create(150, coIds, coCreator);
    pCoSelector->setTooltipText(tr("CO which should be selected."));
    pCoSelector->setPosition(width, 110);
    pCoSelector->setCurrentItem(m_coid);
    connect(pCoSelector.get(), &DropDownmenuSprite::sigItemChanged, this,
            [this, coIds](qint32 value)
    {
        m_coid = coIds[value];
    });
    pBox->addItem(pCoSelector);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateConditios, Qt::QueuedConnection);
}
