#include <QFile>

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/interpreter.h"

#include "objects/dialogs/filedialog.h"
#include "objects/dialogs/dialogmessagebox.h"

#include "objects/base/label.h"

#include "ingamescriptsupport/conditions/scriptcondition.h"
#include "ingamescriptsupport/events/scriptevent.h"
#include "ingamescriptsupport/scripteditor.h"

ScriptEditor::ScriptEditor(GameMap* pMap)
    : m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("ScriptEditor");
#endif
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = MemoryManagement::create<oxygine::Box9Sprite>();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("semidialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    m_Data = spScriptData::create(m_pMap);

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::spTextField pText = MemoryManagement::create<oxygine::TextField>();
    pText->setStyle(style);
    pText->setHtmlText(tr("Conditions"));
    pText->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - pText->getTextRect().width() / 2, 70);
    pSpriteBox->addChild(pText);
    QSize size(oxygine::Stage::getStage()->getWidth() - 40, oxygine::Stage::getStage()->getHeight() / 2 - 160);
    m_ConditionPanel = MemoryManagement::create<Panel>(true, size, size);
    m_ConditionPanel->setPosition(30, 110);
    pSpriteBox->addChild(m_ConditionPanel);
    QStringList items = {tr("Start of turn"),
                         tr("Victory"),
                         tr("Each day"),
                         tr("Unit destroyed"),
                         tr("Building destroyed"),
                         tr("Terrain destroyed"),
                         tr("Building captured"),
                         tr("Player defeated"),
                         tr("Units destroyed"),
                         tr("Buildings owned"),
                         tr("Player in area"),
                         tr("Unit in area"),
                         tr("Check variable"),
                         tr("Is selected co")};
    m_Conditions = MemoryManagement::create<DropDownmenu>(300, items);
    m_Conditions->setTooltipText(tr("Condition type you want to create. If another condition is selected both must be fulfilled to activate the event."));
    m_Conditions->setPosition(30, oxygine::Stage::getStage()->getHeight() / 2 - 45);
    pSpriteBox->addChild(m_Conditions);
    // condition button
    oxygine::spButton pConditionButton = pObjectManager->createButton(tr("Add Condition"), 200);
    pConditionButton->setPosition(m_Conditions->getX() + m_Conditions->getScaledWidth() + 10, oxygine::Stage::getStage()->getHeight() / 2 - 45);
    pSpriteBox->addChild(pConditionButton);
    pConditionButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigAddCondition();
    });
    connect(this, &ScriptEditor::sigAddCondition, this, &ScriptEditor::addCondition, Qt::QueuedConnection);

    oxygine::spButton pConditionDuplicate = pObjectManager->createButton(tr("Duplicate"), 200);
    pConditionDuplicate->setPosition(pConditionButton->getX() + pConditionButton->getScaledWidth() + 10, oxygine::Stage::getStage()->getHeight() / 2 - 45);
    pSpriteBox->addChild(pConditionDuplicate);
    pConditionDuplicate->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigDuplicateCondition();
    });
    connect(this, &ScriptEditor::sigDuplicateCondition, this, &ScriptEditor::duplicateCondition, Qt::QueuedConnection);

    pText = MemoryManagement::create<oxygine::TextField>();
    pText->setStyle(style);
    pText->setHtmlText(tr("Events"));
    pText->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - pText->getTextRect().width() / 2, oxygine::Stage::getStage()->getHeight() / 2);
    pSpriteBox->addChild(pText);
    m_EventPanel = MemoryManagement::create<Panel>(true, size, size);
    m_EventPanel->setPosition(30, oxygine::Stage::getStage()->getHeight() / 2 + 40);
    pSpriteBox->addChild(m_EventPanel);
    items = {
        tr("Dialog"),
        tr("Spawn unit"),
        tr("Defeat player"),
        tr("Change buildlist"),
        tr("Add funds"),
        tr("Change weather"),
        tr("Change co bar"),
        tr("Modify terrain"),
        tr("Modify unit"),
        tr("Animation"),
        tr("Victory info"),
        tr("Modify variable"),
        tr("Change unit ai"),
        tr("Change building owner"),
        tr("Change unit owner"),
        tr("Change player team"),
        tr("Spawn building"),
        tr("Center map"),
        tr("Play sound"),
        tr("Building fire counter"),
        tr("Volcan fire"),
        tr("Extend map")
    };
    m_Events = MemoryManagement::create<DropDownmenu>(300, items);
    m_Events->setTooltipText(tr("The new event that should happen once the conditions are met."));
    m_Events->setPosition(30, oxygine::Stage::getStage()->getHeight() - 115);
    pSpriteBox->addChild(m_Events);
    // condition button
    m_pEventButton = pObjectManager->createButton(tr("Add Event"), 200);
    m_pEventButton->setPosition(m_Events->getX() + m_Events->getScaledWidth() + 10, oxygine::Stage::getStage()->getHeight() - 115);
    pSpriteBox->addChild(m_pEventButton);
    m_pEventButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigAddEvent();
    });
    m_pEventButton->setEnabled(false);
    connect(this, &ScriptEditor::sigAddEvent, this, &ScriptEditor::addEvent, Qt::QueuedConnection);

    pText = MemoryManagement::create<oxygine::TextField>();
    pText->setStyle(style);
    pText->setHtmlText(tr("Immediate Start: "));
    pText->setPosition(30, 30);
    pSpriteBox->addChild(pText);
    m_ImmediateStart = MemoryManagement::create<Checkbox>();
    m_ImmediateStart->setTooltipText(tr("If checked the game starts without being able to change rules, players or CO's."));
    m_ImmediateStart->setPosition(280, 30);
    m_ImmediateStart->setChecked(false);
    pSpriteBox->addChild(m_ImmediateStart);
    connect(m_ImmediateStart.get(), &Checkbox::checkChanged, this, &ScriptEditor::changeImmediateStart, Qt::QueuedConnection);

    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(oxygine::Stage::getStage()->getWidth() - pOkButton->getScaledWidth() - 30,
                           oxygine::Stage::getStage()->getHeight() - 10 - pOkButton->getScaledHeight());
    pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigShowExitBox();
    });

    oxygine::spButton pSaveButton = pObjectManager->createButton(tr("Save"), 150);
    pSaveButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - pSaveButton->getScaledWidth() / 2,
                             oxygine::Stage::getStage()->getHeight() - 10 - pSaveButton->getScaledHeight());
    pSpriteBox->addChild(pSaveButton);
    pSaveButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigSaveScript();
    });
    oxygine::spButton pLoadButton = pObjectManager->createButton(tr("Load"), 150);
    pLoadButton->setPosition(30, oxygine::Stage::getStage()->getHeight() - 10 - pLoadButton->getScaledHeight());
    pSpriteBox->addChild(pLoadButton);
    pLoadButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
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
    spDialogMessageBox pExit = MemoryManagement::create<DialogMessageBox>(tr("Do you want to exit the script editor?"), true);
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
    QStringList wildcards;
    wildcards.append("*.js");
    QString path = Settings::getInstance()->getUserPath() + "maps";
    spFileDialog fileDialog = MemoryManagement::create<FileDialog>(path, wildcards, true, "", false, tr("Save"));
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &ScriptEditor::saveScript, Qt::QueuedConnection);
}

