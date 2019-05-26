#include <QFileInfo>

#include <QDirIterator>

#include "weaponmanager.h"

#include "coreengine/mainapp.h"

WeaponManager* WeaponManager::m_pInstance = nullptr;

WeaponManager::WeaponManager()
{

}

WeaponManager* WeaponManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new WeaponManager();
    }
    return m_pInstance;
}

QString WeaponManager::getWeaponName(QString weaponID)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getName";
    QJSValue ret = pApp->getInterpreter()->doFunction(weaponID, function1);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "";
    }
}

void WeaponManager::reset()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < m_loadedWeapons.size(); i++)
    {
        pMainapp->getInterpreter()->deleteObject(m_loadedWeapons[i]);
    }
    m_loadedWeapons.clear();
}

float WeaponManager::getBaseDamage(const QString& weaponID, Unit* pDefender)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getBaseDamage";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pDefender);
    args1 << obj1;
    QJSValue erg = pApp->getInterpreter()->doFunction(weaponID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toNumber();
    }
    else
    {
        return 0.0f;
    }
}

void WeaponManager::loadAll()
{
    reset();
    Mainapp* pMainapp = Mainapp::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/weapons");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/weapons");
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
            if (!m_loadedWeapons.contains(file.toUpper()))
            {
                loadWeapon(file.toUpper());
            }
        }
    }
}

bool WeaponManager::loadWeapon(const QString& weaponID)
{
    Mainapp* pMainapp = Mainapp::getInstance();

    QStringList searchPaths;
    searchPaths.append("resources/scripts/weapons");
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/weapons");
    }
    bool bRet = false;
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + weaponID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pMainapp->getInterpreter()->openScript(file);
            if (!bRet)
            {
                m_loadedWeapons.append(weaponID);
            }
            bRet = true;
        }
    }
    return bRet;
}
