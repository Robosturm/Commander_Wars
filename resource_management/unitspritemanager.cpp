#include <QFileInfo>

#include <QDirIterator>
#include "unitspritemanager.h"
#include "coreengine/mainapp.h"

UnitSpriteManager* UnitSpriteManager::m_pInstance = nullptr;

UnitSpriteManager::UnitSpriteManager()
{
    oxygine::Resources::loadXML("resources/images/units/res.xml");
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        if (QFile::exists(pMainapp->getSettings()->getMods().at(i) + "/images/units/res.xml"))
        {
            oxygine::Resources::loadXML(pMainapp->getSettings()->getMods().at(i) + "/images/units/res.xml");
        }
    }
}

UnitSpriteManager* UnitSpriteManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new UnitSpriteManager();
    }
    return m_pInstance;
}

void UnitSpriteManager::loadAll()
{
    reset();
    Mainapp* pMainapp = Mainapp::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/units");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/units");
    }
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString path =  QCoreApplication::applicationDirPath() + "/" + searchPaths[i];
        QStringList filter;
        filter << "*.js";
        QDirIterator* dirIter = new QDirIterator(path, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter->hasNext())
        {
            dirIter->next();
            QString file = dirIter->fileInfo().fileName().split(".").at(0);
            if (!m_loadedUnits.contains(file.toUpper()))
            {
                loadUnit(file.toUpper());
            }
        }
    }
    m_loadedUnits.sort();
}


bool UnitSpriteManager::loadUnit(QString unitID)
{
    Mainapp* pMainapp = Mainapp::getInstance();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/units");
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/units");
    }
    bool bRet = false;
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + unitID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pInterpreter->openScript(file);
            if (!bRet)
            {
                m_loadedUnits.append(unitID);
            }
            bRet = true;
        }
    }
    return bRet;
}

GameEnums::UnitType UnitSpriteManager::getUnitType(qint32 i)
{
    if ((i >= 0) && (i < m_loadedUnits.size()))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "getUnitType";
        QJSValue ret = pInterpreter->doFunction(m_loadedUnits[i], function1);
        if (ret.isNumber())
        {
            return static_cast<GameEnums::UnitType>(ret.toInt());
        }
    }
    return GameEnums::UnitType_Ground;
}

qint32 UnitSpriteManager::getUnitIndex(QString id)
{
    for (qint32 i = 0; i < m_loadedUnits.size(); i++)
    {
        if (m_loadedUnits[i] == id)
        {
            return i;
        }
    }
    return -1;
}

void UnitSpriteManager::reset()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    for (qint32 i = 0; i < m_loadedUnits.size(); i++)
    {
        pInterpreter->deleteObject(m_loadedUnits[i]);
    }
    m_loadedUnits.clear();
}

bool UnitSpriteManager::existsUnit(QString unitID)
{
    for (qint32 i = 0; i < m_loadedUnits.size(); i++)
    {
        if (m_loadedUnits[i] == unitID)
        {
            return true;
        }
    }
    return false;
}

QString UnitSpriteManager::getUnitName(qint32 position)
{
    if ((position >= 0) && (position < m_loadedUnits.size()))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue value = pInterpreter->doFunction(m_loadedUnits[position], "getName");
        if (value.isString())
        {
            return value.toString();
        }
    }
    return "";
}

QString UnitSpriteManager::getUnitName(QString id)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue value = pInterpreter->doFunction(id, "getName");
    if (value.isString())
    {
        return value.toString();
    }
    return "";
}

QStringList UnitSpriteManager::getUnitsSorted()
{
    QStringList sortedUnits;
    QVector<GameEnums::UnitType> unitTypes;
    for (qint32 i = 0; i < getUnitCount(); i++)
    {
        GameEnums::UnitType unitType = getUnitType(i);
        if (!unitTypes.contains(unitType))
        {
            unitTypes.append(unitType);
        }
    }
    for (qint32 i2 = 0; i2 < unitTypes.size(); i2++)
    {
        for (qint32 i = 0; i < getUnitCount(); i++)
        {
            if (getUnitType(i) == unitTypes[i2])
            {
                sortedUnits.append(getUnitID(i));
            }
        }
    }
    return sortedUnits;
}
