#include "resource_management/customspritemanager.h"
#include "coreengine/interpreter.h"

CustomSpriteManager* CustomSpriteManager::m_pInstance = nullptr;

CustomSpriteManager* CustomSpriteManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        Interpreter::setCppOwnerShip(m_pInstance);
        m_pInstance = new CustomSpriteManager();
    }
    return m_pInstance;
}

oxygine::ResAnim* CustomSpriteManager::getResAnim(QString id)
{
    if (!m_resources.contains(id))
    {

    }
    return m_resources[id].get();
}

void CustomSpriteManager::release()
{
    m_resources.clear();
}
