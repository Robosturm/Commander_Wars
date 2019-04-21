#include "objects/h_scrollbar.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"

#include "coreengine/console.h"

#include "coreengine/mainapp.h"

H_Scrollbar::H_Scrollbar(qint32 heigth, qint32 contentHeigth)
    : m_Heigth(heigth),
      m_ContentHeigth(contentHeigth)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    m_ScrollTimer.start();
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setSize(33, heigth);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("scrollbar");
    oxygine::spBox9Sprite pBox = new oxygine::Box9Sprite();
    pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setResAnim(pAnim);
    pBox->setSize(33, heigth);
    this->addChild(pBox);

    oxygine::spButton pArrowDown = new oxygine::Button();
    pArrowDown->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+down"));
    pArrowDown->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    pArrowDown->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        pArrowDown->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(16, 16, 16, 0)), 300);
    });
    pArrowDown->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        pArrowDown->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(0, 0, 0, 0)), 300);
    });
    pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [ = ](oxygine::Event*)
    {
        m_scroll = 1;
    });
    pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event*)
    {
        m_scroll = 0;
    });
    pBox->addChild(pArrowDown);
    pArrowDown->setPosition(9, heigth - pArrowDown->getHeight() - 8);

    oxygine::spButton pArrowUp = new oxygine::Button();
    // pButton->setPosition(200, 200);
    pArrowUp->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+down"));
    pArrowUp->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    pArrowUp->setFlippedY(true);
    pArrowUp->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        pArrowUp->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(16, 16, 16, 0)), 300);
    });
    pArrowUp->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        pArrowUp->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(0, 0, 0, 0)), 300);
    });
    pArrowUp->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [ = ](oxygine::Event*)
    {
        m_scroll = -1;
    });
    pArrowUp->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event*)
    {
        m_scroll = 0;
    });
    pBox->addChild(pArrowUp);
    pArrowUp->setPosition(9, 8);

    m_slider = new oxygine::Box9Sprite();
    pAnim = pObjectManager->getResAnim("h_scrollbar");
    m_slider->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_slider->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_slider->setResAnim(pAnim);

    qint32 sliderHeight = 50;
    sliderHeight = ((heigth - m_slider->getHeight() - 20 - 20) * heigth) / contentHeigth;
    if (sliderHeight < 11)
    {
        sliderHeight = 11;
    }
    else if (sliderHeight > (heigth - m_slider->getHeight() - 20 - 20))
    {
        sliderHeight = (heigth - m_slider->getHeight() - 20 - 20);
    }

    m_slider->setSize(18, sliderHeight);
    m_slider->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    m_slider->setPosition(9, 20);
    pBox->addChild(m_slider);
    m_slider->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("h_scrollbar_active");
        m_slider->setResAnim(pAnimState);
        m_slider->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(16, 16, 16, 0)), 300);
    });
    m_slider->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("h_scrollbar");
        m_slider->setResAnim(pAnimState);
        pArrowUp->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(0, 0, 0, 0)), 300);
    });
    m_slider->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [ = ](oxygine::Event*)
    {
        oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("h_scrollbar_pressed");
        m_slider->setResAnim(pAnimState);
        m_sliding = true;
    });
    m_slider->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event*)
    {
        oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("h_scrollbar");
        m_slider->setResAnim(pAnimState);
        m_sliding = false;
    });
    pBox->addEventListener(oxygine::TouchEvent::MOVE, [ = ](oxygine::Event* pEvent)
    {
        if (m_sliding)
        {
            oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
            if (pTouchEvent != nullptr)
            {
                qint32 y = pTouchEvent->localPosition.y - m_slider->getHeight() / 2;
                if (y < 20)
                {
                    y = 20;
                }
                else if (y > this->getHeight() - m_slider->getHeight() - 20)
                {
                    y = this->getHeight() - m_slider->getHeight() - 20;
                }
                m_slider->setY(y);
                // calc new scroll value :)
                if (static_cast<float>(this->getHeight() - m_slider->getHeight() - 20 - 20) > 0)
                {
                    m_Scrollvalue = static_cast<float>(y - 20) / static_cast<float>(this->getHeight() - m_slider->getHeight() - 20 - 20);
                }
                else
                {
                    m_Scrollvalue = 0;
                }
                emit sigScrollValueChanged(m_Scrollvalue);
            }
        }
    });
    connect(this, &H_Scrollbar::sigChangeScrollValue, this, &H_Scrollbar::changeScrollValue, Qt::QueuedConnection);
}

void H_Scrollbar::setContentHeigth(qint32 heigth)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_ContentHeigth = heigth;
    qint32 sliderHeight = 50;
    sliderHeight = ((this->getHeight() - 20 - 20) * this->getHeight()) / m_ContentHeigth;
    if (sliderHeight < 11)
    {
        sliderHeight = 11;
    }
    else if (sliderHeight > (this->getHeight() - 20 - 20))
    {
        sliderHeight = (this->getHeight() - 20 - 20);
    }
    m_Scrollvalue = 0;
    m_slider->setSize(18, sliderHeight);
    emit sigScrollValueChanged(m_Scrollvalue);
    pApp->continueThread();
}

void H_Scrollbar::update(const oxygine::UpdateState& us)
{
    // no need to calculate more than we need if we're invisible
    if(m_scroll != 0)
    {
        if (m_ScrollTimer.elapsed() > 250)
        {
            if (m_ContentHeigth > m_Heigth)
            {
                emit sigChangeScrollValue(m_scroll * 10.0f / static_cast<float>(m_ContentHeigth));
                m_ScrollTimer.start();
            }
        }
    }
}

void H_Scrollbar::changeScrollValue(float value)
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
    m_slider->setY(20 + m_Scrollvalue * (m_Heigth - m_slider->getHeight() - 20 - 20));
    emit sigScrollValueChanged(m_Scrollvalue);
}
