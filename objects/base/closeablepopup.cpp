#include "closeablepopup.h"

#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"

CloseablePopUp::CloseablePopUp(qint32 width, qint32 heigth)
    : QObject()
{
    setObjectName("CloseablePopUp");
    setSize(width, heigth);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");

    oxygine::spBox9Sprite pBox = oxygine::spBox9Sprite::create();
    pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setResAnim(pAnim);
    pBox->setSize(width, heigth);

    oxygine::spSprite pSprite = oxygine::spSprite::create();
    pAnim = pObjectManager->getResAnim("checkbox");
    pSprite->setResAnim(pAnim);
    pSprite->setScale(0.5f);
    setPriority(static_cast<qint32>(Mainapp::ZOrder::FocusedObjects));
    pSprite->setPosition(width - pSprite->getScaledWidth(), 0);
    pSprite->setAnimFrame(pAnim, 3);
    pBox->addChild(pSprite);
    pSprite->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigClosed();
    });
    pSprite->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        pSprite->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    pSprite->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        pSprite->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    addChild(pBox);
    connect(this, &CloseablePopUp::sigClosed, this, &CloseablePopUp::close, Qt::QueuedConnection);

    m_drag.init(this);
    m_drag.setDragEnabled(true);
    m_drag.setDragBounds(oxygine::RectF(0, 0, Settings::getWidth() - width, Settings::getHeight() - heigth));
    QSize size(width - 10, heigth - 4 - pSprite->getScaledHeight());
    m_pPanel = spPanel::create(true, size, size);
    m_pPanel->setPosition(5, pSprite->getScaledHeight());
    pBox->addChild(m_pPanel);
}

void CloseablePopUp::setLocked(bool locked)
{
    m_drag.setDragEnabled(!locked);
}

void CloseablePopUp::close()
{
    detach();
}

void CloseablePopUp::addItem(oxygine::spActor pActor)
{
    m_pPanel->addItem(pActor);
}

void CloseablePopUp::removeItem(oxygine::spActor pActor)
    {
        m_pPanel->removeItem(pActor);
    }

void CloseablePopUp::setContentHeigth(qint32 heigth)
{
    m_pPanel->setContentHeigth(heigth);
}

void CloseablePopUp::setContentWidth(qint32 width)
{
    m_pPanel->setContentWidth(width);
}

qint32 CloseablePopUp::getContentHeigth()
{
    return m_pPanel->getContentHeigth();
}

qint32 CloseablePopUp::getContentWidth()
{
    return m_pPanel->getContentWidth();
}

void CloseablePopUp::clearContent()
{
    m_pPanel->clearContent();
}
