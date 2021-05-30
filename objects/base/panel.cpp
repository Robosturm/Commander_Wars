#include "panel.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"

#include "objects/base/dropdownmenubase.h"

Panel::Panel(bool useBox, QSize size, QSize contentSize)
{
    setObjectName("Panel");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    m_HScrollbar = spH_Scrollbar::create(size.height() - sliderSize, contentSize.height());
    addChild(m_HScrollbar);
    m_HScrollbar->setX(size.width() - m_HScrollbar->getWidth());
    connect(m_HScrollbar.get(), &H_Scrollbar::sigScrollValueChanged, this, &Panel::scrolledY, Qt::QueuedConnection);

    m_VScrollbar = spV_Scrollbar::create(size.width() - sliderSize, contentSize.width());
    addChild(m_VScrollbar);
    m_VScrollbar->setY(size.height() - m_VScrollbar->getHeight());
    connect(m_VScrollbar.get(), &V_Scrollbar::sigScrollValueChanged, this, &Panel::scrolledX, Qt::QueuedConnection);

    m_ClipRect = oxygine::spClipRectActor::create();
    if (useBox)
    {
        m_Panelbox = oxygine::spBox9Sprite::create();
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
        m_Panelbox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
        m_Panelbox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
        m_Panelbox->setResAnim(pAnim);
        m_Panelbox->setSize(size.width() - m_HScrollbar->getWidth(),
                            size.height() - m_VScrollbar->getHeight());
        addChild(m_Panelbox);
        m_Panelbox->addChild(m_ClipRect);
        m_ClipRect->setPosition(10, 10);
        m_ClipRect->setSize(m_Panelbox->getWidth() - 20, m_Panelbox->getHeight() - 20);
    }
    else
    {
        addChild(m_ClipRect);
        m_ClipRect->setSize(size.width() - m_HScrollbar->getWidth(),
                            size.height() - m_VScrollbar->getHeight());
    }
    m_ContentRect = oxygine::spActor::create();
    m_SlidingActor = oxygine::spSlidingActor::create();
    m_SlidingActor->setSize(contentSize.width(), contentSize.height());
    m_ContentRect->setSize(contentSize.width(), contentSize.height());
    m_SlidingActor->setContent(m_ContentRect);
    m_ClipRect->addChild(m_SlidingActor);

    setSize(size.width(), size.height());
    setContentWidth(contentSize.width());
    setContentHeigth(contentSize.height());

    addEventListener(oxygine::TouchEvent::WHEEL_DIR, [ = ](oxygine::Event* pEvent)
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
           emit m_HScrollbar->sigChangeScrollValue(-pTouchEvent->wheelDirection.y / getContentHeigth());
           emit m_VScrollbar->sigChangeScrollValue(-pTouchEvent->wheelDirection.x / getContentWidth());
           pTouchEvent->stopPropagation();
        }
    });
}

void Panel::scrolledY(float value)
{
    
    if (m_HScrollbar->getVisible() && getVisible())
    {
        oxygine::RectF bounds = m_SlidingActor->getDragBounds();
        qint32 newY = -(m_ContentRect->getHeight() - m_SlidingActor->getHeight()) * value;
        if (newY < bounds.getTop())
        {
            newY = bounds.getTop();
        }
        else if (newY > bounds.getBottom())
        {
            newY = bounds.getBottom();
        }
        m_ContentRect->setY(newY);
    }
    hideItems();
}

void Panel::scrolledX(float value)
{
    if (m_VScrollbar->getVisible() && getVisible())
    {
        oxygine::RectF bounds = m_SlidingActor->getDragBounds();
        qint32 newX = -(m_ContentRect->getWidth() - m_SlidingActor->getWidth()) * value;
        if (newX < bounds.getLeft())
        {
            newX = bounds.getLeft();
        }
        else if (newX > bounds.getRight())
        {
            newX = bounds.getRight();
        }
        m_ContentRect->setX(newX);
    }
    hideItems();
}

void Panel::doUpdate(const oxygine::UpdateState& us)
{
    if (m_VScrollbar->getVisible())
    {
        float value = m_ContentRect->getX() / -(m_ContentRect->getWidth() - m_SlidingActor->getWidth());
        m_VScrollbar->setScrollvalue(value);
    }
    else
    {
        m_ContentRect->setX(0);
    }
    if (m_HScrollbar->getVisible())
    {
        float value = m_ContentRect->getY() / -(m_ContentRect->getHeight() - m_SlidingActor->getHeight());
        m_HScrollbar->setScrollvalue(value);
    }
    else
    {
        m_ContentRect->setY(0);
    }
}

bool Panel::getSubComponent() const
{
    return m_HScrollbar->getSubComponent();
}
void Panel::setSubComponent(bool subComponent)
{
    m_HScrollbar->setSubComponent(subComponent);
    m_VScrollbar->setSubComponent(subComponent);
}

