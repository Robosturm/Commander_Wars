#include "dropdownmenu.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

DropDownmenu::DropDownmenu(qint32 width, QVector<QString> items, bool up)
    : m_ItemTexts(items)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setWidth(width);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("dropdownmenu");
    m_Textbox = new oxygine::Box9Sprite();
    m_Textbox->setResAnim(pAnim);
    m_Textbox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_Textbox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);

    m_Textfield = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    m_Textfield->setStyle(style);
    if (items.size() > 0)
    {
        m_Textfield->setText(items[0].toStdString().c_str());
    }
    else
    {
        m_Textfield->setText("unknown");
    }
    oxygine::spClipRectActor pClipActor = new oxygine::ClipRectActor();
    m_Textfield->attachTo(pClipActor);
    m_Textbox->addChild(pClipActor);
    m_Textbox->setSize(width, 40);
    m_Textfield->setWidth(m_Textbox->getWidth() - 20 - 45);
    m_Textfield->setHeight(m_Textbox->getHeight());
    pClipActor->setSize(m_Textfield->getSize());
    pClipActor->setX(10);
    this->addChild(m_Textbox);

    qint32 scrollHeigth = 6 * 40;
    if (items.size() < 6)
    {
        scrollHeigth = (items.size() + 1) * 40;
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


    m_pArrowDown = new oxygine::Button();
    m_Textbox->addChild(m_pArrowDown);
    m_pArrowDown->setPosition(m_Textbox->getWidth() - 45, 10);
    // pButton->setPosition(200, 200);
    m_pArrowDown->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+down"));
    m_pArrowDown->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        m_pArrowDown->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(16, 16, 16, 0)), 300);
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        m_pArrowDown->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(0, 0, 0, 0)), 300);
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        if (m_Panel->getVisible())
        {
            this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
        }
        else
        {
            this->setPriority(static_cast<short>(Mainapp::ZOrder::FocusedObjects));
        }
        this->m_Panel->setVisible(!this->m_Panel->getVisible());
    });
    this->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        if (m_Panel->getVisible())
        {
            this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
        }
        this->m_Panel->setVisible(false);
    });
    for (qint32 i = 0; i < m_ItemTexts.size(); i++)
    {
        addDropDownItem(m_ItemTexts[i], i);
    }
}

void DropDownmenu::setEnabled(bool value)
{
    oxygine::Actor::setEnabled(value);
    m_pArrowDown->setEnabled(value);
    m_Textbox->setEnabled(value);
    m_Panel->setVisible(false);
}

qint32 DropDownmenu::getCurrentItem() const
{
    return m_currentItem;
}

void DropDownmenu::setCurrentItem(qint32 index)
{
    if ((index >= 0) && (index < m_ItemTexts.size()))
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        m_currentItem = index;
        m_Textfield->setText(m_ItemTexts[index].toStdString().c_str());
        pApp->continueThread();
    }
}

QString DropDownmenu::getCurrentItemText()
{
    return QString(m_Textfield->getText().c_str());
}

void DropDownmenu::setCurrentItemText(QString value)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Textfield->setText(value.toStdString().c_str());
    m_currentItem = -1;
    pApp->continueThread();
}

void DropDownmenu::addDropDownItem(QString text, qint32 id)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("topbar+dropdown");
    oxygine::spBox9Sprite pBox = new oxygine::Box9Sprite();
    pBox->setResAnim(pAnim);
    pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
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