void ScriptEditor::showLoadScript()
{    
    QStringList wildcards;
    wildcards.append("*.js");
    QString path = Settings::getInstance()->getUserPath() + "maps";
    spFileDialog fileDialog = MemoryManagement::create<FileDialog>(path, wildcards, false, "", false, tr("Load"));
    addChild(fileDialog);
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
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("textbox");
    oxygine::spBox9Sprite pSpritebox = MemoryManagement::create<oxygine::Box9Sprite>();
    pSpritebox->setResAnim(pAnim);
    pSpritebox->setSize(x + 140 * 3 + 10, 50);
    pSpritebox->setPosition(5, y);
    m_ConditionBoxes.append(pSpritebox);
    m_ConditionPanel->addItem(pSpritebox);

    qint32 boxY = 5;
    spScriptCondition condition = pCondition;
    while (condition.get() != nullptr)
    {
        spLabel text = MemoryManagement::create<Label>(420);
        text->setStyle(style);
        text->setHtmlText(condition->getDescription());
        text->setPosition(10, boxY);
        pSpritebox->addChild(text);

        oxygine::spButton pEditButton = pObjectManager->createButton(tr("Edit"), 130);
        pEditButton->setPosition(x, boxY);
        pSpritebox->addChild(pEditButton);
        pEditButton->addEventListener(oxygine::TouchEvent::CLICK, [this, condition](oxygine::Event*)
        {
            emit sigShowEditCondition(condition);
        });
        oxygine::spButton pRemoveButton = pObjectManager->createButton(tr("Remove"), 130);
        pRemoveButton->setPosition(x + 140, boxY);
        pSpritebox->addChild(pRemoveButton);
        pRemoveButton->addEventListener(oxygine::TouchEvent::CLICK, [this, condition](oxygine::Event*)
        {
            m_Data->removeCondition(condition);
            emit sigUpdateConditions();
        });
        oxygine::spButton pSelectButton = pObjectManager->createButton(tr("Select"), 130);
        pSelectButton->setPosition(x + 140 * 2, boxY);
        pSpritebox->addChild(pSelectButton);
        auto pPtrSpritebox = pSpritebox.get();
        auto pCondition = condition.get();
        pSelectButton->addEventListener(oxygine::TouchEvent::CLICK, [this, pPtrSpritebox, pCondition](oxygine::Event*)
        {
            selectCondition(pPtrSpritebox, pCondition);
        });
        condition = condition->getSubCondition();
        if (condition.get() != nullptr)
        {
            boxY += text->getHeight() + 10;
            pSpritebox->setHeight(pSpritebox->getScaledHeight() + text->getHeight() + 10);
        }
    }

    y += 54 + boxY;
    m_ConditionPanel->setContentWidth(x + 140 * 3 + 30);
}

