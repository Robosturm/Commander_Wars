#include "objects/base/moveinbutton.h"

#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

MoveInButton::MoveInButton(oxygine::Actor* pParent, qint32 moveInSize, qint32 direction, qint32 startOffset, float buttonScale, bool useY)
    : m_pParent(pParent),
      m_moveInSize(moveInSize),
      m_direction(direction),
      m_useY(useY)
{
    Interpreter::setCppOwnerShip(this);

    m_pButton = MemoryManagement::create<oxygine::Button>();
    if (m_useY)
    {
        m_pButton->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+down"));
    }
    else
    {
        m_pButton->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+right"));
    }
    auto* pAnim = m_pButton->getResAnim();
    if (pAnim != nullptr)
    {
        m_pButton->setSize(pAnim->getSize());
    }
    m_pButton->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    oxygine::Sprite* ptr = m_pButton.get();
    m_pButton->addEventListener(oxygine::TouchEvent::OVER, [ptr](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    m_pButton->addEventListener(oxygine::TouchEvent::OUTX, [ptr](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    m_pButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        addMoveTween();
    });
    m_pButton->setScale(buttonScale);
    if (m_useY)
    {
        if (direction < 0)
        {
            m_pButton->setFlippedY(true);
            setY(-m_pButton->getScaledHeight() - 5);
        }
        else
        {
            setY(pParent->getScaledHeight() + 5);
        }
    }
    else
    {
        if (direction < 0)
        {
            m_pButton->setFlippedX(true);
            setX(-m_pButton->getScaledWidth() - 5);
        }
        else
        {
            setX(pParent->getScaledWidth() + 5);
        }
    }
    addChild(m_pButton);

    if (startOffset < 0)
    {
        if (m_useY)
        {
            setX(pParent->getScaledWidth() / 2 - m_pButton->getScaledWidth() / 2);
        }
        else
        {
            setY(pParent->getScaledHeight() / 2 - m_pButton->getScaledHeight() / 2);
        }
    }
    else
    {
        if (m_useY)
        {
            setX(startOffset);
        }
        else
        {
            setY(startOffset);
        }
    }
}

void MoveInButton::addMoveTween()
{
    if (m_finished)
    {
        m_finished = false;
        qint32 endPos = m_pParent->getX();
        if (m_useY)
        {
            endPos = m_pParent->getY();
        }
        if (m_movedOut)
        {
            endPos -= m_moveInSize * m_direction;
        }
        else
        {
            endPos += m_moveInSize * m_direction;
        }
        oxygine::spTween pTween;
        if (m_useY)
        {
            pTween = oxygine::createTween(oxygine::Actor::TweenY(endPos), oxygine::timeMS(200));
        }
        else
        {
            pTween = oxygine::createTween(oxygine::Actor::TweenX(endPos), oxygine::timeMS(200));
        }
        oxygine::Sprite* pActor = m_pButton.get();
        pTween->addDoneCallback([this, pActor](oxygine::Event*)
        {
            m_movedOut = !m_movedOut;
            if (m_useY)
            {
                if (m_direction < 0)
                {
                    pActor->setFlippedY(!m_movedOut);
                }
                else
                {
                    pActor->setFlippedY(m_movedOut);
                }
            }
            else
            {
                if (m_direction < 0)
                {
                    pActor->setFlippedX(!m_movedOut);
                }
                else
                {
                    pActor->setFlippedX(m_movedOut);
                }
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
