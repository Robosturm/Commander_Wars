#include "unitspritemanager.h"

UnitSpriteManager* UnitSpriteManager::m_pInstance = nullptr;

UnitSpriteManager::UnitSpriteManager()
{
    oxygine::Resources::loadXML("resources/images/units/res.xml");
}

UnitSpriteManager* UnitSpriteManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new UnitSpriteManager();
    }
    return m_pInstance;
}
