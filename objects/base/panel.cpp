#include "objects/base/panel.h"
#include "objects/base/dropdownmenubase.h"

#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"

Panel::Panel(bool useBox, QSize size, QSize contentSize, QString resAnim)
    : m_hideTimer(this)
{
#ifdef GRAPHICSUPPORT
    setObjectName("Panel");
#endif
    Interpreter::setCppOwnerShip(this);

    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_HScrollbar = spH_Scrollbar::create(size.height() - sliderSize, contentSize.height());
    addChild(m_HScrollbar);
    m_HScrollbar->setX(size.width() - m_HScrollbar->getWidth());
    connect(m_HScrollbar.get(), &H_Scrollbar::sigScrollValueChanged, this, &Panel::scrolledY, Qt::QueuedConnection);

    m_VScrollbar = spV_Scrollbar::create(size.width() - sliderSize, contentSize.width());
    addChild(m_VScrollbar);
    m_VScrollbar->setY(size.height() - m_VScrollbar->getScaledHeight());
    connect(m_VScrollbar.get(), &V_Scrollbar::sigScrollValueChanged, this, &Panel::scrolledX, Qt::QueuedConnection);

    m_ClipRect = oxygine::spClipRectActor::create();
    if (useBox)
    {
        m_Panelbox = oxygine::spBox9Sprite::create();
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim(resAnim);
        m_Panelbox->setResAnim(pAnim);
        m_Panelbox->setSize(size.width() - m_HScrollbar->getScaledWidth(),
                            size.height() - m_VScrollbar->getScaledHeight());
        addChild(m_Panelbox);
        m_Panelbox->addChild(m_ClipRect);
        m_ClipRect->setPosition(10, 10);
        m_ClipRect->setSize(m_Panelbox->getScaledWidth() - 20,
                            m_Panelbox->getScaledHeight() - 20);
    }
    else
    {
        addChild(m_ClipRect);
        m_ClipRect->setSize(size.width() - m_HScrollbar->getScaledWidth(),
                            size.height() - m_VScrollbar->getScaledHeight());
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

    addEventListener(oxygine::TouchEvent::WHEEL_DIR, [this](oxygine::Event* pEvent)
    {
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
        auto width = getContentWidth();
        auto height = getContentHeigth();
        if (pTouchEvent != nullptr &&
            width > 0 &&
            height > 0)
        {
            emit m_HScrollbar->sigChangeScrollValue(static_cast<float>(-pTouchEvent->wheelDirection.y()) / static_cast<float>(height));
            emit m_VScrollbar->sigChangeScrollValue(static_cast<float>(-pTouchEvent->wheelDirection.x()) / static_cast<float>(width));
            if (m_stopMouseWheel)
            {
                pTouchEvent->stopPropagation();
            }
        }
    });
    m_hideTimer.setSingleShot(true);
    connect(&m_hideTimer, &QTimer::timeout, this, &Panel::hideItems);
}

Panel::~Panel()
{
    clearContent();
}

void Panel::scrolledY(float value)
{    
    if (m_HScrollbar->getVisible() && getVisible())
    {
        auto bounds = m_SlidingActor->getDragBounds();
        qint32 newY = -(m_ContentRect->getScaledHeight() - m_SlidingActor->getScaledHeight()) * value;
        if (newY < bounds.top())
        {
            newY = bounds.top();
        }
        else if (newY > bounds.bottom())
        {
            newY = bounds.bottom();
        }
        m_ContentRect->setY(newY);
    }
    if (!m_hideTimer.isActive())
    {
        m_hideTimer.start(250);
    }
}

void Panel::scrolledX(float value)
{
    if (m_VScrollbar->getVisible() && getVisible())
    {
        auto bounds = m_SlidingActor->getDragBounds();
        qint32 newX = -(m_ContentRect->getScaledWidth() - m_SlidingActor->getScaledWidth()) * value;
        if (newX < bounds.left())
        {
            newX = bounds.left();
        }
        else if (newX > bounds.right())
        {
            newX = bounds.right();
        }
        m_ContentRect->setX(newX);
    }
    if (!m_hideTimer.isActive())
    {
        m_hideTimer.start(250);
    }
}

void Panel::doUpdate(const oxygine::UpdateState& us)
{
    auto div = m_ContentRect->getScaledWidth() - m_SlidingActor->getScaledWidth();
    if (m_VScrollbar->getVisible())
    {
        float value = static_cast<float>(m_ContentRect->getX()) / -static_cast<float>(div);
        m_VScrollbar->setScrollvalue(value);
    }
    else
    {
        m_ContentRect->setX(0);
    }
    div = m_ContentRect->getScaledHeight() - m_SlidingActor->getScaledHeight();
    if (m_HScrollbar->getVisible() && div > 0)
    {
        float value = static_cast<float>(m_ContentRect->getY()) / -static_cast<float>(div);
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
    auto & children = m_ContentRect->getChildren();
    for (auto & child : children)
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
        hideChildItems(child);
    }
}

void Panel::hideChildItems(oxygine::spActor parent)
{
    auto & children = parent->getChildren();
    for (auto & child : children)
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
        hideChildItems(child);
    }
}

