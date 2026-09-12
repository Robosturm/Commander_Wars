#include "menue/basemenu.h"

Basemenu::Basemenu()
    : m_onEnterTimer(this)
{
    connect(&m_onEnterTimer, &QTimer::timeout, this, &Basemenu::onEnter);
    m_onEnterTimer.setSingleShot(true);
    m_onEnterTimer.start(10000);
}

bool Basemenu::getFocused() const
{
    return m_Focused;
}

void Basemenu::setFocused(bool Focused)
{
    m_Focused = Focused;
}

void Basemenu::showCustomDialog(const QString & jsName, const QString & uiXml, const QString & confirmText, bool contentString)
{
    spCustomDialog pCustomDialog = MemoryManagement::create<CustomDialog>(jsName, uiXml, this, confirmText, contentString);
    addChild(pCustomDialog);
}