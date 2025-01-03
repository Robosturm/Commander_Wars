#include <QFileInfo>

#include "resource_management/weaponmanager.h"

#include "game/unit.h"

#include "modding/csvtableimporter.h"

float WeaponManager::getBaseDamage(const QString & weaponID, Unit* pDefender)
{
    if (!weaponID.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "getBaseDamage";
        QJSValueList args({JsThis::getJsThis(pDefender)});
        QJSValue erg = pInterpreter->doFunction(weaponID, function1, args);
        if (erg.isNumber())
        {
            return erg.toNumber();
        }
        else
        {
            return -1.0f;
        }
    }
    else
    {
        return -1.0f;
    }
}

float WeaponManager::getEnviromentDamage(const QString & weaponID, const QString & terrainID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getEnviromentDamage";
    QJSValueList args({terrainID});
    QJSValue erg = pInterpreter->doFunction(weaponID, function1, args);
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
    QTemporaryDir tempDir = Settings::getInstance()->newTempDir();
    QStringList data;
    QFile file("resources/scripts/weapons/weapon_csv_import.txt");
    if (!file.exists())
    {
        file.setFileName(QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/scripts/weapons/weapon_csv_import.txt");
    }
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString jsHeader = stream.readAll();
    QStringList searchPaths = getSearchPaths();
    for (auto & path : searchPaths)
    {
        QString jsData = CsvTableImporter::ImportCsvTable(path + "/weapontable.csv", jsHeader, "damageTable", data);
        if (!jsData.isEmpty())
        {
            m_loadedRessources.append(data);
            pInterpreter->loadScript(jsData, tempDir.path() + "/weaponttable.js");
        }
    }
}
