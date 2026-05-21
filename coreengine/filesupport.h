#ifndef FILESUPPORT_H
#define FILESUPPORT_H

#include <QFile>
#include <QObject>
#include <QDataStream>
#include <QCryptographicHash>
#include <QMap>
#include <QPair>

class Filesupport final
{
public:
    struct StringList
    {
        QString name;
        QStringList items;
    };
    struct ModSyncCaps
    {
        qint32 perModBytes{64 * 1024 * 1024};
        qint32 fileCountMax{5000};
        qint32 relPathMaxLen{260};
    };
    // rejectReason matches NetworkCommands::ModSyncRejectReason; qint32 keeps coreengine decoupled from multiplayer/.
    struct ModSyncPackage
    {
        QByteArray compressedBlob;
        qint32 declaredUncompressedSize{0};
        qint32 fileCount{0};
        qint32 rejectReason{0};
    };
    static const char* const LIST_FILENAME_ENDING;
    static constexpr qint32 LegacyRuntimeHashSize = 64;
    // Old clients read this field as a QByteArray length, so versions must stay small and never collide with 64.
    static constexpr qint32 LegacyHashPayloadVersion = 1;
    static constexpr qint32 CurrentHashPayloadVersion = 2;
    static_assert(CurrentHashPayloadVersion != LegacyRuntimeHashSize, "sentinel collision with legacy hash size");
    static_assert(CurrentHashPayloadVersion != LegacyHashPayloadVersion, "sentinel collision with legacy payload version");

    // Bit 0 = slice-1 mod-sync wire format; future schema breakages claim new bits.
    static constexpr quint32 CapabilityModSync = 0x00000001u;

    static constexpr qint32 ModPathDefaultMaxLen = 260;

    Filesupport() = delete;
    ~Filesupport() = delete;
    static QByteArray getLegacyRuntimeHash(const QStringList & mods);
    static QMap<QString, QByteArray> getPerModHashes(const QStringList & mods);
    static QMap<QString, QByteArray> getResourceFolderHashes();
    static QByteArray hashSingleFolder(const QString & folder, const QStringList & filter);
    static QByteArray getHash(const QStringList & filter, const QStringList & folders);
    static void addHash(QCryptographicHash & hash, const QString & folder, const QStringList & filter);
    static bool validateModPath(const QString & modPath, qint32 maxLen = ModPathDefaultMaxLen);
    static bool validateRelativeFilePath(const QString & relPath, qint32 maxLen);
    static ModSyncPackage buildModSyncPackage(const QString & installRoot, const QString & modPath, const ModSyncCaps & caps);
    static QMap<QString, QByteArray> extractModSyncPackage(const QByteArray & compressedBlob, qint32 declaredUncompressedSize, const ModSyncCaps & caps, qint32 & rejectReason);
    // Returns staging path RELATIVE to installRoot on success (e.g. "mods/foo.sync-staging-12345"); the caller writes that into the manifest.
    // `caps` re-validates relpath, file count, per-file and total bytes here as defense in depth; callers should still run extractModSyncPackage first.
    static QString stageModSync(const QString & installRoot, const QString & modPath, const QMap<QString, QByteArray> & files, const ModSyncCaps & caps, qint32 & rejectReason);
    static void reapModSyncFolders(const QString & installRoot, qint32 backupKeep = 3);
    static QString pendingModSyncManifestPath(const QString & userDataPath);
    static bool writePendingModSyncManifest(const QString & userDataPath, const QList<QPair<QString, QString>> & swaps);
    // Returns the list of `final` paths that were successfully swapped in; slice 3 uses this to drive settings mutation.
    static QStringList executePendingModSyncManifest(const QString & installRoot, const QString & userDataPath);

    struct RejoinManifest
    {
        bool valid{false};
        QString host;
        quint16 port{0};
        qint64 timestamp{0};
    };
    static QString rejoinManifestPath(const QString & userDataPath);
    static bool writeRejoinManifest(const QString & userDataPath, const QString & host, quint16 port);
    // Reads .rejoin.json, deletes the file, returns parsed contents. Caller decides freshness based on RejoinManifest::timestamp.
    static RejoinManifest consumeRejoinManifest(const QString & userDataPath);
    /**
     * @brief writeByteArray
     * @param stream
     * @param array
     */
    static void writeByteArray(QDataStream& stream, const QByteArray& array);
    /**
     * @brief writeBytes
     * @param stream
     * @param array
     */
    static void writeBytes(QDataStream& stream, const QByteArray& array);
    /**
     * @brief readByteArray
     * @param stream
     * @param array
     */
    static QByteArray readByteArray(QDataStream& stream);

    template<typename _TVectorList>
    static void writeVectorList(QDataStream& stream, const _TVectorList & array)
    {
        stream << static_cast<qint32>(array.size());
        for (qint32 i = 0; i < array.size(); i++)
        {
            stream << array[i];
        }
    }

    template<typename _TType, template<typename T> class _TVectorList>
    static _TVectorList<_TType> readVectorList(QDataStream& stream)
    {
        _TVectorList<_TType> array;
        qint32 size = 0;
        stream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            _TType value = 0;
            stream >> value;
            array.append(value);
        }
        return array;
    }
    /**
     * @brief storeList
     * @param file
     * @param items
     * @param folder
     */
    static void storeList(const QString & file, const QStringList  &items, const QString & folder);
    /**
     * @brief readList
     * @param file
     * @return
     */
    static StringList readList(const QString & file);
    /**
     * @brief readList
     * @param file
     * @param folder
     * @return
     */
    static StringList readList(const QString & file, const QString & folder);
    /**
     *
     */
    template<typename _TMap>
    static void writeMap(QDataStream& stream, const _TMap & map)
    {
        stream << static_cast<qint32>(map.size());
        auto iter = map.constBegin();
        while (iter != map.constEnd())
        {
            stream << iter.key();
            stream << iter.value();
            ++iter;
        }
    }
    /**
     *
     */
    template<typename _TKey, typename _TType, template<typename _T1, typename _T2> class _TMap>
    static _TMap<_TKey, _TType> readMap(QDataStream& stream)
    {
        _TMap<_TKey, _TType> map;
        qint32 size = 0;
        stream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            _TKey key;
            _TType value;
            stream >> key;
            stream >> value;
            map.insert(key, value);
        }
        return map;
    }
};

#endif // FILESUPPORT_H
