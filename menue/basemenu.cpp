#include "menue/basemenu.h"

#include "coreengine/interpreter.h"

Basemenu::Basemenu()
    : QObject(),
      m_onEnterTimer(this)
{
    connect(&m_onEnterTimer, &QTimer::timeout, this, &Basemenu::onEnter);
    m_onEnterTimer.setSingleShot(true);
    m_onEnterTimer.start(200);
}

Basemenu::~Basemenu()
{
    for (auto & pItem : m_factoryUiItem)
    {
        pItem->detach();
    }
}

bool Basemenu::getFocused() const
{
    return m_Focused;
}

void Basemenu::setFocused(bool Focused)
{
    m_Focused = Focused;
}

void Basemenu::addFactoryUiItem(oxygine::spActor pItem)
{
    m_factoryUiItem.append(pItem);
    addChild(pItem);
}

