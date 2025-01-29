#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QSet>

#include "coreengine/vfs.h"

const QStringList Vfs::emptyList;

struct SearchPath final
{
    QString root;
    bool isModPath = false;
};
static QList<SearchPath> searchPath;

void Vfs::setSearchPath(const QString& userPath, const QStringList& mods)
{
    CONSOLE_PRINT("Initializing VFS...", GameConsole::eINFO);

    searchPath.clear();

    searchPath.append({ userPath });

#ifndef USEAPPCONFIGPATH
    // USEAPPCONFIGPATH is primarily set on Linux, where the "current directory" of programs launched from the start
    // menu is normally the user's home directory. This is very unexpected behavior, and this should not be checked.
    if (QFileInfo(".") != QFileInfo(userPath))
    {
        searchPath.append({ "." });
    }
#endif

    for (const auto & mod : std::as_const(mods))
    {
        searchPath.append({ userPath + mod, true });
        searchPath.append({ oxygine::Resource::RCC_PREFIX_PATH + mod, true });
    }

#ifdef DEPLOY_RESOURCES_AS_FOLDER
    searchPath.append({ QCoreApplication::applicationDirPath() });

#ifdef __linux__
    searchPath.append({ QCoreApplication::applicationDirPath() + "/../share/commander_wars" });
#endif
#endif

    searchPath.append({ oxygine::Resource::RCC_PREFIX_PATH });

    for (auto & path : searchPath)
    {
        CONSOLE_PRINT("- Path: " + path.root + (path.isModPath ? " (mod data)" : ""), GameConsole::eINFO);
    }
}

struct ProcessedName final
{
    QString path;
    bool isResources = false;
    QString resourcesPath = "";
};
static ProcessedName processName(const QString& pName)
{
    // Strip leading slashes
    QString name = pName;
    while (name.startsWith("/"))
        name = name.last(name.length() - 1);

    // Search for the root directory itself.
    if (name.isEmpty())
    {
        return { "" };
    }
    // Search for the resources directory itself.
    else if (name == "resources" || name == "resources/")
    {
        return { "/" + name, true, name.last(name.length() - 9) };
    }
    // Search for a file in the resources directory
    else if (name.startsWith("resources/"))
    {
        return { "/" + name, true, name.last(name.length() - 9) };
    }
    // Search for a file anywhere else
    else
    {
        return { "/" + name };
    }
}

QString Vfs::find(const QString& pName, bool checkMods) {
    auto name = processName(pName);

    QString newPath;
    for (auto & path : searchPath)
    {
        if (!checkMods && path.isModPath)
        {
            continue;
        }

        newPath = path.root + name.path;
        if (QFileInfo(newPath).exists())
        {
            return newPath;
        }

        if (name.isResources && path.isModPath)
        {
            newPath = path.root + name.resourcesPath;
            if (QFileInfo(newPath).exists())
            {
                return newPath;
            }
        }
    }

    return ":/this_should_not_exist" + name.path;
}

QStringList Vfs::createSearchPathInternal(const QString& pName, bool checkMods, bool firstPriority)
{
    auto name = processName(pName);

    QStringList list;
    QString newPath;
    for (auto & path : searchPath)
    {
        if (!checkMods && path.isModPath)
        {
            continue;
        }

        newPath = path.root + name.path;
        list.append(newPath);

        if (name.isResources && path.isModPath)
        {
            newPath = path.root + name.resourcesPath;
            list.append(newPath);
        }
    }

    if (!firstPriority)
    {
        std::reverse(list.begin(), list.end());
    }
    return list;
}

QStringList Vfs::findAllInternal(const QString& pName, bool checkMods, bool firstPriority)
{
    auto name = processName(pName);

    QStringList list;
    QString newPath;
    for (auto & path : searchPath)
    {
        if (!checkMods && path.isModPath)
        {
            continue;
        }

        newPath = path.root + name.path;
        if (QFileInfo(newPath).exists())
        {
            list.append(newPath);
        }

        if (name.isResources && path.isModPath)
        {
            newPath = path.root + name.resourcesPath;
            if (QFileInfo(newPath).exists())
            {
                list.append(newPath);
            }
        }
    }

    if (!firstPriority)
    {
        std::reverse(list.begin(), list.end());
    }
    return list;
}

QFileInfoList Vfs::list(const QString& name, const QStringList& filters, bool checkMods)
{
    QSet<QString> foundBaseNames;
    QFileInfoList infoList;

    for (auto & path : Vfs::createSearchPathRev(name, checkMods))
    {
        QFileInfo pathInfo(path);
        if (pathInfo.exists() && pathInfo.isDir())
        {
            for (const auto & entry : QDir(path).entryInfoList(QDir::Dirs))
            {
                if (!foundBaseNames.contains(entry.baseName()))
                {
                    infoList.append(entry);
                    foundBaseNames.insert(entry.baseName());
                }
            }

            if (!filters.isEmpty())
            {
                for (const auto & entry : QDir(path).entryInfoList(filters, QDir::Files))
                {
                    if (!foundBaseNames.contains(entry.baseName()))
                    {
                        infoList.append(entry);
                        foundBaseNames.insert(entry.baseName());
                    }
                }
            }
        }
    }

    return infoList;
}
