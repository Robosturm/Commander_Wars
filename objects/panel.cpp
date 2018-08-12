#include "panel.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"

Panel::Panel(bool useBox, QSize size, QSize contentSize)
{
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_Scrollbar = new H_Scrollbar(size.height(), contentSize.height());
    this->addChild(m_Scrollbar);
    this->setSize(size.width(), size.height());
    m_Scrollbar->setX(size.width() - m_Scrollbar->getWidth());
    connect(m_Scrollbar.get(), SIGNAL(sigScrollValueChanged(float)), this, SLOT(scrolled(float)), Qt::QueuedConnection);
    m_ClipRect = new oxygine::ClipRectActor();
    if (useBox)
    {
        m_Panelbox = new oxygine::Box9Sprite();
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
        m_Panelbox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
        m_Panelbox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
        m_Panelbox->setResAnim(pAnim);
        m_Panelbox->setSize(size.width() - m_Scrollbar->getWidth(), size.height());
        this->addChild(m_Panelbox);
        m_Panelbox->addChild(m_ClipRect);
        m_ClipRect->setPosition(10, 10);
        m_ClipRect->setSize(size.width() - 20, size.height() - 20);
    }
    else
    {
        this->addChild(m_ClipRect);
        m_ClipRect->setSize(size.width(), size.height());
    }
    m_ContentRect = new oxygine::Actor();
    m_ContentRect->setSize(contentSize.width(), contentSize.height());
    m_ClipRect->addChild(m_ContentRect);
}

void Panel::scrolled(float value)
{
    m_ContentRect->setY(-value * (m_ContentRect->getHeight() - m_ClipRect->getHeight()));
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
    m_Scrollbar->setContentHeigth(heigth);

}

void Panel::addItem(oxygine::spActor pActor)
{
    m_ContentRect->addChild(pActor);
}

void Panel::removeItem(oxygine::spActor pActor)
{
    m_ContentRect->removeChild(pActor);
}
