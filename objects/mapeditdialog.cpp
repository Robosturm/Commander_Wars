#include "mapeditdialog.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

MapEditDialog::MapEditDialog(QString mapName, qint32 mapWidth, qint32 mapHeigth, qint32 playerCount)
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
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));

    // Label
    oxygine::spTextField text = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont();
    text->setStyle(style);
    text->setText(tr("Map name:").toStdString().c_str());
    text->setPosition(30, 30);
    pSpriteBox->addChild(text);
    qint32 width = text->getTextRect().getWidth();
    m_MapName = new Textbox(pApp->getSettings()->getWidth() - 60 - width);
    m_MapName->setPosition(text->getX() + width, text->getY());
    m_MapName->setCurrentText(mapName);
    pSpriteBox->addChild(m_MapName);

    float y = m_MapName->getY() + m_MapName->getHeight();
    // Label
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Map width:").toStdString().c_str());
    text->setPosition(30, 5 + y );
    pSpriteBox->addChild(text);
    width = text->getTextRect().getWidth();
    m_MapWidth = new SpinBox(300, 1, 999, SpinBox::Mode::Float);
    m_MapWidth->setPosition(text->getX() + width, text->getY());
    m_MapWidth->setCurrentValue(mapWidth);
    pSpriteBox->addChild(m_MapWidth);

    // Label
    y = m_MapWidth->getY() + m_MapWidth->getHeight();
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Map heigth:").toStdString().c_str());
    text->setPosition(30, 5 + y + text->getHeight());
    pSpriteBox->addChild(text);
    width = text->getTextRect().getWidth();
    m_MapHeigth = new SpinBox(300, 1, 999, SpinBox::Mode::Float);
    m_MapHeigth->setPosition(text->getX() + width, text->getY());
    m_MapHeigth->setCurrentValue(mapHeigth);
    pSpriteBox->addChild(m_MapHeigth);

    // Label
    y = m_MapHeigth->getY() + m_MapHeigth->getHeight();
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Map Player:").toStdString().c_str());
    text->setPosition(30, 5 + y + text->getHeight());
    pSpriteBox->addChild(text);
    width = text->getTextRect().getWidth();
    m_MapPlayerCount = new SpinBox(300, 2, 40, SpinBox::Mode::Float);
    m_MapPlayerCount->setPosition(text->getX() + width, text->getY());
    m_MapPlayerCount->setCurrentValue(playerCount);
    pSpriteBox->addChild(m_MapPlayerCount);

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(pApp->getSettings()->getWidth() - m_OkButton->getWidth() - 30, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit editFinished(m_MapName->getCurrentText(), static_cast<qint32>(m_MapWidth->getCurrentValue()),
                          static_cast<qint32>(m_MapHeigth->getCurrentValue()), static_cast<qint32>(m_MapPlayerCount->getCurrentValue()));
        this->getParent()->removeChild(this);
    });

    // cancel button
    m_ExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_ExitButton->setPosition(30, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_ExitButton);
    m_ExitButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        this->getParent()->removeChild(this);
    });
}
