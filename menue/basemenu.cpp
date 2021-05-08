#include "menue/basemenu.h"

Basemenu::Basemenu()
    : QObject()
{

}

bool Basemenu::getFocused() const
{
    return m_Focused;
}

void Basemenu::setFocused(bool Focused)
{
    m_Focused = Focused;
}
