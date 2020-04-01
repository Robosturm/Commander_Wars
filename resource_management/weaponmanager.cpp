#include <QFileInfo>

#include <QDirIterator>

#include "weaponmanager.h"

#include "coreengine/mainapp.h"

#include "modding/csvtableimporter.h"

WeaponManager* WeaponManager::m_pInstance = nullptr;

WeaponManager::WeaponManager()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
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
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getName";
    QJSValue ret = pInterpreter->doFunction(weaponID, function1);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "";
    }
}

bool WeaponManager::existsWeapon(QString weaponID)
{
    for (qint32 i = 0; i < m_loadedWeapons.size(); i++)
    {
        if (m_loadedWeapons[i] == weaponID)
        {
            return true;
        }
    }
    return false;
}

void WeaponManager::reset()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    for (qint32 i = 0; i < m_loadedWeapons.size(); i++)
    {
        pInterpreter->deleteObject(m_loadedWeapons[i]);
    }
    m_loadedWeapons.clear();
}

float WeaponManager::getBaseDamage(QString weaponID, Unit* pDefender)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBaseDamage";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(pDefender);
    args1 << obj1;
    QJSValue erg = pInterpreter->doFunction(weaponID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toNumber();
    }
    else
    {
        return -1.0f;
    }
}

float WeaponManager::getEnviromentDamage(QString weaponID, QString terrainID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getEnviromentDamage";
    QJSValueList args1;
    args1 << terrainID;
    QJSValue erg = pInterpreter->doFunction(weaponID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toNumber();
    }
    else
    {
        return -1.0f;
    }
}

void WeaponManager::loadAll()
{
    reset();
    Mainapp* pMainapp = Mainapp::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/weapons");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        searchPaths.append(Settings::getMods().at(i) + "/scripts/weapons");
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

    // import csv tables
    // clean up temp folder
    Interpreter* pInterpreter = Interpreter::getInstance();
    QDir dir("temp/");
    dir.removeRecursively();
    dir.mkpath(".");
    QStringList data;
    QFile file("resources/scripts/weapons/weapon_csv_import.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString jsHeader = stream.readAll();
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString jsData = CsvTableImporter::ImportCsvTable(searchPaths[i] + "/weapontable.csv", jsHeader, "damageTable", data);
        if (!jsData.isEmpty())
        {
            m_loadedWeapons.append(data);
            QFile tmp("temp/table.js");
            tmp.open(QIODevice::WriteOnly);
            QTextStream stream2(&tmp);
            stream2 << jsData;
            tmp.close();
            pInterpreter->openScript("temp/table.js", true);
            QFile::remove("temp/table.js");
        }
    }
}

bool WeaponManager::loadWeapon(QString weaponID)
{
    Mainapp* pMainapp = Mainapp::getInstance();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/weapons");
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        searchPaths.append(Settings::getMods().at(i) + "/scripts/weapons");
    }
    bool bRet = false;
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + weaponID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pInterpreter->openScript(file, true);
            if (!bRet)
            {
                m_loadedWeapons.append(weaponID);
            }
            bRet = true;
        }
    }    
    return bRet;
}
