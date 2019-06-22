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
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
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
    pText->setText(tr("Conditions").toStdString().c_str());
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
    pText->setText(tr("Events").toStdString().c_str());
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
    pText->setText(tr("Immediate Start: ").toStdString().c_str());
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

    pApp->continueThread();
}

void ScriptEditor::selectCondition(qint32 index)
{

}

void ScriptEditor::addCondition()
{

}

void ScriptEditor::addEvent()
{

}
