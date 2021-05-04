#include "objects/base/topbar.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "objects/base/tooltip.h"

Topbar::Topbar(qint32 x, qint32 width)
{
    setObjectName("Topbar");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    // create the box for the bar in which everything will be placed
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_pSpriteBox = oxygine::spBox9Sprite::create();
    m_pSpriteBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    this->addChild(m_pSpriteBox);
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("editorselection");
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setSize(width, 80);
    m_pSpriteBox->setPosition(x, 0);
    m_pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setSize(width, 80);
}

void Topbar::focusedLost()
{
    hide();
}

void Topbar::hide()
{    
    for (qint32 i = 0; i < m_Items.size(); i++)
    {
        for (qint32 i2 = 0; i2 < m_Items.at(i).size(); i2++)
        {
            m_Items.at(i).at(i2)->setVisible(false);
        }
    }
    
}

void Topbar::addItem(QString text, QString itemID, qint32 group, QString tooltip)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("topbar+dropdown");

    spTooltip pTooltip = spTooltip::create();
    pTooltip->setTooltipText(tooltip);
    pTooltip->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    oxygine::spBox9Sprite pBox = oxygine::spBox9Sprite::create();
    pBox->setVerticalMode(oxygine::Box9Sprite::TILING);
    pBox->setHorizontalMode(oxygine::Box9Sprite::TILING);
    pBox->setResAnim(pAnim);
    oxygine::spClipRectActor clipRect = oxygine::spClipRectActor::create();
    clipRect->setSize(280, 40);
    oxygine::spTextField textField = oxygine::spTextField::create();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    style.multiline = false;
    textField->setStyle(style);
    textField->setHtmlText(text);
    textField->attachTo(clipRect);
    clipRect->attachTo(pBox);
    textField->setY(5);
    pBox->setSize(300, 40);
    textField->setSize(pBox->getSize());
    pBox->setPosition(m_Buttons.at(group)->getX(), 65 + 40 * m_Items.at(group).size());
    m_Items[group].append(pBox);
    pBox->setVisible(false);
    pBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));
    pTooltip->addChild(pBox);
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
            for (qint32 i2 = 0; i2 < m_Items.at(i).size(); i2++)
            {
                m_Items.at(i).at(i2)->setVisible(false);
            }
        }
        emit pTooltip->sigHideTooltip();
        emit sigItemClicked(itemID);
    });
    addChild(pTooltip);

    
}

void Topbar::addGroup(QString text)
{
    
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    oxygine::spTextField textField = oxygine::spTextField::create();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
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
            for (qint32 i2 = 0; i2 < m_Items.at(i).size(); i2++)
            {
                m_Items.at(i).at(i2)->setVisible(false);
            }
        }
        // show selected selection
        for (qint32 i = 0; i < m_Items.at(groupID).size(); i++)
        {
            m_Items.at(groupID).at(i)->setVisible(true);
        }
        emit sigFocused();
    });
    m_Buttons.append(pButton);
    m_Items.append(QVector<oxygine::spBox9Sprite>());

    
}
