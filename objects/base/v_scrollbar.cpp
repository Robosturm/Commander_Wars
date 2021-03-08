#include "objects/base/v_scrollbar.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"

#include "coreengine/console.h"

V_Scrollbar::V_Scrollbar(qint32 width, qint32 contentWidth)
    : m_Width(width),
      m_ContentWidth(contentWidth)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    m_ScrollTimer.start();
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setSize(width, 33);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("scrollbar");
    m_pBox = new oxygine::Box9Sprite();
    m_pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_pBox->setResAnim(pAnim);
    m_pBox->setSize(width, 33);
    this->addChild(m_pBox);

    m_pArrowRigth = new oxygine::Button();
    m_pArrowRigth->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+right"));
    m_pArrowRigth->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_pArrowRigth->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        if (m_enabled)
        {
            m_pArrowRigth->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
        }
    });
    m_pArrowRigth->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        if (m_enabled)
        {
            m_pArrowRigth->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
        }
    });
    m_pArrowRigth->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [ = ](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            pEvent->stopPropagation();
            m_scroll = 1;
            m_currentScrollspeed = m_Scrollspeed;
            speedCounter = 0;
            emit sigStartEditValue();
        }
    });
    m_pArrowRigth->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            pEvent->stopPropagation();
            m_scroll = 0;
            emit sigEndEditValue(m_Scrollvalue);
        }
    });
    m_pBox->addChild(m_pArrowRigth);
    m_pArrowRigth->setPosition(width - m_pArrowRigth->getWidth() - 8, 9);

    m_pArrowLeft = new oxygine::Button();
    // pButton->setPosition(200, 200);
    m_pArrowLeft->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+right"));
    m_pArrowLeft->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_pArrowLeft->setFlippedX(true);
    m_pArrowLeft->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        if (m_enabled)
        {
            m_pArrowLeft->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
        }
    });
    m_pArrowLeft->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        if (m_enabled)
        {
            m_pArrowLeft->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
        }
    });
    m_pArrowLeft->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [ = ](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            pEvent->stopPropagation();
            m_scroll = -1;
            m_currentScrollspeed = m_Scrollspeed;
            speedCounter = 0;
            emit sigStartEditValue();
        }
    });
    m_pArrowLeft->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            pEvent->stopPropagation();
            m_scroll = 0;
            emit sigEndEditValue(m_Scrollvalue);
        }
    });
    m_pBox->addChild(m_pArrowLeft);
    m_pArrowLeft->setPosition(9, 8);

    m_slider = new oxygine::Box9Sprite();
    pAnim = pObjectManager->getResAnim("v_scrollbar");
    m_slider->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_slider->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_slider->setResAnim(pAnim);

    qint32 sliderWidth = 50;
    sliderWidth = ((width - m_slider->getWidth() - 20 - 20) * width) / contentWidth;
    if (sliderWidth < 11)
    {
        sliderWidth = 11;
    }
    else if (sliderWidth > (width - m_slider->getWidth() - 20 - 20))
    {
        sliderWidth = (width - m_slider->getWidth() - 20 - 20);
    }

    m_slider->setSize(sliderWidth, 18);
    m_slider->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_slider->setPosition(20, 9);
    m_pBox->addChild(m_slider);
    m_slider->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        if (m_enabled)
        {
            oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("h_scrollbar_active");
            m_slider->setResAnim(pAnimState);
            m_slider->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
        }
    });
    addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        if (m_enabled)
        {
            setSliding(getSliding());
        }
    });
    m_slider->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [ = ](oxygine::Event* event)
    {
        if (m_enabled)
        {
            event->stopPropagation();
            setSliding(true);
            emit sigStartEditValue();
            emit sigFocused();
        }
    });
    m_slider->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event* event)
    {
        if (m_enabled)
        {
            event->stopPropagation();
            if (getSliding())
            {
                setSliding(false);
                emit sigEndEditValue(m_Scrollvalue);
            }
            emit sigFocusedLost();
        }
    });
    m_pBox->addEventListener(oxygine::TouchEvent::MOVE, [ = ](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            scroll(pEvent);
        }
    });

    connect(this, &V_Scrollbar::sigChangeScrollValue, this, &V_Scrollbar::changeScrollValue, Qt::QueuedConnection);
}

void V_Scrollbar::focusedLost()
{
    bool emitSignal = getSliding();
    setSliding(false);
    if (emitSignal)
    {
        emit sigEndEditValue(m_Scrollvalue);
    }
}

