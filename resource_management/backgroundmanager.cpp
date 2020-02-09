#include "backgroundmanager.h"

#include "coreengine/mainapp.h"

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
    oxygine::Resources::loadXML("resources/images/backgrounds/res.xml");
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        if (QFile::exists(pMainapp->getSettings()->getMods().at(i) + "/images/backgrounds/res.xml"))
        {
            oxygine::Resources::loadXML(pMainapp->getSettings()->getMods().at(i) + "/images/backgrounds/res.xml");
        }
    }
}
