#include "panel.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"

#include "objects/dropdownmenubase.h"

Panel::Panel(bool useBox, QSize size, QSize contentSize)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    m_HScrollbar = new H_Scrollbar(size.height() - sliderSize, contentSize.height());
    this->addChild(m_HScrollbar);
    m_HScrollbar->setX(size.width() - m_HScrollbar->getWidth());
    connect(m_HScrollbar.get(), &H_Scrollbar::sigScrollValueChanged, this, &Panel::scrolledY, Qt::QueuedConnection);

    m_VScrollbar = new V_Scrollbar(size.width() - sliderSize, contentSize.width());
    this->addChild(m_VScrollbar);
    m_VScrollbar->setY(size.height() - m_VScrollbar->getHeight());
    connect(m_VScrollbar.get(), &V_Scrollbar::sigScrollValueChanged, this, &Panel::scrolledX, Qt::QueuedConnection);

    m_ClipRect = new oxygine::ClipRectActor();
    if (useBox)
    {
        m_Panelbox = new oxygine::Box9Sprite();
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
        m_Panelbox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
        m_Panelbox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
        m_Panelbox->setResAnim(pAnim);
        m_Panelbox->setSize(size.width() - m_HScrollbar->getWidth(),
                            size.height() - m_VScrollbar->getHeight());
        this->addChild(m_Panelbox);
        m_Panelbox->addChild(m_ClipRect);
        m_ClipRect->setPosition(10, 10);
        m_ClipRect->setSize(m_Panelbox->getWidth() - 20, m_Panelbox->getHeight() - 20);
    }
    else
    {
        this->addChild(m_ClipRect);
        m_ClipRect->setSize(size.width() - m_HScrollbar->getWidth(),
                            size.height() - m_VScrollbar->getHeight());
    }
    m_ContentRect = new oxygine::Actor();
    m_ContentRect->setSize(contentSize.width(), contentSize.height());
    m_ClipRect->addChild(m_ContentRect);

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
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (m_HScrollbar->getVisible())
    {
        m_ContentRect->setY(-value * (m_ContentRect->getHeight() - m_ClipRect->getHeight()));
    }

    hideItems();

    pApp->continueThread();
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

void Panel::scrolledX(float value)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (m_VScrollbar->getVisible())
    {
        m_ContentRect->setX(-value * (m_ContentRect->getWidth() - m_ClipRect->getWidth()));
    }
    hideItems();
    pApp->continueThread();
}

void Panel::setContentHeigth(qint32 heigth)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    // content can't be smaller than our own size
    // avoid complicate handling of smaller content
    if (heigth <= this->getHeight())
    {
        heigth = this->getHeight();
        if (m_HScrollbar->getVisible())
        {
            m_HScrollbar->setVisible(false);
            if (m_Panelbox.get() != nullptr)
            {
                m_Panelbox->setWidth(m_Panelbox->getWidth() + m_HScrollbar->getWidth());
            }
            m_ClipRect->setWidth(m_ClipRect->getWidth() + m_HScrollbar->getWidth());
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
    }

    m_ContentRect->setHeight(heigth);
    m_HScrollbar->setContentHeigth(heigth);
    pApp->continueThread();
}

void Panel::setContentWidth(qint32 width)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    // content can't be smaller than our own size
    // avoid complicate handling of smaller content
    if (width <= this->getWidth())
    {
        width = this->getWidth();
        if (m_VScrollbar->getVisible())
        {
            m_VScrollbar->setVisible(false);
            if (m_Panelbox.get() != nullptr)
            {
                m_Panelbox->setHeight(m_Panelbox->getHeight() + m_VScrollbar->getHeight());
            }
            m_ClipRect->setHeight(m_ClipRect->getHeight() + m_VScrollbar->getHeight());
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
    }
    m_ContentRect->setWidth(width);

    m_VScrollbar->setContentWidth(width);
    pApp->continueThread();
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
}
