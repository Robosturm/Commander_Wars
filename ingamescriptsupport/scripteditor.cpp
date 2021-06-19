#include "qfile.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/dialogs/filedialog.h"
#include "objects/dialogs/dialogmessagebox.h"

#include "ingamescriptsupport/conditions/scriptcondition.h"
#include "ingamescriptsupport/events/scriptevent.h"
#include "ingamescriptsupport/scripteditor.h"

ScriptEditor::ScriptEditor()
    : QObject()
{
    setObjectName("ScriptEditor");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("semidialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    m_Data = spScriptData::create();

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::spTextField pText = oxygine::spTextField::create();
    pText->setStyle(style);
    pText->setHtmlText(tr("Conditions"));
    pText->setPosition(Settings::getWidth() / 2 - pText->getTextRect().getWidth() / 2, 70);
    pSpriteBox->addChild(pText);
    QSize size(Settings::getWidth() - 40, Settings::getHeight() / 2 - 160);
    m_ConditionPanel = spPanel::create(true, size, size);
    m_ConditionPanel->setPosition(30, 110);
    pSpriteBox->addChild(m_ConditionPanel);
    QVector<QString> items = {tr(ScriptCondition::ConditionStartOfTurn.toStdString().c_str()),
                              tr(ScriptCondition::ConditionVictory.toStdString().c_str()),
                              tr(ScriptCondition::ConditionEachDay.toStdString().c_str()),
                              tr(ScriptCondition::ConditionUnitDestroyed.toStdString().c_str()),
                              tr(ScriptCondition::ConditionBuildingDestroyed.toStdString().c_str()),
                              tr(ScriptCondition::ConditionTerrainDestroyed.toStdString().c_str()),
                              tr(ScriptCondition::ConditionBuildingCaptured.toStdString().c_str()),
                              tr(ScriptCondition::ConditionPlayerDefeated.toStdString().c_str()),
                              tr(ScriptCondition::ConditionUnitsDestroyed.toStdString().c_str()),
                              tr(ScriptCondition::ConditionBuildingsOwned.toStdString().c_str()),
                              tr(ScriptCondition::ConditionPlayerReachedArea.toStdString().c_str()),
                              tr(ScriptCondition::ConditionUnitReachedArea.toStdString().c_str()),
                              tr(ScriptCondition::ConditionCheckVariable.toStdString().c_str())};
    m_Conditions = spDropDownmenu::create(300, items);
    m_Conditions->setTooltipText(tr("Condition type you wan't to create. If a condition is selected this condition and the selected one need to be fullfilled to activate the event."));
    m_Conditions->setPosition(30, Settings::getHeight() / 2 - 45);
    pSpriteBox->addChild(m_Conditions);
    // condition button
    oxygine::spButton pConditionButton = pObjectManager->createButton(tr("Add Condition"), 200);
    pConditionButton->setPosition(m_Conditions->getX() + m_Conditions->getWidth() + 10, Settings::getHeight() / 2 - 45);
    pSpriteBox->addChild(pConditionButton);
    pConditionButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigAddCondition();
    });
    connect(this, &ScriptEditor::sigAddCondition, this, &ScriptEditor::addCondition, Qt::QueuedConnection);

    oxygine::spButton pConditionDuplicate = pObjectManager->createButton(tr("Duplicate"), 200);
    pConditionDuplicate->setPosition(pConditionButton->getX() + pConditionButton->getWidth() + 10, Settings::getHeight() / 2 - 45);
    pSpriteBox->addChild(pConditionDuplicate);
    pConditionDuplicate->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigDuplicateCondition();
    });
    connect(this, &ScriptEditor::sigDuplicateCondition, this, &ScriptEditor::duplicateCondition, Qt::QueuedConnection);

    pText = oxygine::spTextField::create();
    pText->setStyle(style);
    pText->setHtmlText(tr("Events"));
    pText->setPosition(Settings::getWidth() / 2 - pText->getTextRect().getWidth() / 2, Settings::getHeight() / 2);
    pSpriteBox->addChild(pText);
    m_EventPanel = spPanel::create(true, size, size);
    m_EventPanel->setPosition(30, Settings::getHeight() / 2 + 40);
    pSpriteBox->addChild(m_EventPanel);
    items = {
        tr(ScriptEvent::EventDialog.toStdString().c_str()),
        tr(ScriptEvent::EventSpawnUnit.toStdString().c_str()),
        tr(ScriptEvent::EventDefeatPlayer.toStdString().c_str()),
        tr(ScriptEvent::EventChangeBuildlist.toStdString().c_str()),
        tr(ScriptEvent::EventAddFunds.toStdString().c_str()),
        tr(ScriptEvent::EventChangeWeather.toStdString().c_str()),
        tr(ScriptEvent::EventChangeCOBar.toStdString().c_str()),
        tr(ScriptEvent::EventModifyTerrain.toStdString().c_str()),
        tr(ScriptEvent::EventModifyUnit.toStdString().c_str()),
        tr(ScriptEvent::EventAnimation.toStdString().c_str()),
        tr(ScriptEvent::EventVictoryInfo.toStdString().c_str()),
        tr(ScriptEvent::EventModifyVariable.toStdString().c_str()),
        tr(ScriptEvent::EventChangeUnitAI.toStdString().c_str()),
        tr(ScriptEvent::EventChangeBuildingOwner.toStdString().c_str()),
        tr(ScriptEvent::EventChangeUnitOwner.toStdString().c_str()),
        tr(ScriptEvent::EventChangePlayerTeam.toStdString().c_str()),
        tr(ScriptEvent::EventSpawnBuilding.toStdString().c_str()),
        tr(ScriptEvent::EventCenterMap.toStdString().c_str()),
        tr(ScriptEvent::EventPlaySound.toStdString().c_str())
    };
    m_Events = spDropDownmenu::create(300, items);
    m_Events->setTooltipText(tr("The new event that should happen once the conditions are met."));
    m_Events->setPosition(30, Settings::getHeight() - 115);
    pSpriteBox->addChild(m_Events);
    // condition button
    oxygine::spButton pEventButton = pObjectManager->createButton(tr("Add Event"), 200);
    pEventButton->setPosition(m_Events->getX() + m_Events->getWidth() + 10, Settings::getHeight() - 115);
    pSpriteBox->addChild(pEventButton);
    pEventButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigAddEvent();
    });
    connect(this, &ScriptEditor::sigAddEvent, this, &ScriptEditor::addEvent, Qt::QueuedConnection);

    pText = oxygine::spTextField::create();
    pText->setStyle(style);
    pText->setHtmlText(tr("Immediate Start: "));
    pText->setPosition(30, 30);
    pSpriteBox->addChild(pText);
    m_ImmediateStart = spCheckbox::create();
    m_ImmediateStart->setTooltipText(tr("If checked the game starts without beeing able to change rules, players or co's."));
    m_ImmediateStart->setPosition(280, 30);
    m_ImmediateStart->setChecked(false);
    pSpriteBox->addChild(m_ImmediateStart);
    connect(m_ImmediateStart.get(), &Checkbox::checkChanged, this, &ScriptEditor::changeImmediateStart, Qt::QueuedConnection);

    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(Settings::getWidth() - pOkButton->getWidth() - 30, Settings::getHeight() - 30 - pOkButton->getHeight());
    pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigShowExitBox();
    });

    oxygine::spButton pSaveButton = pObjectManager->createButton(tr("Save"), 150);
    pSaveButton->setPosition(Settings::getWidth() / 2 - pSaveButton->getWidth() / 2, Settings::getHeight() - 30 - pSaveButton->getHeight());
    pSpriteBox->addChild(pSaveButton);
    pSaveButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigSaveScript();
    });
    oxygine::spButton pLoadButton = pObjectManager->createButton(tr("Load"), 150);
    pLoadButton->setPosition(30, Settings::getHeight() - 30 - pLoadButton->getHeight());
    pSpriteBox->addChild(pLoadButton);
    pLoadButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigLoadScript();
    });
    connect(this, &ScriptEditor::sigSaveScript, this, &ScriptEditor::showSaveScript, Qt::QueuedConnection);
    connect(this, &ScriptEditor::sigLoadScript, this, &ScriptEditor::showLoadScript, Qt::QueuedConnection);
    connect(this, &ScriptEditor::sigUpdateConditions, this, &ScriptEditor::updateConditios, Qt::QueuedConnection);
    connect(this, &ScriptEditor::sigUpdateEvents, this, &ScriptEditor::updateEvents, Qt::QueuedConnection);
    connect(this, &ScriptEditor::sigShowEditCondition, this, &ScriptEditor::showEditCondition, Qt::QueuedConnection);
    connect(this, &ScriptEditor::sigShowEditEvent, this, &ScriptEditor::showEditEvent, Qt::QueuedConnection);
    connect(this, &ScriptEditor::sigDuplicateEvent, this, &ScriptEditor::duplicateEvent, Qt::QueuedConnection);
    connect(this, &ScriptEditor::sigShowExitBox, this, &ScriptEditor::showExitBox, Qt::QueuedConnection);
}

