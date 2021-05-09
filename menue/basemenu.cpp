#include "menue/basemenu.h"

#include "coreengine/interpreter.h"

Basemenu::Basemenu()
    : QObject()
{
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

