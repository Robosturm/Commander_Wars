#include "unitspritemanager.h"

#include "coreengine/mainapp.h"

UnitSpriteManager* UnitSpriteManager::m_pInstance = nullptr;

UnitSpriteManager::UnitSpriteManager()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        oxygine::Resources::loadXML(QString(pMainapp->getSettings()->getMods().at(i) + "/images/units/res.xml").toStdString());
    }
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
