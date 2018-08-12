#include "dropdownmenu.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

DropDownmenu::DropDownmenu(qint32 width, QVector<QString> items, bool up)
    : m_ItemTexts(items)
{
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setWidth(width);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("dropdownmenu");
    m_Textbox = new oxygine::Box9Sprite();
    m_Textbox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_Textbox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_Textbox->setResAnim(pAnim);
    m_Textfield = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    m_Textfield->setStyle(style);
    m_Textfield->setText(items[0].toStdString().c_str());
    oxygine::spClipRectActor pClipActor = new oxygine::ClipRectActor();
    m_Textfield->attachTo(pClipActor);
    m_Textbox->addChild(pClipActor);
    m_Textbox->setSize(width, 40);
    m_Textfield->setWidth(m_Textbox->getWidth() - 20 - 45);
    m_Textfield->setHeight(m_Textbox->getHeight());
    pClipActor->setSize(m_Textfield->getSize());
    pClipActor->setX(10);
    this->addChild(m_Textbox);

    qint32 scrollHeigth = 5 * 40;
    if (items.size() < 5)
    {
        scrollHeigth = items.size() * 40;
    }
    m_Panel = new Panel(false, QSize(width, scrollHeigth), QSize(width, items.size() * 40));
    m_Panel->setVisible(false);
    if (up)
    {
        m_Panel->setY(-m_Panel->getHeight());
    }
    else
    {
        m_Panel->setY(m_Textbox->getHeight());
    }
    this->addChild(m_Panel);


    oxygine::spButton pArrowDown = new oxygine::Button();
    m_Textbox->addChild(pArrowDown);
    pArrowDown->setPosition(m_Textbox->getWidth() - 45, 10);
    // pButton->setPosition(200, 200);
    pArrowDown->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+down"));
    pArrowDown->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    pArrowDown->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        pArrowDown->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(16, 16, 16, 0)), 300);
    });
    pArrowDown->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        pArrowDown->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(0, 0, 0, 0)), 300);
    });
    pArrowDown->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        this->m_Panel->setVisible(!this->m_Panel->getVisible());
    });
    this->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        this->m_Panel->setVisible(false);
    });
    for (qint32 i = 0; i < m_ItemTexts.size(); i++)
    {
        addDropDownItem(m_ItemTexts[i], i);
    }
}

qint32 DropDownmenu::getCurrentItem() const
{
    return m_currentItem;
}

QString DropDownmenu::getCurrentItemText()
{
    return m_ItemTexts[m_currentItem];
}

void DropDownmenu::addDropDownItem(QString text, qint32 id)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("topbar+dropdown");
    oxygine::spBox9Sprite pBox = new oxygine::Box9Sprite();
    pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setResAnim(pAnim);
    oxygine::spTextField textField = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    style.multiline = true;
    textField->setStyle(style);
    textField->setText(text.toStdString().c_str());
    textField->attachTo(pBox);
    pBox->setSize(getWidth() - 33, 40);
    textField->setSize(pBox->getSize());
    pBox->setPosition(0, 40 * m_Items.size());
    m_Items.append(pBox);
    pBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->m_Panel->addItem(pBox);
    // add some event handling :)
    pBox->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        pBox->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(32, 200, 32, 0)), 300);
    });
    pBox->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        pBox->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(0, 0, 0, 0)), 300);
    });
    pBox->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        m_currentItem = id;
        this->m_Panel->setVisible(false);
        m_Textfield->setText(text.toStdString().c_str());
        emit sigItemChanged(m_currentItem);
    });
}