void ScriptEditor::showExitBox()
{
    
    spDialogMessageBox pExit = spDialogMessageBox::create(tr("Do you want to exit the script editor?"), true);
    connect(pExit.get(), &DialogMessageBox::sigOk, this, &ScriptEditor::exitEditor, Qt::QueuedConnection);
    addChild(pExit);
    
}

void ScriptEditor::exitEditor()
{
    
    emit sigFinished();
    detach();
    
}

void ScriptEditor::showSaveScript()
{
    
    QVector<QString> wildcards;
    wildcards.append("*.js");
    QString path = QCoreApplication::applicationDirPath() + "/maps";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards, "");
    this->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &ScriptEditor::saveScript, Qt::QueuedConnection);
    
}

void ScriptEditor::showLoadScript()
{
    
    QVector<QString> wildcards;
    wildcards.append("*.js");
    QString path = QCoreApplication::applicationDirPath() + "/maps";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards, "");
    this->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &ScriptEditor::loadScript, Qt::QueuedConnection);
    
}

void ScriptEditor::saveScript(QString filename)
{
    
    if (filename.endsWith(".js"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream stream(&file);
        m_Data->writeScript(stream);
        file.close();
    }
    
}

void ScriptEditor::loadScript(QString filename)
{
    
    if (filename.endsWith(".js"))
    {
        QFile file(filename);
        file.open(QIODevice::ReadOnly | QIODevice::Truncate);
        QTextStream stream(&file);
        m_Data->readScript(stream);
        file.close();
    }
    updateConditios();
    
}

void ScriptEditor::changeImmediateStart()
{
    m_Data->setStartMode(m_ImmediateStart->getChecked());
}

void ScriptEditor::updateConditios()
{
    
    m_ImmediateStart->setChecked(m_Data->getStartMode());

    m_ConditionPanel->clearContent();
    m_ConditionBoxes.clear();
    qint32 y = 10;

    for (qint32 i = 0; i < m_Data->getDayConditionSize(); i++)
    {
        spScriptCondition pCondition = m_Data->getDayCondition(i);
        addConditionEntry(pCondition, y);
    }

    for (qint32 i = 0; i < m_Data->getVictoryConditionSize(); i++)
    {
        spScriptCondition pCondition = m_Data->getVictoryCondition(i);
        addConditionEntry(pCondition, y);
    }

    for (qint32 i = 0; i < m_Data->getActionConditionSize(); i++)
    {
        spScriptCondition pCondition = m_Data->getActionCondition(i);
        addConditionEntry(pCondition, y);
    }

    m_CurrentCondition = nullptr;
    updateEvents();
    m_ConditionPanel->setContentHeigth(y + 40);
    
}

void ScriptEditor::addConditionEntry(spScriptCondition pCondition, qint32& y)
{
    qint32 x = 450;
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("textbox");
    oxygine::spBox9Sprite pSpritebox = oxygine::spBox9Sprite::create();
    pSpritebox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pSpritebox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pSpritebox->setResAnim(pAnim);
    pSpritebox->setSize(x + 140 * 3 + 10, 50);
    pSpritebox->setPosition(5, y);
    m_ConditionBoxes.append(pSpritebox);
    m_ConditionPanel->addItem(pSpritebox);

    qint32 boxY = 5;
    spScriptCondition condition = pCondition;
    while (condition.get() != nullptr)
    {
        oxygine::spTextField text = oxygine::spTextField::create();
        text->setStyle(style);
        text->setHtmlText(condition->getDescription());
        text->setPosition(10, boxY);
        pSpritebox->addChild(text);

        oxygine::spButton pEditButton = pObjectManager->createButton(tr("Edit"), 130);
        pEditButton->setPosition(x, boxY);
        pSpritebox->addChild(pEditButton);
        pEditButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigShowEditCondition(condition);
        });
        oxygine::spButton pRemoveButton = pObjectManager->createButton(tr("Remove"), 130);
        pRemoveButton->setPosition(x + 140, boxY);
        pSpritebox->addChild(pRemoveButton);
        pRemoveButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            m_Data->removeCondition(condition);
            emit sigUpdateConditions();
        });
        oxygine::spButton pSelectButton = pObjectManager->createButton(tr("Select"), 130);
        pSelectButton->setPosition(x + 140 * 2, boxY);
        pSpritebox->addChild(pSelectButton);
        pSelectButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            for (qint32 i = 0; i < m_ConditionBoxes.size(); i++)
            {
                m_ConditionBoxes[i]->setAddColor(0, 0, 0);
            }
            if (m_CurrentCondition.get() == condition.get())
            {
                m_CurrentCondition = nullptr;
            }
            else
            {
                pSpritebox->setAddColor(32, 200, 32);
                m_CurrentCondition = condition;
            }
            emit sigUpdateEvents();
        });
        condition = condition->getSubCondition();
        if (condition.get() != nullptr)
        {
            boxY += 40;
            pSpritebox->setHeight(pSpritebox->getHeight() + 40);
        }
    }

    y += 54 + boxY;
    m_ConditionPanel->setContentWidth(x + 140 * 3 + 30);
}