void V_Scrollbar::scroll(oxygine::Event* pEvent)
{
    if (m_sliding)
    {
        pEvent->stopPropagation();
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            qint32 x = pTouchEvent->localPosition.x - m_slider->getWidth() / 2;
            if (x < 20)
            {
                x = 20;
            }
            else if (x > this->getWidth() - m_slider->getWidth() - 20)
            {
                x = this->getWidth() - m_slider->getWidth() - 20;
            }
            m_slider->setX(x);
            // calc new scroll value :)
            if (static_cast<float>(this->getWidth() - m_slider->getWidth() - 20 - 20) > 0)
            {
                m_Scrollvalue = static_cast<float>(x - 20) / static_cast<float>(this->getWidth() - m_slider->getWidth() - 20 - 20);
            }
            else
            {
                m_Scrollvalue = 0;
            }
            emit sigScrollValueChanged(m_Scrollvalue);
        }
    }
}

bool V_Scrollbar::getSliding() const
{
    return m_sliding;
}

void V_Scrollbar::setSliding(bool sliding)
{
    
    m_sliding = sliding;
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    if (sliding)
    {
        oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("v_scrollbar_pressed");
        m_slider->setResAnim(pAnimState);
    }
    else
    {
        oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("v_scrollbar");
        m_slider->setResAnim(pAnimState);
    }
    
}

float V_Scrollbar::getScrollspeed() const
{
    return m_Scrollspeed;
}

void V_Scrollbar::setScrollspeed(float Scrollspeed)
{
    m_Scrollspeed = Scrollspeed;
}

void V_Scrollbar::setContentWidth(qint32 width)
{
    
    m_ContentWidth = width;
    qint32 sliderWidth = 50;
    sliderWidth = ((this->getWidth() - 20 - 20) * this->getWidth()) / m_ContentWidth;
    if (sliderWidth < 11)
    {
        sliderWidth = 11;
    }
    else if (sliderWidth > (this->getWidth() - 20 - 20))
    {
        sliderWidth = (this->getWidth() - 20 - 20);
    }
    m_Scrollvalue = 0;
    m_slider->setSize(sliderWidth, 18);
    setScrollvalue(m_Scrollvalue);
    
}

void V_Scrollbar::update(const oxygine::UpdateState& us)
{
    // no need to calculate more than we need if we're invisible
    if(m_scroll != 0)
    {
        if (m_ScrollTimer.elapsed() > 250)
        {
            if (m_ContentWidth > m_Width)
            {
                speedCounter++;
                if (speedCounter % 8 == 0)
                {
                    m_currentScrollspeed *= 2;
                }
                emit sigChangeScrollValue(m_scroll * m_currentScrollspeed * 10.0f / static_cast<float>(m_ContentWidth));
                m_ScrollTimer.start();
            }
        }
    }
    oxygine::Actor::update(us);
}

void V_Scrollbar::changeScrollValue(float value)
{
    
    m_Scrollvalue += value;
    if (m_Scrollvalue < 0)
    {
        m_Scrollvalue = 0;
    }
    else if (m_Scrollvalue > 1.0f)
    {
        m_Scrollvalue = 1.0f;
    }
    else
    {
        // all fine do nothing
    }
    setScrollvalue(m_Scrollvalue);
    emit sigScrollValueChanged(m_Scrollvalue);
    
}

float V_Scrollbar::getScrollvalue() const
{
    return m_Scrollvalue;
}

void V_Scrollbar::setScrollvalue(float Scrollvalue)
{
    
    m_Scrollvalue = Scrollvalue;
    if (m_Scrollvalue < 0)
    {
        m_Scrollvalue = 0;
    }
    else if (m_Scrollvalue > 1.0f)
    {
        m_Scrollvalue = 1.0f;
    }
    else
    {
        // all fine do nothing
    }
    m_slider->setX(20 + m_Scrollvalue * (m_Width - m_slider->getWidth() - 20 - 20));
    
}

void V_Scrollbar::setWidth(float w)
{    
    oxygine::Actor::setWidth(w);
    m_Width = w;
    m_pBox->setWidth(w);
    m_pArrowRigth->setPosition(m_Width - m_pArrowRigth->getWidth() - 8, 9);
    setContentWidth(m_ContentWidth);    
}

void V_Scrollbar::setEnabled(bool value)
{
    oxygine::Actor::setEnabled(value);
    m_slider->setEnabled(value);
    m_pBox->setEnabled(value);
    m_pArrowRigth->setEnabled(value);
    m_pArrowLeft->setEnabled(value);
}
