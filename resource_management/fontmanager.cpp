#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

QColor FontManager::defaultColor{QColor(250, 210, 0)};

void FontManager::setFontColor(QColor color)
{
    defaultColor = color;
}

QColor FontManager::getFontColor()
{
    return defaultColor;
}