void ScriptEditor::updateEvents()
{
    
    m_EventPanel->clearContent();
    qint32 y = 10;
    if (m_CurrentCondition.get() != nullptr)
    {

        for (qint32 i = 0; i < m_CurrentCondition->getEventSize(); i++)
        {
            spScriptEvent pEvent = m_CurrentCondition->getEvent(i);
            addEventEntry(pEvent, y);
        }

    }
    m_EventPanel->setContentHeigth(y + 40);
    
}

void ScriptEditor::addEventEntry(spScriptEvent pEvent, qint32& y)
{
    qint32 x = 450;
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::spTextField text = oxygine::spTextField::create();
    text->setStyle(style);
    text->setHtmlText(pEvent->getDescription());
    text->setPosition(10, y);
    m_EventPanel->addItem(text);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spButton pEditButton = pObjectManager->createButton(tr("Edit"), 130);
    pEditButton->setPosition(x, y);
    m_EventPanel->addItem(pEditButton);
    pEditButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigShowEditEvent(pEvent);
    });
    oxygine::spButton pRemoveButton = pObjectManager->createButton(tr("Remove"), 130);
    pRemoveButton->setPosition(x + 140, y);
    m_EventPanel->addItem(pRemoveButton);
    pRemoveButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        m_CurrentCondition->removeEvent(pEvent);
        emit sigUpdateEvents();
    });
    oxygine::spButton pDuplicateButton = pObjectManager->createButton(tr("Duplicate"), 130);
    pDuplicateButton->setPosition(x + 140 * 2, y);
    m_EventPanel->addItem(pDuplicateButton);
    pDuplicateButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigDuplicateEvent(pEvent);
    });

    y += 40;
    m_EventPanel->setContentWidth(x + 140 * 3 + 10);
}

