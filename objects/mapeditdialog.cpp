#include "mapeditdialog.h"

#include "coreengine/mainapp.h"

#include "objects/filedialog.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

MapEditDialog::MapEditDialog(QString mapName, QString author, QString description, QString scriptFile,
                             qint32 mapWidth, qint32 mapHeigth, qint32 playerCount,
                             qint32 turnLimit, quint32 buildLimit)
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("filedialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

    float y = 30;
    qint32 width = 250;
    // Label
    oxygine::spTextField text = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont();
    text->setStyle(style);
    text->setHtmlText(tr("Map name:"));
    text->setPosition(30, y);
    pSpriteBox->addChild(text);
    m_MapName = new Textbox(pApp->getSettings()->getWidth() - 60 - width);
    m_MapName->setTooltipText(tr("The name of the map."));
    m_MapName->setPosition(text->getX() + width, text->getY());
    m_MapName->setCurrentText(mapName);
    pSpriteBox->addChild(m_MapName);
    y += 40;

    // Label
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Map Author:"));
    text->setPosition(30, y);
    pSpriteBox->addChild(text);
    m_MapAuthor = new Textbox(pApp->getSettings()->getWidth() - 60 - width);
    m_MapAuthor->setTooltipText(tr("The author of the map."));
    m_MapAuthor->setPosition(text->getX() + width, text->getY());
    m_MapAuthor->setCurrentText(author);
    pSpriteBox->addChild(m_MapAuthor);
    y += 40;

    // Label
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Map Description:"));
    text->setPosition(30, y);
    pSpriteBox->addChild(text);
    m_MapDescription = new Textbox(pApp->getSettings()->getWidth() - 60 - width);
    m_MapName->setTooltipText(tr("The description for the map."));
    m_MapDescription->setPosition(text->getX() + width, text->getY());
    m_MapDescription->setCurrentText(description);
    pSpriteBox->addChild(m_MapDescription);
    y += 40;

    // Label
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Map Script:"));
    text->setPosition(30, y);
    pSpriteBox->addChild(text);
    m_ScriptButton = pObjectManager->createButton(tr("Select File"), 150);
    m_ScriptButton->setPosition(pApp->getSettings()->getWidth() - m_ScriptButton->getWidth() - 30, y);
    pSpriteBox->addChild(m_ScriptButton);
    m_MapScriptFile = new Textbox(m_ScriptButton->getX() - text->getX() - width);
    m_MapScriptFile->setTooltipText(tr("The relative path from the exe to the script associated with this map."));
    m_MapScriptFile->setPosition(text->getX() + width, text->getY());
    m_MapScriptFile->setCurrentText(scriptFile);
    pSpriteBox->addChild(m_MapScriptFile);
    m_ScriptButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        QVector<QString> wildcards;
        wildcards.append("*.js");
        QString path = QCoreApplication::applicationDirPath() + "/maps";
        spFileDialog fileDialog = new FileDialog(path, wildcards);
        this->addChild(fileDialog);
        connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &MapEditDialog::scriptFileChanged, Qt::QueuedConnection);
    });
    y += 40;

    // Label
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Map width:"));
    text->setPosition(30, 5 + y );
    pSpriteBox->addChild(text);
    m_MapWidth = new SpinBox(300, 1, 999, SpinBox::Mode::Int);
    m_MapWidth->setTooltipText(tr("The width of the map."));
    m_MapWidth->setPosition(text->getX() + width, text->getY());
    m_MapWidth->setCurrentValue(mapWidth);
    pSpriteBox->addChild(m_MapWidth);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Map heigth:"));
    text->setPosition(30, 5 + y + text->getHeight());
    pSpriteBox->addChild(text);
    m_MapHeigth = new SpinBox(300, 1, 999, SpinBox::Mode::Int);
    m_MapWidth->setTooltipText(tr("The heigth of the map."));
    m_MapHeigth->setPosition(text->getX() + width, text->getY());
    m_MapHeigth->setCurrentValue(mapHeigth);
    pSpriteBox->addChild(m_MapHeigth);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Map Player:"));
    text->setPosition(30, 5 + y + text->getHeight());
    pSpriteBox->addChild(text);
    m_MapPlayerCount = new SpinBox(300, 2, 40, SpinBox::Mode::Int);
    m_MapWidth->setTooltipText(tr("The amount of players for this map."));
    m_MapPlayerCount->setPosition(text->getX() + width, text->getY());
    m_MapPlayerCount->setCurrentValue(playerCount);
    pSpriteBox->addChild(m_MapPlayerCount);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Map Turn Limit:"));
    text->setPosition(30, 5 + y + text->getHeight());
    pSpriteBox->addChild(text);
    m_MapTurnLimit = new SpinBox(300, 0, std::numeric_limits<qint32>::max(), SpinBox::Mode::Int);
    m_MapTurnLimit->setTooltipText(tr("The amount of turns in which the map needs to be finished to get a maximum score."));
    m_MapTurnLimit->setPosition(text->getX() + width, text->getY());
    m_MapTurnLimit->setCurrentValue(turnLimit);
    pSpriteBox->addChild(m_MapTurnLimit);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Deploy Limit:"));
    text->setPosition(30, 5 + y + text->getHeight());
    pSpriteBox->addChild(text);
    m_UnitBuildLimit = new SpinBox(300, 0, std::numeric_limits<qint32>::max(), SpinBox::Mode::Int);
    m_UnitBuildLimit->setTooltipText(tr("The amount of units allowed to be deploayed to get a maximum score."));
    m_UnitBuildLimit->setPosition(text->getX() + width, text->getY());
    m_UnitBuildLimit->setCurrentValue(buildLimit);
    pSpriteBox->addChild(m_UnitBuildLimit);

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(pApp->getSettings()->getWidth() - m_OkButton->getWidth() - 30, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit editFinished(m_MapName->getCurrentText(), m_MapAuthor->getCurrentText(),
                          m_MapDescription->getCurrentText(), m_MapScriptFile->getCurrentText(),
                          static_cast<qint32>(m_MapWidth->getCurrentValue()), static_cast<qint32>(m_MapHeigth->getCurrentValue()),
                          static_cast<qint32>(m_MapPlayerCount->getCurrentValue()), static_cast<qint32>(m_MapTurnLimit->getCurrentValue()),
                          static_cast<quint32>(m_UnitBuildLimit->getCurrentValue()));
        detach();
    });

    // cancel button
    m_ExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_ExitButton->setPosition(30, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_ExitButton);
    m_ExitButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        detach();
        emit sigCanceled();
    });
}

void MapEditDialog::scriptFileChanged(QString file)
{
    file = file.replace(QCoreApplication::applicationDirPath() + "/", "");
    file = file.replace(QCoreApplication::applicationDirPath(), "");
    m_MapScriptFile->setCurrentText(file);
}
