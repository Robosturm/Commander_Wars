#ifndef COREENGINE_VFS_H
#define COREENGINE_VFS_H

#include <QString>
#include <QStringList>
#include <QFileInfoList>

class Vfs final
{
    public:

    static void init();

    /**
     * Locates the real path for a resource, or a file path guaranteed not to exist if it is not found.
     */
    static QString find(const QString& name, bool checkMods = true);

    /**
     * Finds all real paths that match a particular file name.
     */
    static QStringList findAll(const QString& name, bool checkMods = true) {
        return findAllInternal(name, checkMods, false);
    }

    /**
     * Finds all real paths that match a particular file name in reverse order.
     */
    static QStringList findAllRev(const QString& name, bool checkMods = true) {
        return findAllInternal(name, checkMods, true);
    }

    /**
     * Builds the search path for a particular file path in reverse order.
     */
    static QStringList createSearchPath(const QString& name, bool checkMods = true) {
        return createSearchPathInternal(name, checkMods, false);
    }

    /**
     * Builds the search path for a particular file path in reverse order.
     */
    static QStringList createSearchPathRev(const QString& name, bool checkMods = true) {
        return createSearchPathInternal(name, checkMods, true);
    }

    /**
     * Lists all files in a particular directory.
     */
    static QFileInfoList list(const QString& name, const QStringList& filters = emptyList, bool checkMods = true);

    private:

    Vfs() = delete;
    ~Vfs() = delete;

    static QStringList createSearchPathInternal(const QString& name, bool checkMods = true, bool firstPriority = false);
    static QStringList findAllInternal(const QString& name, bool checkMods = true, bool firstPriority = false);

    static const QStringList emptyList;
};

#endif // COREENGINE_VFS_H