void ScriptEditor::addCondition()
{
    ScriptCondition::ConditionType type = static_cast<ScriptCondition::ConditionType>(m_Conditions->getCurrentItem());
    spScriptCondition pCondition;
    if (m_CurrentCondition.get() != nullptr &&
        ScriptCondition::sameConditionGroup(m_CurrentCondition->getType(), type))
    {
        spScriptCondition parent = m_CurrentCondition;
        spScriptCondition subCondition = m_CurrentCondition->getSubCondition();
        while (subCondition.get() != nullptr)
        {
            parent = subCondition;
            subCondition = parent->getSubCondition();
        }
        parent->setSubCondition(ScriptCondition::createCondition(type));
    }
    else
    {
        addConditionToData(ScriptCondition::createCondition(type));
    }
    updateConditios();
}

void ScriptEditor::addEvent()
{
    if (m_CurrentCondition.get() != nullptr)
    {
        ScriptEvent::EventType type = static_cast<ScriptEvent::EventType>(m_Events->getCurrentItem());
        m_CurrentCondition->addEvent(ScriptEvent::createEvent(type));
        updateEvents();
    }
}

void ScriptEditor::showEditCondition(spScriptCondition pCondition)
{
    
    pCondition->showEditCondition(this);
    
}

void ScriptEditor::showEditEvent(spScriptEvent pEvent)
{
    
    pEvent->showEditEvent(this);
    
}

