#include "coperkmanager.h"

COPerkManager::COPerkManager()
    : RessourceManagement<COPerkManager>("/images/co_perks/res.xml",
                                         "/scripts/co_perks")
{
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
