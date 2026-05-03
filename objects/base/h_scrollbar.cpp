#include "objects/base/h_scrollbar.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "resource_management/objectmanager.h"

#include "coreengine/interpreter.h"

H_Scrollbar::H_Scrollbar(qint32 heigth, qint32 contentHeigth)
    : m_Heigth(heigth),
      m_ContentHeigth(contentHeigth)
{
#ifdef GRAPHICSUPPORT
    setObjectName("H_Scrollbar");
#endif
    Interpreter::setCppOwnerShip(this);

    m_ScrollTimer.start();
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setSize(33, heigth);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("scrollbar");
    m_pBox = MemoryManagement::create<oxygine::Box9Sprite>();
    m_pBox->setResAnim(pAnim);
    m_pBox->setSize(33, heigth);
    addChild(m_pBox);

    m_pArrowDown = MemoryManagement::create<oxygine::Button>();
    m_pArrowDown->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+down"));
    m_pArrowDown->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    oxygine::Actor* pPtrDown = m_pArrowDown.get();
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OVER, [this, pPtrDown](oxygine::Event*)
    {
        if (m_enabled)
        {
            pPtrDown->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
        }
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OUTX, [this, pPtrDown](oxygine::Event*)
    {
        if (m_enabled)
        {
            pPtrDown->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
        }
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [this](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            pEvent->stopPropagation();
            m_trackSliding = false;
            m_scroll = 1;
            m_Scrollspeed = 1.0f;
            m_speedCounter = 0;
        }
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_UP, [this](oxygine::Event* pEvent)
    {
        if (m_enabled && m_scroll > 0)
        {
            pEvent->stopPropagation();
            m_scroll = 0;
        }
    });
    m_pBox->addChild(m_pArrowDown);
    m_pArrowDown->setPosition(9, heigth - m_pArrowDown->getScaledHeight() - 8);

    m_pArrowUp = MemoryManagement::create<oxygine::Button>();
    // pButton->setPosition(200, 200);
    m_pArrowUp->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+down"));
    m_pArrowUp->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_pArrowUp->setFlippedY(true);
    oxygine::Actor* pPtrUp = m_pArrowUp.get();
    m_pArrowUp->addEventListener(oxygine::TouchEvent::OVER, [this, pPtrUp](oxygine::Event*)
    {
        if (m_enabled)
        {
            pPtrUp->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
        }
    });
    m_pArrowUp->addEventListener(oxygine::TouchEvent::OUTX, [this, pPtrUp](oxygine::Event*)
    {
        if (m_enabled)
        {
            pPtrUp->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
        }
    });
    m_pArrowUp->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [this](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            pEvent->stopPropagation();
            m_trackSliding = false;
            m_scroll = -1;
            m_Scrollspeed = 1.0f;
            m_speedCounter = 0;
        }
    });
    m_pArrowUp->addEventListener(oxygine::TouchEvent::TOUCH_UP, [this](oxygine::Event* pEvent)
    {
        if (m_enabled && m_scroll < 0)
        {
            pEvent->stopPropagation();
            m_scroll = 0;
        }
    });
    m_pBox->addChild(m_pArrowUp);
    m_pArrowUp->setPosition(9, 8);

    m_slider = MemoryManagement::create<oxygine::Box9Sprite>();
    pAnim = pObjectManager->getResAnim("h_scrollbar");
    m_slider->setResAnim(pAnim);

    qint32 sliderHeight = 0;
    if (contentHeigth > 0)
    {
        sliderHeight = ((heigth - m_slider->getScaledHeight() - 20 - 20) * heigth) / contentHeigth;
    }
    if (sliderHeight < 11)
    {
        sliderHeight = 11;
    }
    else if (sliderHeight > (heigth - m_slider->getScaledHeight() - 20 - 20))
    {
        sliderHeight = (heigth - m_slider->getScaledHeight() - 20 - 20);
    }

    m_slider->setSize(18, sliderHeight);
    m_slider->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_slider->setPosition(9, 20);
    m_pBox->addChild(m_slider);
    oxygine::Sprite* pSlider = m_slider.get();
    m_slider->addEventListener(oxygine::TouchEvent::OVER, [this, pSlider, pObjectManager](oxygine::Event*)
    {
        if (m_enabled)
        {
            oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("h_scrollbar_active");
            pSlider->setResAnim(pAnimState);
            pSlider->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
        }
    });
    addEventListener(oxygine::TouchEvent::OUTX, [this](oxygine::Event*)
    {
        if (m_enabled)
        {
            setSliding(getSliding());
        }
    });
    m_slider->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [this](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            pEvent->stopPropagation();
            m_trackSliding = false;
            setSliding(true);
        }
    });
    m_slider->addEventListener(oxygine::TouchEvent::TOUCH_UP, [this](oxygine::Event* pEvent)
    {
        if (m_enabled && getSliding())
        {
            if (!m_trackSliding)
            {
                pEvent->stopPropagation();
            }
            setSliding(false);
        }
    });
    m_slider->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            pEvent->stopPropagation();
        }
    });
    m_pBox->addEventListener(oxygine::TouchEvent::MOVE, [this](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            scroll(pEvent);
        }
    });
    m_pBox->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [this](oxygine::Event* pEvent)
    {
        if (!m_enabled)
        {
            return;
        }
        auto target = pEvent->target.get();
        if (target == m_slider.get() ||
            target == m_pArrowDown.get() ||
            target == m_pArrowUp.get())
        {
            return;
        }
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent == nullptr)
        {
            return;
        }
        pTouchEvent->stopPropagation();
        m_trackSliding = true;
        setSliding(true);
        scrollToLocalY(pTouchEvent->localPosition.y());
    });
    m_pBox->addEventListener(oxygine::TouchEvent::TOUCH_UP, [this](oxygine::Event*)
    {
        if (!m_enabled)
        {
            return;
        }
        if (m_trackSliding && getSliding())
        {
            setSliding(false);
        }
    });

    addEventListener(oxygine::TouchEvent::WHEEL_DIR, [this](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
            if (pTouchEvent != nullptr && m_ContentHeigth > 0)
            {
               emit sigChangeScrollValue(-pTouchEvent->wheelDirection.y() / m_ContentHeigth);
               pTouchEvent->stopPropagation();
            }
        }
    });
    connect(this, &H_Scrollbar::sigChangeScrollValue, this, &H_Scrollbar::changeScrollValue, Qt::QueuedConnection);
}

