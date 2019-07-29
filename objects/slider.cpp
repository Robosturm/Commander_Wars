#include "slider.h"

#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

Slider::Slider(qint32 width, qint32 minValue, qint32 maxValue, QString unit)
    : V_Scrollbar (width, width * 100 / 10),
      m_minValue(minValue),
      m_maxValue(maxValue),
      m_Unit(unit)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    V_Scrollbar::setScrollspeed( width / (maxValue - minValue));

    m_Textfield = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    m_Textfield->setStyle(style);
    m_CurrentValue = minValue;
    m_Textfield->setHtmlText((QString::number(minValue)  + " " + m_Unit).toStdString().c_str());
    addChild(m_Textfield);
    m_Textfield->setX(getWidth() + 10);
    connect(this, &Slider::V_Scrollbar::sigScrollValueChanged, this, &Slider::slotSliderValueChanged, Qt::QueuedConnection);
}

void Slider::slotSliderValueChanged(float value)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_CurrentValue = (m_maxValue - m_minValue) * value + m_minValue;
    m_Textfield->setHtmlText((QString::number(m_CurrentValue) + " " + m_Unit).toStdString().c_str());
    emit sliderValueChanged(m_CurrentValue);

    pApp->continueThread();
}

qint32 Slider::getCurrentValue() const
{
    return m_CurrentValue;
}

void Slider::setCurrentValue(const qint32 &CurrentValue)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();

    m_CurrentValue = CurrentValue;
    if (m_CurrentValue < m_minValue)
    {
        m_CurrentValue = m_minValue;
    }
    else if (m_CurrentValue > m_maxValue)
    {
        m_CurrentValue = m_maxValue;
    }
    else
    {
        // all fine do nothing
    }
    m_Textfield->setHtmlText((QString::number(m_CurrentValue) + " " + m_Unit).toStdString().c_str());
    float scrollValue = static_cast<float>(m_CurrentValue - m_minValue) / static_cast<float>(m_maxValue - m_minValue);
    V_Scrollbar::setScrollvalue(scrollValue);

    pApp->continueThread();
}
