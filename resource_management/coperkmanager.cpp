#include "resource_management/coperkmanager.h"

COPerkManager::COPerkManager()
    : RessourceManagement<COPerkManager>("/images/co_perks/res.xml",
                                         "/scripts/co_perks")
{
    setObjectName("COPerkManager");
}

QString COPerkManager::getIcon(qint32 position)
{
    if ((position >= 0) && (position < m_loadedRessources.size()))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue value = pInterpreter->doFunction(m_loadedRessources[position], "getIcon");
        if (value.isString())
        {
            return value.toString();
        }
    }
    return "";
}

bool COPerkManager::isSelectable(qint32 position)
{
    if ((position >= 0) && (position < m_loadedRessources.size()))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue value = pInterpreter->doFunction(m_loadedRessources[position], "isSelectable");
        if (value.isBool())
        {
            return value.toBool();
        }
    }
    return true;
}

QString COPerkManager::getDescription(qint32 position)
{
    if ((position >= 0) && (position < m_loadedRessources.size()))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue value = pInterpreter->doFunction(m_loadedRessources[position], "getDescription");
        if (value.isString())
        {
            return value.toString();
        }
    }
    return "";
}