H_Scrollbar::~H_Scrollbar()
{
    if (m_globalMoveListenerId >= 0)
    {
        auto stage = oxygine::Stage::getStage();
        if (stage != nullptr)
        {
            stage->removeEventListener(m_globalMoveListenerId);
        }
        m_globalMoveListenerId = -1;
    }
}

void H_Scrollbar::scroll(oxygine::Event* pEvent)
{
    if (m_sliding)
    {
        pEvent->stopPropagation();
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            scrollToLocalY(pTouchEvent->localPosition.y());
        }
        
    }
}

void H_Scrollbar::scrollToLocalY(qint32 localY)
{
    qint32 y = localY - m_slider->getScaledHeight() / 2;
    qint32 minY = 20;
    qint32 maxY = m_Heigth - m_slider->getScaledHeight() - 20;
    if (y < minY)
    {
        y = minY;
    }
    else if (y > maxY)
    {
        y = maxY;
    }
    m_slider->setY(y);
    auto div = m_Heigth - m_slider->getScaledHeight() - 20 - 20;
    if (div > 0)
    {
        m_Scrollvalue = static_cast<float>(y - 20) / static_cast<float>(div);
    }
    else
    {
        m_Scrollvalue = 0;
    }
    emit sigScrollValueChanged(m_Scrollvalue);
}

float H_Scrollbar::getScrollvalue() const
{
    return m_Scrollvalue;
}

