#include "fontmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

FontManager* FontManager::m_pInstance = nullptr;
QColor FontManager::defaultColor{QColor(230, 200, 60)};

FontManager* FontManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new FontManager();
    }
    return m_pInstance;
}

FontManager::FontManager()
{
    Interpreter::setCppOwnerShip(this);
    oxygine::Resources::loadXML("resources/fonts/fonts.xml");
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        if (QFile::exists(Settings::getMods().at(i) + "/fonts/fonts.xml"))
        {
            oxygine::Resources::loadXML(Settings::getMods().at(i) + "/fonts/fonts.xml");
        }
    }
}

void FontManager::setFontColor(QColor color)
{
    defaultColor = color;
}

QColor FontManager::getFontColor()
{
    return defaultColor;
}
