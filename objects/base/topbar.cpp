#include "objects/base/topbar.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

Topbar::Topbar(qint32 x, qint32 width)
{
    setObjectName("Topbar");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    // create the box for the bar in which everything will be placed
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_pSpriteBox = oxygine::spBox9Sprite::create();
    addChild(m_pSpriteBox);
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("editorselection");
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setSize(width, 80);
    m_pSpriteBox->setPosition(x, 0);
    m_pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
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
        m_ItemPanels[i]->setVisible(false);
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
    pBox->setResAnim(pAnim);
    oxygine::spClipRectActor clipRect = oxygine::spClipRectActor::create();
    clipRect->setSize(280, 40);
    oxygine::spTextField textField = oxygine::spTextField::create();
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    style.multiline = false;
    textField->setStyle(style);
    textField->setHtmlText(text);
    clipRect->addChild(textField);
    pBox->addChild(clipRect);
    textField->setY(5);
    pBox->setSize(300, 40);
    textField->setSize(pBox->getSize());
    pBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));
    pTooltip->addChild(pBox);
    pTooltip->setPosition(0, 40 * m_Items.at(group).size());
    // add some event handling :)
    auto* pPtrBox = pBox.get();
    pBox->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event*)
    {
        pPtrBox->addTween(oxygine::Sprite::TweenAddColor(QColor(32, 200, 32, 0)), oxygine::timeMS(300));
    });
    pBox->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event*)
    {
        pPtrBox->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    auto* pPtrTooltip = pTooltip.get();
    pBox->addEventListener(oxygine::TouchEvent::CLICK, [this, pPtrTooltip, itemID](oxygine::Event*)
    {
        for (qint32 i = 0; i < m_Items.size(); i++)
        {
            m_ItemPanels[i]->setVisible(false);
        }
        emit pPtrTooltip->sigHideTooltip();
        emit sigItemClicked(itemID);
    });
    m_Items[group].append(pTooltip);
}

void Topbar::addGroup(QString text)
{
    
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    oxygine::spTextField textField = oxygine::spTextField::create();
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
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
    pButton->addEventListener(oxygine::TouchEvent::CLICK, [this, groupID](oxygine::Event *)->void
    {
        // hide selection
        for (qint32 i = 0; i < m_ItemPanels.size(); i++)
        {
            m_ItemPanels[i]->setVisible(false);
        }
        // show selected selection
        m_ItemPanels[groupID]->setVisible(true);
        emit sigFocused();
    });
    m_Buttons.append(pButton);
    m_Items.append(QVector<spTooltip>());
}

void Topbar::finishCreation()
{
    qint32 maxHeight = Settings::getHeight() / 2;
    for (qint32 i = 0; i < m_Items.size(); ++i)
    {
        qint32 panelHeight = m_Items[i].size() + 1 * 40;
        if (maxHeight > (m_Items[i].size() + 1 * 40))
        {
            panelHeight = maxHeight;
        }
        QSize size(333, panelHeight);
        spPanel pPanel = spPanel::create(false, size, QSize(size.width(), m_Items[i].size() * 40));
        pPanel->setSubComponent(true);
        pPanel->setVisible(false);
        pPanel->setStopMouseWheel(true);
        for (auto & item : m_Items[i])
        {
            pPanel->addItem(item);
        }
        addChild(pPanel);
        pPanel->setPosition(m_Buttons.at(i)->getX(), 65);
        m_ItemPanels.append(pPanel);
    }
}