qint32 H_Scrollbar::getContentHeigth() const
{
    return m_ContentHeigth;
}

bool H_Scrollbar::getSliding() const
{
    return m_sliding;
}

void H_Scrollbar::setSliding(bool sliding)
{
    m_sliding = sliding;
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    auto stage = oxygine::Stage::getStage();
    if (sliding)
    {
        if (m_globalMoveListenerId < 0 && stage != nullptr)
        {
            m_globalMoveListenerId = stage->addEventListenerWithId(oxygine::TouchEvent::MOVE,
                [this](oxygine::Event* pEvent)
            {
                if (!m_enabled || !m_sliding)
                {
                    return;
                }
                oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
                if (pTouchEvent == nullptr)
                {
                    return;
                }
                QPoint boxLocal = m_pBox->stage2local(pTouchEvent->localPosition, oxygine::Stage::getStage().get());
                scrollToLocalY(boxLocal.y());
            });
        }
        oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("h_scrollbar_pressed");
        m_slider->setResAnim(pAnimState);
    }
    else
    {
        m_trackSliding = false;
        if (m_globalMoveListenerId >= 0 && stage != nullptr)
        {
            stage->removeEventListener(m_globalMoveListenerId);
        }
        m_globalMoveListenerId = -1;
        oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("h_scrollbar");
        m_slider->setResAnim(pAnimState);
    }
}

void H_Scrollbar::setContentHeigth(qint32 heigth)
{    
    m_ContentHeigth = heigth;
    qint32 sliderHeight = 50;
    if (m_ContentHeigth > 0)
    {
        sliderHeight = ((m_Heigth - 20 - 20) * m_Heigth) / m_ContentHeigth;
    }
    if (sliderHeight < 11)
    {
        sliderHeight = 11;
    }
    else if (sliderHeight > (m_Heigth - 20 - 20))
    {
        sliderHeight = (m_Heigth - 20 - 20);
    }
    m_Scrollvalue = 0;
    m_slider->setSize(18, sliderHeight);
    changeScrollValue(m_Scrollvalue);
    
}

void H_Scrollbar::update(const oxygine::UpdateState&)
{
    // no need to calculate more than we need if we're invisible
    if(m_scroll != 0)
    {
        if (m_ScrollTimer.elapsed() > 250)
        {
            if (m_ContentHeigth > m_Heigth && m_ContentHeigth > 0)
            {
                m_speedCounter++;
                if (m_speedCounter % 8 == 0)
                {
                    m_Scrollspeed *= 2;
                }
                emit sigChangeScrollValue(m_scroll * m_Scrollspeed * 10.0f / static_cast<float>(m_ContentHeigth));
                m_ScrollTimer.start();
            }
        }
    }
}

void H_Scrollbar::changeScrollValue(float value)
{
    if (value != 0.0f)
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
        m_slider->setY(20 + m_Scrollvalue * (m_Heigth - m_slider->getScaledHeight() - 20 - 20));
        emit sigScrollValueChanged(m_Scrollvalue);
    }
}

void H_Scrollbar::setScrollvalue(float Scrollvalue)
{    
    if (m_Scrollvalue != Scrollvalue)
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
        m_slider->setY(20 + m_Scrollvalue * (m_Heigth - m_slider->getScaledHeight() - 20 - 20));
    }
}

void H_Scrollbar::setHeight(qint32 h)
{
    
    oxygine::Actor::setHeight(h);
    m_Heigth = h;
    m_pBox->setHeight(h);
    m_pArrowDown->setPosition(9, m_Heigth - m_pArrowDown->getScaledHeight() - 8);
    setContentHeigth(m_ContentHeigth);
    
}

void H_Scrollbar::setEnabled(bool value)
{
    if (!value)
    {
        m_scroll = 0;
        setSliding(false);
    }
    oxygine::Actor::setEnabled(value);
    m_slider->setEnabled(value);
    m_pBox->setEnabled(value);
    m_pArrowDown->setEnabled(value);
    m_pArrowUp->setEnabled(value);
}
