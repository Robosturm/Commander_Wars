#include "scripteditor.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/filedialog.h"

#include "ingamescriptsupport/scriptcondition.h"
#include "ingamescriptsupport/scriptevent.h"

#include "qfile.h"

ScriptEditor::ScriptEditor()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("semidialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

    m_Data = new ScriptData();

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::spTextField pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Conditions").toStdString().c_str());
    pText->setPosition(pApp->getSettings()->getWidth() / 4 - pText->getTextRect().getWidth() / 2, 90);
    pSpriteBox->addChild(pText);
    QSize size(pApp->getSettings()->getWidth() / 2 - 40, pApp->getSettings()->getHeight() - 250);
    m_ConditionPanel = new Panel(true, size, size);
    m_ConditionPanel->setPosition(30, 130);
    pSpriteBox->addChild(m_ConditionPanel);
    QVector<QString> items = {tr(ScriptCondition::ConditionStartOfTurn.toStdString().c_str()),
                              tr(ScriptCondition::ConditionVictory.toStdString().c_str()),
                              tr(ScriptCondition::ConditionEachDay.toStdString().c_str())};
    m_Conditions = new DropDownmenu(200, items, true);
    m_Conditions->setPosition(30, pApp->getSettings()->getHeight() - 115);
    pSpriteBox->addChild(m_Conditions);
    // condition button
    oxygine::spButton pConditionButton = pObjectManager->createButton(tr("Add Condition"), 150);
    pConditionButton->setPosition(m_Conditions->getX() + m_Conditions->getWidth() + 10, pApp->getSettings()->getHeight() - 115);
    pSpriteBox->addChild(pConditionButton);
    pConditionButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigAddCondition();
    });
    connect(this, &ScriptEditor::sigAddCondition, this, &ScriptEditor::addCondition, Qt::QueuedConnection);

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Events").toStdString().c_str());
    pText->setPosition(pApp->getSettings()->getWidth() * 3 / 4 - pText->getTextRect().getWidth() / 2, 90);
    pSpriteBox->addChild(pText);
    m_EventPanel = new Panel(true, size, size);
    m_EventPanel->setPosition(pApp->getSettings()->getWidth() / 2 + 10, 130);
    pSpriteBox->addChild(m_EventPanel);
    items = {tr(ScriptEvent::EventDialog.toStdString().c_str()),
             tr(ScriptEvent::EventSpawnUnit.toStdString().c_str()),
             tr(ScriptEvent::EventDefeatPlayer.toStdString().c_str())};
    m_Events = new DropDownmenu(200, items, true);
    m_Events->setPosition(pApp->getSettings()->getWidth() / 2 + 10, pApp->getSettings()->getHeight() - 115);
    pSpriteBox->addChild(m_Events);
    // condition button
    oxygine::spButton pEventButton = pObjectManager->createButton(tr("Add Event"), 150);
    pEventButton->setPosition(m_Events->getX() + m_Events->getWidth() + 10, pApp->getSettings()->getHeight() - 115);
    pSpriteBox->addChild(pEventButton);
    pEventButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigAddEvent();
    });
    connect(this, &ScriptEditor::sigAddEvent, this, &ScriptEditor::addEvent, Qt::QueuedConnection);



    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Immediate Start: ").toStdString().c_str());
    pText->setPosition(30, 30);
    pSpriteBox->addChild(pText);
    m_ImmediateStart = new Checkbox();
    m_ImmediateStart->setPosition(200, 30);
    m_ImmediateStart->setChecked(false);
    pSpriteBox->addChild(m_ImmediateStart);
    connect(m_ImmediateStart.get(), &Checkbox::checkChanged, this, &ScriptEditor::changeImmediateStart, Qt::QueuedConnection);

    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(pApp->getSettings()->getWidth() - pOkButton->getWidth() - 30, pApp->getSettings()->getHeight() - 30 - pOkButton->getHeight());
    pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigFinished();
        this->getParent()->removeChild(this);
    });

    oxygine::spButton pSaveButton = pObjectManager->createButton(tr("Save Script"), 150);
    pSaveButton->setPosition(pApp->getSettings()->getWidth() / 2 - pSaveButton->getWidth() / 2, pApp->getSettings()->getHeight() - 30 - pSaveButton->getHeight());
    pSpriteBox->addChild(pSaveButton);
    pSaveButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigSaveScript();
    });
    oxygine::spButton pLoadButton = pObjectManager->createButton(tr("Load Script"), 150);
    pLoadButton->setPosition(30, pApp->getSettings()->getHeight() - 30 - pLoadButton->getHeight());
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
}

void ScriptEditor::showSaveScript()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    QVector<QString> wildcards;
    wildcards.append("*.js");
    QString path = QCoreApplication::applicationDirPath() + "/maps";
    spFileDialog fileDialog = new FileDialog(path, wildcards, "");
    this->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &ScriptEditor::saveScript, Qt::QueuedConnection);
    pApp->continueThread();
}