void ScriptEditor::duplicateEvent(spScriptEvent pEvent)
{
    QString data;
    QTextStream stream(&data);
    pEvent->writeEvent(stream);
    spScriptEvent pNewEvent = ScriptEvent::createEvent(pEvent->getEventType());
    stream.seek(0);
    QString line = stream.readLine();
    pNewEvent->readEvent(stream, line);
    m_CurrentCondition->addEvent(pNewEvent);
    updateEvents();
}

void ScriptEditor::duplicateCondition()
{
    if (m_CurrentCondition.get() != nullptr)
    {
        QString data;
        QTextStream stream(&data);
        m_CurrentCondition->writeCondition(stream);
        spScriptCondition pNewCondition = ScriptCondition::createCondition(m_CurrentCondition->getType());
        stream.seek(0);
        QString line = stream.readLine();
        pNewCondition->readCondition(stream, line);
        addConditionToData(pNewCondition);
        updateConditios();
    }
}

void ScriptEditor::addConditionToData(spScriptCondition pCondition)
{
    switch (pCondition->getType())
    {
        case ScriptCondition::ConditionType::victory:
        {
            m_Data->addVictoryCondition(pCondition);
            break;
        }
        case ScriptCondition::ConditionType::startOfTurn:
        case ScriptCondition::ConditionType::eachDay:
        {
            m_Data->addDayCondition(pCondition);
            break;
        }
        case ScriptCondition::ConditionType::buildingDestroyed:
        case ScriptCondition::ConditionType::terrainDestroyed:
        case ScriptCondition::ConditionType::buildingCaptured:
        case ScriptCondition::ConditionType::playerDefeated:
        case ScriptCondition::ConditionType::unitsDestroyed:
        case ScriptCondition::ConditionType::buildingsOwned:
        case ScriptCondition::ConditionType::unitDestroyed:
        case ScriptCondition::ConditionType::unitReachedArea:
        case ScriptCondition::ConditionType::playerReachedArea:
        case ScriptCondition::ConditionType::checkVariable:
        {
            m_Data->addActionCondition(pCondition);
            break;
        }
    }
}
