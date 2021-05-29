#include <QFileInfo>
#include <QDirIterator>

#include "resource_management/weaponmanager.h"

#include "coreengine/mainapp.h"

#include "game/unit.h"

#include "modding/csvtableimporter.h"

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
    RessourceManagement<WeaponManager>::loadAll();
    // import csv tables
    // clean up temp folder
    Interpreter* pInterpreter = Interpreter::getInstance();
    QDir dir("temp/");
    dir.removeRecursively();
    dir.mkpath(".");
    QStringList data;
    QFile file(QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/scripts/weapons/weapon_csv_import.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString jsHeader = stream.readAll();
    QStringList searchPaths = getSearchPaths();
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString jsData = CsvTableImporter::ImportCsvTable(searchPaths[i] + "/weapontable.csv", jsHeader, "damageTable", data);
        if (!jsData.isEmpty())
        {
            m_loadedRessources.append(data);
            pInterpreter->loadScript(jsData, "temp/weaponttable.js");
        }
    }
}
