#include "multislider.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "objects/base/label.h"

#include "coreengine/mainapp.h"

Multislider::Multislider(QStringList texts, qint32 width, QVector<qint32> values)
{
#ifdef GRAPHICSUPPORT
    setObjectName("Multislider");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    qint32 textWidth = 0;
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    for (qint32 i = 0; i < texts.size(); i++)
    {
        m_Textfields.append(oxygine::spTextField::create());
        m_Textfields[i]->setStyle(style);
        m_Textfields[i]->setHtmlText(texts[i]);
        m_Textfields[i]->setY(i * 40);
        if (m_Textfields[i]->getTextRect().getWidth() > textWidth)
        {
            textWidth = m_Textfields[i]->getTextRect().getWidth();
        }
        addChild(m_Textfields[i]);
    }
    width = width - textWidth - 10 - 110 - 50;
    qint32 totalSliderValue = 0;
    setSize(width, texts.size() * 40);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    for (qint32 i = 0; i < texts.size(); i++)
    {
        m_Slider.append(spSlider::create(width, 0, 100));
        m_Slider[i]->setX(textWidth + 10);
        m_Slider[i]->setY(i * 40);
        if (values.size() != texts.size())
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


        qint32 x = m_Slider[i]->getX() + m_Slider[i]->getWidth() + 10;
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
