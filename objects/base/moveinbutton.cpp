#include "objects/base/moveinbutton.h"

#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"

MoveInButton::MoveInButton(oxygine::Actor* pParent, qint32 moveInSize, qint32 direction, qint32 startY, float buttonScale)
    : m_pParent(pParent),
      m_moveInSize(moveInSize),
      m_direction(direction)
{
    m_pButton = oxygine::spButton::create();
    m_pButton->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+right"));
    m_pButton->setSize(m_pButton->getResAnim()->getSize());
    m_pButton->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    oxygine::Sprite* ptr = m_pButton.get();
    m_pButton->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    m_pButton->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    m_pButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
    {
        addMoveTween();
    });
    m_pButton->setScale(buttonScale);
    if (direction < 0)
    {
        m_pButton->setFlippedX(true);
        setX(-m_pButton->getScaledWidth() - 5);
    }
    else
    {
        setX(pParent->getScaledWidth() + 5);
    }
    addChild(m_pButton);
    if (startY < 0)
    {
        setY(pParent->getScaledHeight() / 2 - m_pButton->getScaledHeight() / 2);
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
        oxygine::Sprite* pActor = m_pButton.get();
        pTween->addDoneCallback([=](oxygine::Event*)
        {
            m_movedOut = !m_movedOut;
            if (m_direction < 0)
            {
                pActor->setFlippedX(!m_movedOut);
            }
            else
            {
                pActor->setFlippedX(m_movedOut);
            }
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
