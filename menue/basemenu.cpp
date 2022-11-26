#include "menue/basemenu.h"

Basemenu::Basemenu()
    : m_onEnterTimer(this)
{
    connect(&m_onEnterTimer, &QTimer::timeout, this, &Basemenu::onEnter);
    m_onEnterTimer.setSingleShot(true);
    m_onEnterTimer.start(100);
}

bool Basemenu::getFocused() const
{
    return m_Focused;
}

void Basemenu::setFocused(bool Focused)
{
    m_Focused = Focused;
}