bool Panel::getStopMouseWheel() const
{
    return m_stopMouseWheel;
}

void Panel::setStopMouseWheel(bool newStopMouseWheel)
{
    m_stopMouseWheel = newStopMouseWheel;
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
                m_Panelbox->setWidth(m_Panelbox->getScaledWidth() + m_HScrollbar->getScaledWidth());
            }
            m_ClipRect->setWidth(m_ClipRect->getScaledWidth() + m_HScrollbar->getScaledWidth());
            m_HScrollbar->setVisible(false);
            m_VScrollbar->setWidth(m_VScrollbar->getScaledWidth() + m_HScrollbar->getScaledWidth());
        }
    }
    else if (!m_HScrollbar->getVisible())
    {
        if (m_Panelbox.get() != nullptr)
        {
            m_Panelbox->setWidth(m_Panelbox->getScaledWidth() - m_HScrollbar->getScaledWidth());
        }
        m_ClipRect->setWidth(m_ClipRect->getScaledWidth() - m_HScrollbar->getScaledWidth());

        m_HScrollbar->setVisible(true);
        m_VScrollbar->setWidth(m_VScrollbar->getScaledWidth() - m_HScrollbar->getScaledWidth());
    }
    m_SlidingActor->setSize(m_ClipRect->getSize());
    m_ContentRect->setHeight(heigth);
    m_SlidingActor->updateDragBounds();
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
                m_Panelbox->setHeight(m_Panelbox->getScaledHeight() + m_VScrollbar->getScaledHeight());
            }
            m_ClipRect->setHeight(m_ClipRect->getScaledHeight() + m_VScrollbar->getScaledHeight());
            m_VScrollbar->setVisible(false);
            m_HScrollbar->setHeight(m_HScrollbar->getScaledHeight() + m_VScrollbar->getScaledHeight());
        }
    }
    else if (!m_VScrollbar->getVisible())
    {
        if (m_Panelbox.get() != nullptr)
        {
            m_Panelbox->setHeight(m_Panelbox->getScaledHeight() - m_VScrollbar->getScaledHeight());
        }
        m_ClipRect->setHeight(m_ClipRect->getScaledHeight() - m_VScrollbar->getScaledHeight());
        m_VScrollbar->setVisible(true);
        m_HScrollbar->setHeight(m_HScrollbar->getScaledHeight() - m_VScrollbar->getScaledHeight());
    }
    m_SlidingActor->setSize(m_ClipRect->getSize());
    m_ContentRect->setWidth(width);
    m_SlidingActor->updateDragBounds();
    m_VScrollbar->setContentWidth(width);
    m_VScrollbar->setScrollvalue(0);    
}

qint32 Panel::getContentHeigth() const
{
    return m_ContentRect->getScaledHeight();
}

qint32 Panel::getContentWidth() const
{
   return m_ContentRect->getScaledWidth();
}

void Panel::addItem(oxygine::spActor pActor)
{
    m_ContentRect->addChild(pActor);
}

qint32 Panel::getContentX() const
{
    return m_ContentRect->getX();
}

qint32 Panel::getContentY() const
{
    return m_ContentRect->getY();
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
