#include "dropdownmenubase.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

DropDownmenuBase::DropDownmenuBase(qint32 width, qint32 itemcount)
{
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setWidth(width);
    setHeight(40);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("dropdownmenu");
    m_Box = new oxygine::Box9Sprite();
    m_Box->setResAnim(pAnim);
    m_Box->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_Box->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_Box->setSize(width, 40);
    m_pClipActor = new oxygine::ClipRectActor();
    m_Box->addChild(m_pClipActor);
    m_pClipActor->setSize(m_Box->getWidth() - 20 - 45, m_Box->getHeight());
    m_pClipActor->setX(10);
    addChild(m_Box);
    qint32 maxItemCount = 6;
    qint32 changedCount = Settings::getHeight() / 40 / 3;
    if (changedCount > maxItemCount)
    {
        maxItemCount = changedCount;
    }
    if (Settings::getHeight() / 2 < maxItemCount * 40)
    {
        maxItemCount = Settings::getHeight() / 40;
    }
    qint32 scrollHeigth = maxItemCount * 40;
    if (itemcount < maxItemCount)
    {
        scrollHeigth = (itemcount + 1) * 40;
    }
    m_Panel = new Panel(false, QSize(width, scrollHeigth), QSize(width, itemcount * 40));
    m_Panel->setSubComponent(true);
    m_Panel->setVisible(false);
    this->addChild(m_Panel);
    m_pArrowDown = new oxygine::Button();
    m_Box->addChild(m_pArrowDown);
    m_pArrowDown->setPosition(m_Box->getWidth() - 45, 10);
    m_pArrowDown->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+down"));
    m_pArrowDown->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        m_pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        m_pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    m_Box->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        if (m_Panel->getVisible())
        {
            emit sigHideDropDown();
        }
        else
        {
            emit sigFocused();
            emit sigShowDropDown();
        }
    });
    connect(this, &DropDownmenuBase::sigItemChangedInternal, this, &DropDownmenuBase::itemChanged, Qt::QueuedConnection);
    connect(this, &DropDownmenuBase::sigShowDropDown, this, &DropDownmenuBase::showDropDown, Qt::QueuedConnection);
    connect(this, &DropDownmenuBase::sigHideDropDown, this, &DropDownmenuBase::hideDropDown, Qt::QueuedConnection);
}

void DropDownmenuBase::changeItemCount(qint32 itemcount)
{
    m_Panel->setContentHeigth(itemcount * 40);
}

void DropDownmenuBase::focusedLost()
{
    emit sigHideDropDown();
}

void DropDownmenuBase::showDropDown()
{
    
    setPriority(static_cast<qint32>(Mainapp::ZOrder::DropDownList));
    m_Panel->setVisible(true);
    m_OriginalOwner = getParent();
    m_OriginalPosition = getPosition();
    auto transform = computeGlobalTransform();
    setPosition(transform.x, transform.y);
    oxygine::spActor pMe = this;
    oxygine::getStage()->addChild(pMe);
    if (getY() > Settings::getHeight() / 2)
    {
        if (m_Panel->getH_Scrollbar()->getVisible())
        {
            m_Panel->setY(-m_Panel->getHeight());
        }
        else
        {
            m_Panel->setY(-m_Panel->getHeight() + m_Panel->getV_Scrollbar()->getHeight() + 7);
        }
    }
    else
    {
        m_Panel->setY(m_Box->getHeight());
    }
    
}

void DropDownmenuBase::hideDropDown()
{
    
    if (m_OriginalOwner.get() != nullptr &&
        m_OriginalOwner->getParent() == nullptr)
    {
        m_Panel->setVisible(false);
        hideTooltip();
        stopTooltiptimer();
        m_OriginalOwner = nullptr;
    }
    else if (m_Panel->getVisible())
    {
        m_Panel->setVisible(false);
        hideTooltip();
        stopTooltiptimer();
        oxygine::spActor pMe = this;
        m_OriginalOwner->addChild(pMe);
        setPosition(m_OriginalPosition);
        m_OriginalOwner = nullptr;
    }
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    
}

void DropDownmenuBase::setEnabled(bool value)
{
    
    oxygine::Actor::setEnabled(value);
    m_pArrowDown->setEnabled(value);
    m_Box->setEnabled(value);
    m_Panel->setVisible(false);
    
}

qint32 DropDownmenuBase::getCurrentItem() const
{
    return m_currentItem;
}

const oxygine::Vector2& DropDownmenuBase::addDropDownItem(oxygine::spActor item, qint32 id)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("topbar+dropdown");
    oxygine::spBox9Sprite pBox = new oxygine::Box9Sprite();
    pBox->setResAnim(pAnim);
    pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->addChild(item);
    pBox->setSize(getWidth() - 33, 40);
    pBox->setPosition(0, 40 * m_Items.size());

    m_Items.append(pBox);
    pBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
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
        m_currentItem = id;
        pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(1));
        emit sigItemChangedInternal(m_currentItem);
        emit sigHideDropDown();
    });
    return pBox->getSize();
}
