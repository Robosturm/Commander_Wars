#include "backgroundmanager.h"

BackgroundManager* BackgroundManager::m_pInstance = nullptr;

BackgroundManager* BackgroundManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new BackgroundManager();
    }
    return m_pInstance;
}


BackgroundManager::BackgroundManager()
{
    oxygine::Resources::loadXML(QString("resources/images/backgrounds/res.xml"));
}
