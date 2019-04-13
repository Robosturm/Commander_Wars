#include "gameanimationmanager.h"

#include <QFileInfo>

#include <QDirIterator>
#include "coreengine/mainapp.h"

GameAnimationManager* GameAnimationManager::m_pInstance = nullptr;

GameAnimationManager::GameAnimationManager()
{
    oxygine::Resources::loadXML("resources/images/animations/res.xml");
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        if (QFile::exists(pMainapp->getSettings()->getMods().at(i) + "/images/animations/res.xml"))
        {
            oxygine::Resources::loadXML(QString(pMainapp->getSettings()->getMods().at(i) + "/images/animations/res.xml").toStdString());
        }
    }
}

GameAnimationManager* GameAnimationManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new GameAnimationManager();
    }
    return m_pInstance;
}
