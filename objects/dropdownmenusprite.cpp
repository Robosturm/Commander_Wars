#include "dropdownmenusprite.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

DropDownmenuSprite::DropDownmenuSprite(qint32 width, QVector<QString>& items, std::function<oxygine::spActor(QString item)> creator,
                                       bool up, qint32 dropDownWidth)
    : m_ItemTexts(items),
      m_Creator(creator)
{
    Q_ASSERT(items.size() != 0);
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

    m_pClipActor = new oxygine::ClipRectActor();
    m_Textbox->addChild(m_pClipActor);
    m_Textbox->setSize(width, 40);
    m_pClipActor->setSize(m_Textbox->getSize());
    m_pClipActor->setX(10);
    m_pClipActor->setY(5);
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
        m_pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        m_pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        if (m_Panel->getVisible())
        {
            this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
            if (m_Tooltip)
            {
                m_Tooltip->setVisible(false);
            }
        }
        else
        {
            this->setPriority(static_cast<short>(Mainapp::ZOrder::FocusedObjects));
        }
        this->m_Panel->setVisible(!this->m_Panel->getVisible());
    });
    this->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
        if (m_Tooltip)
        {
            m_Tooltip->setVisible(false);
        }
        this->m_Panel->setVisible(false);
    });
    for (qint32 i = 0; i < m_ItemTexts.size(); i++)
    {
        addDropDownItem(m_ItemTexts[i], i, dropDownWidth);
    }
    setCurrentItem(0);
}

void DropDownmenuSprite::setEnabled(bool value)
{
    oxygine::Actor::setEnabled(value);
    m_pArrowDown->setEnabled(value);
    m_Textbox->setEnabled(value);
    m_Panel->setVisible(false);
}

qint32 DropDownmenuSprite::getCurrentItem() const
{
    return m_currentItem;
}

void DropDownmenuSprite::setCurrentItem(qint32 index)
{
    if ((index >= 0) && (index < m_ItemTexts.size()))
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        m_currentItem = index;
        m_currentText = m_ItemTexts[index];
        m_pClipActor->removeChildren();
        oxygine::spActor pSprite = m_Creator(m_ItemTexts[index]);
        pSprite->setScaleX((m_Textbox->getWidth() - 13 - 65) / pSprite->getWidth());
        pSprite->setScaleY((m_Textbox->getHeight() - 12) / pSprite->getHeight());
        m_pClipActor->addChild(pSprite);
        pApp->continueThread();
    }
}

void DropDownmenuSprite::setCurrentItem(QString item)
{
    for (qint32 i = 0; i < m_ItemTexts.size(); i++)
    {
        if (m_ItemTexts[i] == item)
        {
            setCurrentItem(i);
            break;
        }
    }
}

QString DropDownmenuSprite::getCurrentItemText()
{
    return m_currentText;
}

void DropDownmenuSprite::addDropDownItem(QString spriteID, qint32 id, qint32 dropDownWidth)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("topbar+dropdown");
    oxygine::spBox9Sprite pBox = new oxygine::Box9Sprite();
    pBox->setResAnim(pAnim);
    pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);

    oxygine::spActor pSprite = m_Creator(spriteID);

    pSprite->setPosition(8, 5);
    pSprite->attachTo(pBox);
    pBox->setSize(getWidth() - 33, 40);
    pBox->setPosition(0, 40 * m_Items.size());

    if (dropDownWidth > 0)
    {
        pSprite->setScaleX((dropDownWidth) / pSprite->getWidth());
    }
    else
    {
        pSprite->setScaleX((pBox->getWidth() - 13) / pSprite->getWidth());
    }
    pSprite->setScaleY((pBox->getHeight() - 12) / pSprite->getHeight());



    m_Items.append(pBox);
    pBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->m_Panel->addItem(pBox);
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
        this->m_Panel->setVisible(false);
        setCurrentItem(id);
        emit sigItemChanged(m_currentItem);
    });
}
