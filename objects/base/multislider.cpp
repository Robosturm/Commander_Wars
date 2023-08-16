#include "objects/base/multislider.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "coreengine/interpreter.h"

Multislider::Multislider(QStringList texts, qint32 width, QVector<qint32> values, qint32 startX)
{
#ifdef GRAPHICSUPPORT
    setObjectName("Multislider");
#endif
    Interpreter::setCppOwnerShip(this);

    qint32 textWidth = 0;
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 sliderX = startX;
    for (qint32 i = 0; i < texts.size(); i++)
    {
        m_Textfields.append(MemoryManagement::create<Label>(100));
        m_Textfields[i]->setStyle(style);
        m_Textfields[i]->setHtmlText(texts[i]);
        m_Textfields[i]->setY(i * 40);
        if (startX > 0)
        {
            textWidth = startX;
        }
        else if (m_Textfields[i]->getTextRect().width() > textWidth)
        {
            textWidth = m_Textfields[i]->getTextRect().width();
            sliderX = m_Textfields[i]->getTextRect().width() + SLIDER_OFFSET_X;
        }
        addChild(m_Textfields[i]);
    }
    for (auto & textField : m_Textfields)
    {
        textField->setWidth(textWidth);
    }
    width = width - textWidth - SLIDER_LOCK_BUTTON_WIDTH ;
    qint32 totalSliderValue = 0;
    setSize(width, texts.size() * 40);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    for (qint32 i = 0; i < texts.size(); i++)
    {
        m_Slider.append(MemoryManagement::create<Slider>(width, 0, 100));
        m_Slider[i]->setX(sliderX);
        m_Slider[i]->setY(i * 40);
        if (values.size() != texts.size() && texts.size() > 0)
        {
            m_Slider[i]->setCurrentValue(100.0f / texts.size());
        }
        else
        {
            m_Slider[i]->setCurrentValue(values[i]);
        }
        totalSliderValue += m_Slider[i]->getCurrentValue();

        connect(m_Slider[i].get(), &Slider::sliderValueChanged, this, [this, i](qint32)
        {
            emit signalSliderValueChanged(i);
        });
        addChild(m_Slider[i]);

        oxygine::spButton pLockButton = pObjectManager->createIconButton("unlock", 40);
        oxygine::Sprite* pSprite = oxygine::safeCast<Sprite*>(pLockButton->getFirstChild().get());


        qint32 x = m_Slider[i]->getX() + m_Slider[i]->getScaledWidth() + 10;
        pLockButton->setPosition(x, i * 40);        
        m_lockButtons.append(pLockButton);
        m_locked.append(false);
        addChild(pLockButton);
        // add click listener for locking
        oxygine::Actor* pActor = pLockButton.get();
        auto* pSlider = m_Slider[i].get();
        pLockButton->addClickListener([this, i, pActor, pSprite, pObjectManager, pSlider](oxygine::Event*)
        {
            if (m_locked[i])
            {
                pSprite->setResAnim(pObjectManager->getResAnim("unlock"));
            }
            else
            {
                pSprite->setResAnim(pObjectManager->getResAnim("lock"));
            }
            pSlider->setEnabled(m_locked[i]);
            m_locked[i] = !m_locked[i];
        });
    }
    connect(this, &Multislider::signalSliderValueChanged, this, &Multislider::sliderValueChanged, Qt::QueuedConnection);
    qint32 sliderDirection = 0;
    if (totalSliderValue > 100)
    {
        sliderDirection = -1;
    }
    else if (totalSliderValue < 100)
    {
        sliderDirection = 1;
    }
    if (m_Slider.size() > 0)
    {
        while (totalSliderValue != 100)
        {
            qint32 currentValue = m_Slider[m_currentSliderChange]->getCurrentValue();
            if ((currentValue > 0 && sliderDirection < 0) ||
                (currentValue < 100 && sliderDirection > 0))
            {
                m_Slider[m_currentSliderChange]->setCurrentValue(m_Slider[m_currentSliderChange]->getCurrentValue() + sliderDirection);
                totalSliderValue += sliderDirection;
            }
            m_currentSliderChange += 1;
            if (m_currentSliderChange >= m_Slider.size())
            {
                m_currentSliderChange = 0;
            }
        }
    }
}

qint32 Multislider::getSliderValue(qint32 slider)
{
    if ((slider >= 0) && (slider < m_Slider.size()))
    {
        return m_Slider[slider]->getCurrentValue();
    }
    else
    {
        return 0;
    }
}

void Multislider::sliderValueChanged(qint32 slider)
{
    
    qint32 totalSliderValue = 0;
    for (qint32 i = 0; i < m_Slider.size(); i++)
    {
        totalSliderValue += m_Slider[i]->getCurrentValue();
    }
    qint32 sliderDirection = 0;
    if (totalSliderValue > 100)
    {
        sliderDirection = -1;
    }
    else if (totalSliderValue < 100)
    {
        sliderDirection = 1;
    }
    bool changed = false;
    while (totalSliderValue != 100)
    {
        if (m_currentSliderChange != slider && !m_locked[m_currentSliderChange])
        {
            qint32 currentValue = m_Slider[m_currentSliderChange]->getCurrentValue();
            if ((currentValue > 0 && sliderDirection < 0) ||
                (currentValue < 100 && sliderDirection > 0))
            {
                changed = true;
                m_Slider[m_currentSliderChange]->setCurrentValue(m_Slider[m_currentSliderChange]->getCurrentValue() + sliderDirection);
                totalSliderValue += sliderDirection;
            }
        }
        m_currentSliderChange += 1;
        if (m_currentSliderChange >= m_Slider.size())
        {
            if (!changed)
            {
                m_Slider[slider]->setCurrentValue(m_Slider[slider]->getCurrentValue() + sliderDirection);
                totalSliderValue += sliderDirection;
            }
            m_currentSliderChange = 0;
            changed = false;
        }
    }
    emit signalSliderChanged();
    
}

void Multislider::setEnabled(bool value)
{
    for (qint32 i = 0; i < m_Slider.size(); i++)
    {
        m_Slider[i]->setEnabled(value);
        m_Textfields[i]->setEnabled(value);
        m_lockButtons[i]->setEnabled(value);
    }
}
