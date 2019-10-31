#include "objects/topbar.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

Topbar::Topbar(qint32 x, qint32 width)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    // create the box for the bar in which everything will be placed
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_pSpriteBox = new oxygine::Box9Sprite();
    m_pSpriteBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    this->addChild(m_pSpriteBox);
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("editorselection");
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setSize(width, 80);
    m_pSpriteBox->setPosition(x, 0);
    m_pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event *)->void
    {
        for (qint32 i = 0; i < m_Items.size(); i++)
        {
            for (qint32 i2 = 0; i2 < m_Items.at(i)->size(); i2++)
            {
                m_Items.at(i)->at(i2)->setVisible(false);
            }
        }
    });
    setSize(width, 80);
}

void Topbar::hide()
{
    for (qint32 i = 0; i < m_Items.size(); i++)
    {
        for (qint32 i2 = 0; i2 < m_Items.at(i)->size(); i2++)
        {
            m_Items.at(i)->at(i2)->setVisible(false);
        }
    }
}

void Topbar::addItem(QString text, QString itemID, qint32 group)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("topbar+dropdown");
    oxygine::spBox9Sprite pBox = new oxygine::Box9Sprite();
    pBox->setVerticalMode(oxygine::Box9Sprite::TILING);
    pBox->setHorizontalMode(oxygine::Box9Sprite::TILING);
    pBox->setResAnim(pAnim);
    oxygine::spTextField textField = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    style.multiline = true;
    textField->setStyle(style);
    textField->setHtmlText(text);
    textField->attachTo(pBox);
    pBox->setSize(200, 40);
    textField->setSize(pBox->getSize());
    pBox->setPosition(m_Buttons.at(group)->getX(), 65 + 40 * m_Items.at(group)->size());
    m_Items.at(group)->append(pBox);
    pBox->setVisible(false);
    pBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->addChild(pBox);
    // add some event handling :)
    pBox->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(32, 200, 32, 0)), oxygine::timeMS(300));
    });
    pBox->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pBox->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        for (qint32 i = 0; i < m_Items.size(); i++)
        {
            for (qint32 i2 = 0; i2 < m_Items.at(i)->size(); i2++)
            {
                m_Items.at(i)->at(i2)->setVisible(false);
            }
        }
        emit sigItemClicked(itemID);
    });

    pApp->continueThread();
}

void Topbar::addGroup(QString text)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    oxygine::spTextField textField = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    style.multiline = true;
    textField->setStyle(style);
    textField->setHtmlText(text);

    oxygine::spButton pButton = pObjectManager->createButton(text, textField->getTextRect().getWidth() + 20);
    m_pSpriteBox->addChild(pButton);
    qint32 x = 22;
    for (qint32 i = 0; i < m_Buttons.size(); i++)
    {
        x += m_Buttons.at(i)->getWidth() + 5;
    }
    pButton->setPosition(x, 18);
    qint32 groupID = m_Buttons.size();
    pButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *)->void
    {
        // hide selection
        for (qint32 i = 0; i < m_Items.size(); i++)
        {
            for (qint32 i2 = 0; i2 < m_Items.at(i)->size(); i2++)
            {
                m_Items.at(i)->at(i2)->setVisible(false);
            }
        }
        // show selected selection
        for (qint32 i = 0; i < m_Items.at(groupID)->size(); i++)
        {
            m_Items.at(groupID)->at(i)->setVisible(true);
        }
    });
    m_Buttons.append(pButton);
    m_Items.append(new QVector<oxygine::spBox9Sprite>());

    pApp->continueThread();
}
