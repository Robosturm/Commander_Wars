#include "fontmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"
QColor FontManager::defaultColor{QColor(230, 200, 60)};

void FontManager::setFontColor(QColor color)
{
    defaultColor = color;
}

QColor FontManager::getFontColor()
{
    return defaultColor;
}