void ScriptEditor::selectCondition(oxygine::Box9Sprite* pPtrSpritebox, ScriptCondition* pCondition)
{
    for (qint32 i = 0; i < m_ConditionBoxes.size(); i++)
    {
        m_ConditionBoxes[i]->setAddColor(0, 0, 0);
    }
    if (m_CurrentCondition == pCondition)
    {
        m_CurrentCondition = nullptr;
    }
    else
    {
        pPtrSpritebox->setAddColor(32, 200, 32);
        m_CurrentCondition = pCondition;
    }
    m_pEventButton->setEnabled(m_CurrentCondition != nullptr);
    emit sigUpdateEvents();
}

void ScriptEditor::updateEvents()
{    
    m_EventPanel->clearContent();
    qint32 y = 10;
    if (m_CurrentCondition != nullptr)
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
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    spLabel text = MemoryManagement::create<Label>(420);
    text->setStyle(style);
    text->setHtmlText(pEvent->getDescription());
    text->setPosition(10, y);
    m_EventPanel->addItem(text);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spButton pEditButton = pObjectManager->createButton(tr("Edit"), 130);
    pEditButton->setPosition(x, y);
    m_EventPanel->addItem(pEditButton);
    auto* pPtrEvent = pEvent.get();
    pEditButton->addEventListener(oxygine::TouchEvent::CLICK, [this, pPtrEvent](oxygine::Event*)
    {
        emit sigShowEditEvent(spScriptEvent(pPtrEvent));
    });
    oxygine::spButton pRemoveButton = pObjectManager->createButton(tr("Remove"), 130);
    pRemoveButton->setPosition(x + 140, y);
    m_EventPanel->addItem(pRemoveButton);
    pRemoveButton->addEventListener(oxygine::TouchEvent::CLICK, [this, pPtrEvent](oxygine::Event*)
    {
        m_CurrentCondition->removeEvent(spScriptEvent(pPtrEvent));
        emit sigUpdateEvents();
    });
    oxygine::spButton pDuplicateButton = pObjectManager->createButton(tr("Duplicate"), 130);
    pDuplicateButton->setPosition(x + 140 * 2, y);
    m_EventPanel->addItem(pDuplicateButton);
    pDuplicateButton->addEventListener(oxygine::TouchEvent::CLICK, [this, pPtrEvent](oxygine::Event*)
    {
        emit sigDuplicateEvent(spScriptEvent(pPtrEvent));
    });

    y += text->getHeight() + 10;
    m_EventPanel->setContentWidth(x + 140 * 3 + 10);
}

void ScriptEditor::addCondition()
{
    ScriptCondition::ConditionType type = static_cast<ScriptCondition::ConditionType>(m_Conditions->getCurrentItem());
    spScriptCondition pCondition;
    if (m_CurrentCondition != nullptr &&
        ScriptCondition::sameConditionGroup(m_CurrentCondition->getType(), type))
    {
        spScriptCondition parent = spScriptCondition(m_CurrentCondition);
        spScriptCondition subCondition = m_CurrentCondition->getSubCondition();
        while (subCondition.get() != nullptr)
        {
            parent = subCondition;
            subCondition = parent->getSubCondition();
        }
        parent->setSubCondition(ScriptCondition::createCondition(m_pMap, type));
    }
    else
    {
        addConditionToData(ScriptCondition::createCondition(m_pMap, type));
    }
    updateConditios();
}

void ScriptEditor::addEvent()
{
    if (m_CurrentCondition != nullptr)
    {
        ScriptEvent::EventType type = static_cast<ScriptEvent::EventType>(m_Events->getCurrentItem());
        m_CurrentCondition->addEvent(ScriptEvent::createEvent(m_pMap, type));
        updateEvents();
    }
}

void ScriptEditor::showEditCondition(spScriptCondition pCondition)
{    
    pCondition->showEditCondition(spScriptEditor(this));
}

void ScriptEditor::showEditEvent(spScriptEvent pEvent)
{    
    pEvent->showEditEvent(spScriptEditor(this));
}

void ScriptEditor::duplicateEvent(spScriptEvent pEvent)
{
    QString data;
    QTextStream stream(&data);
    pEvent->writeEvent(stream);
    spScriptEvent pNewEvent = ScriptEvent::createEvent(m_pMap, pEvent->getEventType());
    stream.seek(0);
    QString line = stream.readLine();
    pNewEvent->readEvent(stream, line);
    m_CurrentCondition->addEvent(pNewEvent);
    updateEvents();
}

void ScriptEditor::duplicateCondition()
{
    if (m_CurrentCondition != nullptr)
    {
        QString data;
        QTextStream stream(&data);
        m_CurrentCondition->writeCondition(stream);
        spScriptCondition pNewCondition = ScriptCondition::createCondition(m_pMap, m_CurrentCondition->getType());
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
        case ScriptCondition::ConditionType::isCo:
        {
            m_Data->addActionCondition(pCondition);
            break;
        }
    }
}
