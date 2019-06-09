#include "mapeditdialog.h"

#include "coreengine/mainapp.h"

#include "objects/filedialog.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

MapEditDialog::MapEditDialog(QString mapName, QString author, QString description, QString scriptFile, qint32 mapWidth, qint32 mapHeigth, qint32 playerCount)
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
    text->setText(tr("Map name:").toStdString().c_str());    
    text->setPosition(30, y);
    pSpriteBox->addChild(text);
    m_MapName = new Textbox(pApp->getSettings()->getWidth() - 60 - width);
    m_MapName->setPosition(text->getX() + width, text->getY());
    m_MapName->setCurrentText(mapName);
    pSpriteBox->addChild(m_MapName);
    y += 40;

    // Label
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Map Author:").toStdString().c_str());
    text->setPosition(30, y);
    pSpriteBox->addChild(text);
    m_MapAuthor = new Textbox(pApp->getSettings()->getWidth() - 60 - width);
    m_MapAuthor->setPosition(text->getX() + width, text->getY());
    m_MapAuthor->setCurrentText(author);
    pSpriteBox->addChild(m_MapAuthor);
    y += 40;

    // Label
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Map Description:").toStdString().c_str());
    text->setPosition(30, y);
    pSpriteBox->addChild(text);
    m_MapDescription = new Textbox(pApp->getSettings()->getWidth() - 60 - width);
    m_MapDescription->setPosition(text->getX() + width, text->getY());
    m_MapDescription->setCurrentText(description);
    pSpriteBox->addChild(m_MapDescription);
    y += 40;

    // Label
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Map Script:").toStdString().c_str());
    text->setPosition(30, y);
    pSpriteBox->addChild(text);
    m_ScriptButton = pObjectManager->createButton(tr("Select File"), 150);
    m_ScriptButton->setPosition(pApp->getSettings()->getWidth() - m_ScriptButton->getWidth() - 30, y);
    pSpriteBox->addChild(m_ScriptButton);
    m_MapScriptFile = new Textbox(m_ScriptButton->getX() - text->getX() - width);
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
    text->setText(tr("Map width:").toStdString().c_str());
    text->setPosition(30, 5 + y );
    pSpriteBox->addChild(text);
    m_MapWidth = new SpinBox(300, 1, 999, SpinBox::Mode::Int);
    m_MapWidth->setPosition(text->getX() + width, text->getY());
    m_MapWidth->setCurrentValue(mapWidth);
    pSpriteBox->addChild(m_MapWidth);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Map heigth:").toStdString().c_str());
    text->setPosition(30, 5 + y + text->getHeight());
    pSpriteBox->addChild(text);
    m_MapHeigth = new SpinBox(300, 1, 999, SpinBox::Mode::Int);
    m_MapHeigth->setPosition(text->getX() + width, text->getY());
    m_MapHeigth->setCurrentValue(mapHeigth);
    pSpriteBox->addChild(m_MapHeigth);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Map Player:").toStdString().c_str());
    text->setPosition(30, 5 + y + text->getHeight());
    pSpriteBox->addChild(text);
    m_MapPlayerCount = new SpinBox(300, 2, 40, SpinBox::Mode::Int);
    m_MapPlayerCount->setPosition(text->getX() + width, text->getY());
    m_MapPlayerCount->setCurrentValue(playerCount);
    pSpriteBox->addChild(m_MapPlayerCount);

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(pApp->getSettings()->getWidth() - m_OkButton->getWidth() - 30, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit editFinished(m_MapName->getCurrentText(), m_MapAuthor->getCurrentText(),
                          m_MapDescription->getCurrentText(), m_MapScriptFile->getCurrentText(),
                          static_cast<qint32>(m_MapWidth->getCurrentValue()), static_cast<qint32>(m_MapHeigth->getCurrentValue()),
                          static_cast<qint32>(m_MapPlayerCount->getCurrentValue()));
        this->getParent()->removeChild(this);
    });

    // cancel button
    m_ExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_ExitButton->setPosition(30, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_ExitButton);
    m_ExitButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        this->getParent()->removeChild(this);
        emit sigCanceled();
    });
}

void MapEditDialog::scriptFileChanged(QString file)
{
    file = file.replace(QCoreApplication::applicationDirPath() + "/", "");
    file = file.replace(QCoreApplication::applicationDirPath(), "");
    m_MapScriptFile->setCurrentText(file);
}