void Panel::hideItems()
{    
    oxygine::spActor child =  m_ContentRect->getFirstChild();
    while (child)
    {
        Tooltip* pTooltip = dynamic_cast<Tooltip*>(child.get());
        DropDownmenuBase* pDropDownmenuBase = dynamic_cast<DropDownmenuBase*>(child.get());
        if (pTooltip != nullptr)
        {
            pTooltip->hideTooltip();
        }
        if (pDropDownmenuBase != nullptr)
        {
            pDropDownmenuBase->hideDropDown();
        }
        hideItems(child);
        child = child->getNextSibling();
    }
    
}

void Panel::hideItems(oxygine::spActor parent)
{
    
    oxygine::spActor child =  parent->getFirstChild();
    while (child)
    {
        Tooltip* pTooltip = dynamic_cast<Tooltip*>(child.get());
        DropDownmenuBase* pDropDownmenuBase = dynamic_cast<DropDownmenuBase*>(child.get());
        if (pTooltip != nullptr)
        {
            pTooltip->hideTooltip();
        }
        if (pDropDownmenuBase != nullptr)
        {
            pDropDownmenuBase->hideDropDown();
        }
        hideItems(child);
        child = child->getNextSibling();
    }
    
}

void Panel::setContentHeigth(qint32 heigth)
{    
    // content can't be smaller than our own size
    // avoid complicate handling of smaller content
    if (heigth <= getHeight())
    {
        heigth = getHeight();
        if (m_HScrollbar->getVisible())
        {
            if (m_Panelbox.get() != nullptr)
            {
                m_Panelbox->setWidth(m_Panelbox->getWidth() + m_HScrollbar->getWidth());
            }
            m_ClipRect->setWidth(m_ClipRect->getWidth() + m_HScrollbar->getWidth());
            m_HScrollbar->setVisible(false);
            m_VScrollbar->setWidth(m_VScrollbar->getWidth() + m_HScrollbar->getWidth());
        }
    }
    else if (!m_HScrollbar->getVisible())
    {
        if (m_Panelbox.get() != nullptr)
        {
            m_Panelbox->setWidth(m_Panelbox->getWidth() - m_HScrollbar->getWidth());
        }
        m_ClipRect->setWidth(m_ClipRect->getWidth() - m_HScrollbar->getWidth());

        m_HScrollbar->setVisible(true);
        m_VScrollbar->setWidth(m_VScrollbar->getWidth() - m_HScrollbar->getWidth());
    }
    m_SlidingActor->setSize(m_ClipRect->getSize());
    m_ContentRect->setHeight(heigth);
    m_SlidingActor->setContent(m_ContentRect);
    m_HScrollbar->setContentHeigth(heigth);
    m_HScrollbar->setScrollvalue(0);    
}

void Panel::setContentWidth(qint32 width)
{    
    // content can't be smaller than our own size
    // avoid complicate handling of smaller content
    if (width <= getWidth())
    {
        width = getWidth();
        if (m_VScrollbar->getVisible())
        {
            if (m_Panelbox.get() != nullptr)
            {
                m_Panelbox->setHeight(m_Panelbox->getHeight() + m_VScrollbar->getHeight());
            }
            m_ClipRect->setHeight(m_ClipRect->getHeight() + m_VScrollbar->getHeight());
            m_VScrollbar->setVisible(false);
            m_HScrollbar->setHeight(m_HScrollbar->getHeight() + m_VScrollbar->getHeight());
        }
    }
    else if (!m_VScrollbar->getVisible())
    {
        if (m_Panelbox.get() != nullptr)
        {
            m_Panelbox->setHeight(m_Panelbox->getHeight() - m_VScrollbar->getHeight());
        }
        m_ClipRect->setHeight(m_ClipRect->getHeight() - m_VScrollbar->getHeight());
        m_VScrollbar->setVisible(true);
        m_HScrollbar->setHeight(m_HScrollbar->getHeight() - m_VScrollbar->getHeight());
    }
    m_SlidingActor->setSize(m_ClipRect->getSize());
    m_ContentRect->setWidth(width);
    m_SlidingActor->setContent(m_ContentRect);
    m_VScrollbar->setContentWidth(width);
    m_VScrollbar->setScrollvalue(0);    
}

qint32 Panel::getContentHeigth()
{
    return m_ContentRect->getHeight();
}

qint32 Panel::getContentWidth()
{
   return m_ContentRect->getWidth();
}

void Panel::addItem(oxygine::spActor pActor)
{
    m_ContentRect->addChild(pActor);
}

void Panel::removeItem(oxygine::spActor pActor)
{
    pActor->detach();
}

void Panel::clearContent()
{
    
    m_ContentRect->removeChildren();
    m_ContentRect->setX(0.0f);
    m_ContentRect->setY(0.0f);
    
}
