#include "objects/base/moveinbutton.h"

#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"

MoveInButton::MoveInButton(oxygine::Actor* pParent, qint32 moveInSize, qint32 direction, qint32 startY)
    : m_pParent(pParent),
      m_moveInSize(moveInSize),
      m_direction(direction)
{
    oxygine::spButton pButton = oxygine::spButton::create();
    pButton->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+right"));
    pButton->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    oxygine::Sprite* ptr = pButton.get();
    pButton->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    pButton->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        addMoveTween();
    });
    pButton->setFlippedX(true);
    pButton->setScale(2.0f);
    addChild(pButton);
    setX(m_direction * (pButton->getScaledWidth() + 5));
    if (startY < 0)
    {
        setY(pParent->getHeight() / 2 - pButton->getHeight() / 2);
    }
    pParent->addChild(this);
}

void MoveInButton::addMoveTween()
{
    if (m_finished)
    {
        m_finished = false;
        qint32 endPos = m_pParent->getX();
        if (m_movedOut)
        {
            endPos -= m_moveInSize * m_direction;
        }
        else
        {
            endPos += m_moveInSize * m_direction;
        }
        oxygine::spTween pTween = oxygine::createTween(oxygine::Actor::TweenX(endPos), oxygine::timeMS(200));
        pTween->addDoneCallback([=](oxygine::Event*)
        {
            m_movedOut = !m_movedOut;
            m_finished = true;
            emit sigMoved();
        });
        m_pParent->addTween(pTween);        
    }
}

bool MoveInButton::getMovedOut() const
{
    return m_movedOut;
}
