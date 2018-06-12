#include "fontmanager.h"

FontManager* FontManager::m_pInstance = nullptr;

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
    oxygine::Resources::loadXML("resources/fonts/fonts.xml");
}
