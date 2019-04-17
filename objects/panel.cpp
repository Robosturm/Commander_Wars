#include "panel.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"

Panel::Panel(bool useBox, QSize size, QSize contentSize)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    this->setSize(size.width(), size.height());

    m_HScrollbar = new H_Scrollbar(size.height() - 33, contentSize.height());
    this->addChild(m_HScrollbar);
    m_HScrollbar->setX(size.width() - m_HScrollbar->getWidth());
    connect(m_HScrollbar.get(), SIGNAL(sigScrollValueChanged(float)), this, SLOT(scrolledY(float)), Qt::QueuedConnection);

    m_VScrollbar = new V_Scrollbar(size.width() - 33, contentSize.width());
    this->addChild(m_VScrollbar);
    m_VScrollbar->setY(size.height() - m_VScrollbar->getHeight());
    connect(m_VScrollbar.get(), SIGNAL(sigScrollValueChanged(float)), this, SLOT(scrolledX(float)), Qt::QueuedConnection);

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
}

void Panel::scrolledY(float value)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_ContentRect->setY(-value * (m_ContentRect->getHeight() - m_ClipRect->getHeight()));
    pApp->continueThread();
}

void Panel::scrolledX(float value)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_ContentRect->setX(-value * (m_ContentRect->getWidth() - m_ClipRect->getWidth()));
    pApp->continueThread();
}

void Panel::setContentHeigth(qint32 heigth)
{
    // content can't be smaller than our own size
    // avoid complicate handling of smaller content
    if (heigth <= this->getHeight())
    {
        heigth = this->getHeight();
    }
    m_ContentRect->setHeight(heigth);

    m_HScrollbar->setContentHeigth(heigth);
}

void Panel::setContentWidth(qint32 width)
{
    // content can't be smaller than our own size
    // avoid complicate handling of smaller content
    if (width <= this->getWidth())
    {
        width = this->getWidth();
    }
    m_ContentRect->setWidth(width);

    m_VScrollbar->setContentWidth(width);
}

void Panel::addItem(oxygine::spActor pActor)
{
    m_ContentRect->addChild(pActor);
}

void Panel::removeItem(oxygine::spActor pActor)
{
    m_ContentRect->removeChild(pActor);
}

void Panel::clearContent()
{
    m_ContentRect->removeChildren();
}