void ScriptEditor::showLoadScript()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    QVector<QString> wildcards;
    wildcards.append("*.js");
    QString path = QCoreApplication::applicationDirPath() + "/maps";
    spFileDialog fileDialog = new FileDialog(path, wildcards, "");
    this->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &ScriptEditor::loadScript, Qt::QueuedConnection);
    pApp->continueThread();
}

void ScriptEditor::saveScript(QString filename)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (filename.endsWith(".js"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream stream(&file);
        m_Data->writeScript(stream);
        file.close();
    }
    pApp->continueThread();
}

void ScriptEditor::loadScript(QString filename)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (filename.endsWith(".js"))
    {
        QFile file(filename);
        file.open(QIODevice::ReadOnly | QIODevice::Truncate);
        QTextStream stream(&file);
        m_Data->readScript(stream);
        file.close();
    }
    updateConditios();
    pApp->continueThread();
}

void ScriptEditor::changeImmediateStart()
{
   m_Data->setStartMode(m_ImmediateStart->getChecked());
}

void ScriptEditor::updateConditios()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
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
    pApp->continueThread();
}

void ScriptEditor::addConditionEntry(spScriptCondition pCondition, qint32& y)
{
    qint32 x = 250;
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("textbox");
    oxygine::spBox9Sprite pSpritebox = new oxygine::Box9Sprite();
    pSpritebox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pSpritebox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pSpritebox->setResAnim(pAnim);
    pSpritebox->setSize(x + 100 * 3 + 10, 50);
    pSpritebox->setPosition(5, y);
    m_ConditionBoxes.append(pSpritebox);
    m_ConditionPanel->addItem(pSpritebox);

    qint32 boxY = 5;
    spScriptCondition condition = pCondition;
    while (condition.get() != nullptr)
    {
        oxygine::spTextField text = new oxygine::TextField();
        text->setStyle(style);
        text->setHtmlText(condition->getDescription().toStdString().c_str());
        text->setPosition(10, boxY);
        pSpritebox->addChild(text);

        oxygine::spButton pEditButton = pObjectManager->createButton(tr("Edit"), 90);
        pEditButton->setPosition(x, boxY);
        pSpritebox->addChild(pEditButton);
        pEditButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigShowEditCondition(condition);
        });
        oxygine::spButton pRemoveButton = pObjectManager->createButton(tr("Remove"), 90);
        pRemoveButton->setPosition(x + 100, boxY);
        pSpritebox->addChild(pRemoveButton);
        pRemoveButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            m_Data->removeCondition(condition);
            emit sigUpdateConditions();
        });
        oxygine::spButton pSelectButton = pObjectManager->createButton(tr("Select"), 90);
        pSelectButton->setPosition(x + 100 * 2, boxY);
        pSpritebox->addChild(pSelectButton);
        pSelectButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            for (qint32 i = 0; i < m_ConditionBoxes.size(); i++)
            {
                m_ConditionBoxes[i]->setAddColor(0, 0, 0);
            }
            pSpritebox->setAddColor(32, 200, 32);
            m_CurrentCondition = condition;
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
    m_ConditionPanel->setContentWidth(x + 100 * 3 + 30);
}

void ScriptEditor::updateEvents()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
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
    pApp->continueThread();
}

void ScriptEditor::addEventEntry(spScriptEvent pEvent, qint32& y)
{
    qint32 x = 250;
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::spTextField text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(pEvent->getDescription().toStdString().c_str());
    text->setPosition(10, y);
    m_EventPanel->addItem(text);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spButton pEditButton = pObjectManager->createButton(tr("Edit"), 90);
    pEditButton->setPosition(x, y);
    m_EventPanel->addItem(pEditButton);
    pEditButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigShowEditEvent(pEvent);
    });
    oxygine::spButton pRemoveButton = pObjectManager->createButton(tr("Remove"), 90);
    pRemoveButton->setPosition(x + 100, y);
    m_EventPanel->addItem(pRemoveButton);
    pRemoveButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        m_CurrentCondition->removeEvent(pEvent);
        emit sigUpdateEvents();
    });
    y += 40;
    m_EventPanel->setContentWidth(x + 100 * 2 + 10);
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
        switch (type)
        {
            case ScriptCondition::ConditionType::victory:
            {
                pCondition = m_Data->addVictoryCondition();
                break;
            }
            case ScriptCondition::ConditionType::startOfTurn:
            case ScriptCondition::ConditionType::eachDay:
            {
                pCondition = m_Data->addDayCondition(type);
                break;
            }
        }
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
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    pCondition->showEditCondition(this);
    pApp->continueThread();
}

void ScriptEditor::showEditEvent(spScriptEvent pEvent)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    pEvent->showEditEvent(this);
    pApp->continueThread();
}
