#include "coreengine/filesupport.h"
#include "coreengine/settings.h"
#include "coreengine/gameconsole.h"

#include <QDirIterator>
#include <QCoreApplication>
#include <QDateTime>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <QSet>
#include <QtEndian>
#include <limits>

const char* const Filesupport::LIST_FILENAME_ENDING = ".bl";

QByteArray Filesupport::getHash(const QStringList & filter, const QStringList & folders)
{
    QCryptographicHash myHash(QCryptographicHash::Sha512);
    QStringList fullList;

    QString userPath = Settings::getInstance()->getUserPath();
    for (const auto & folder : std::as_const(folders))
    {
        fullList.append(oxygine::Resource::RCC_PREFIX_PATH + folder);
        fullList.append(userPath + folder);
        if (!userPath.isEmpty())
        {
            fullList.append(folder);
        }
    }
    for (const auto & folder : std::as_const(fullList))
    {
        CONSOLE_PRINT_MODULE("Adding files for folder: " + folder, GameConsole::eDEBUG, GameConsole::eFileSupport);
        addHash(myHash, folder, filter);
    }
    return myHash.result();
}

void Filesupport::addHash(QCryptographicHash & hash, const QString & folder, const QStringList & filter)
{
    QDir dir(folder);
    auto list = dir.entryInfoList(filter, QDir::Files);
    for (auto & item : list)
    {
        QString filePath = item.filePath();
        CONSOLE_PRINT_MODULE("Adding file: " + filePath + " to hash", GameConsole::eDEBUG, GameConsole::eFileSupport);
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);
        while (!file.atEnd())
        {
            hash.addData(file.readLine().trimmed());
        }
        file.close();
    }
    list = dir.entryInfoList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
    for (auto & item : list)
    {
        QString path = item.filePath();
        addHash(hash, path, filter);
    }
}

namespace
{
    const QStringList kHashFileFilter = {QStringLiteral("*.js"), QStringLiteral("*.csv")};
    const auto kResourceScriptsFolder = QStringLiteral("resources/scripts");
    const auto kResourceAidataFolder = QStringLiteral("resources/aidata");
}

QByteArray Filesupport::getLegacyRuntimeHash(const QStringList & mods)
{
    QStringList folders = mods;
    folders.append(kResourceScriptsFolder);
    folders.append(kResourceAidataFolder);
    return getHash(kHashFileFilter, folders);
}

QByteArray Filesupport::hashSingleFolder(const QString & folder, const QStringList & filter)
{
    return getHash(filter, QStringList{folder});
}

QMap<QString, QByteArray> Filesupport::getPerModHashes(const QStringList & mods)
{
    QMap<QString, QByteArray> result;
    for (const auto & mod : std::as_const(mods))
    {
        result.insert(mod, hashSingleFolder(mod, kHashFileFilter));
    }
    return result;
}

QMap<QString, QByteArray> Filesupport::getResourceFolderHashes()
{
    QMap<QString, QByteArray> result;
    result.insert(kResourceScriptsFolder, hashSingleFolder(kResourceScriptsFolder, kHashFileFilter));
    result.insert(kResourceAidataFolder, hashSingleFolder(kResourceAidataFolder, kHashFileFilter));
    return result;
}

namespace
{
    // reject codes
    constexpr qint32 kModSyncDisabled = 1;
    constexpr qint32 kModSyncUnknownMod = 2;
    constexpr qint32 kModSyncSizeCapExceeded = 3;
    constexpr qint32 kModSyncFileCountCapExceeded = 4;
    constexpr qint32 kModSyncInvalidPath = 5;
    constexpr qint32 kModSyncInternalError = 6;
    constexpr qint32 kModPathSegments = 2;
    constexpr qint32 kSpaceCodePoint = 0x20;
    constexpr qint32 kDelCodePoint = 0x7F;
    // QDataStream encodes a null QByteArray or QString length as 0xFFFFFFFFu.
    constexpr quint32 kQDataStreamNullSentinel = 0xFFFFFFFFu;
    // TCP/UDP port number ceiling (16-bit unsigned per RFC 793).
    constexpr qint32 kTcpPortMax = 0xFFFF;
    // Backup folder timestamp format yyyyMMdd-HHmmsszzzZ; the validator below cross-checks size against these.
    constexpr qint32 kBakTimestampLength = 19;
    constexpr qint32 kBakTimestampDateLength = 8;        // yyyyMMdd
    constexpr qint32 kBakTimestampDateTimeLength = 18;   // yyyyMMdd-HHmmsszzz
    // Same-second collision suffix is "-<digits>" appended; minimum total length is timestamp + separator + one digit.
    constexpr qint32 kBakCollisionCounterMinLength = 21;
    static_assert(kBakTimestampLength == kBakTimestampDateTimeLength + 1, "Z follows datetime");
    static_assert(kBakCollisionCounterMinLength == kBakTimestampLength + 2, "min counter = timestamp + '-' + 1 digit");
    // zlib best-speed; few percent worse ratio on text-heavy mods, big drop in host CPU stall.
    constexpr qint32 kQCompressBestSpeedLevel = 1;
    // qCompress prefixes a 4-byte big-endian uncompressed-size header used for pre-allocation; gate it before qUncompress to bound memory.
    constexpr qint32 kQCompressFramingHeaderBytes = 4;
    // UTF-8 encodes a Unicode code point in at most 4 bytes.
    constexpr qint32 kUtf8MaxBytesPerCodePoint = 4;
    // Cap raw bytes a pending mod-sync manifest may occupy on disk before being treated as corrupt.
    constexpr qint64 kMaxPendingManifestBytes = 1 * 1024 * 1024;
    // Cap raw bytes a rejoin manifest may occupy on disk before being treated as corrupt.
    constexpr qint64 kMaxRejoinManifestBytes = 4 * 1024;
    // Marker between mod folder name and PID during atomic swap; stageModSync, reapModSyncFolders, and the staging scanners must agree on the exact string.
    const auto kSyncStagingMarker = QStringLiteral(".sync-staging-");
    // Marker between mod folder name and timestamp on pre-swap backups; stageModSync, reapModSyncFolders, and matchBackupShape must agree on the exact string.
    const auto kBakMarker = QStringLiteral(".bak-");
    const auto kModsPrefix = QStringLiteral("mods/");
    // Pending-mod-sync and rejoin manifest JSON keys.
    const auto kJsonKeyVersion = QStringLiteral("version");
    const auto kJsonKeySwaps = QStringLiteral("swaps");
    const auto kJsonKeyStaging = QStringLiteral("staging");
    const auto kJsonKeyFinal = QStringLiteral("final");
    const auto kJsonKeyHost = QStringLiteral("host");
    const auto kJsonKeyPort = QStringLiteral("port");
    const auto kJsonKeyTimestamp = QStringLiteral("timestamp");
    constexpr qint32 kPendingManifestVersion = 1;
    constexpr qint32 kRejoinManifestVersion = 1;
    // Length of a Windows drive-letter prefix ("C:").
    constexpr qint32 kWindowsDrivePrefixLength = 2;
    // Staging dirs older than this are presumed orphaned by a dead process and reaped.
    constexpr qint64 kStaleStagingMaxAgeSecs = 3600;

    // Bounds of the documented COM<n>/LPT<n> digit suffix. https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file
    constexpr qint32 kDosDeviceMinDigit = 1;
    constexpr qint32 kDosDeviceMaxDigit = 9;

    QSet<QString> buildReservedDosDeviceNames()
    {
        QSet<QString> names = {
            QStringLiteral("CON"), QStringLiteral("PRN"),
            QStringLiteral("AUX"), QStringLiteral("NUL"),
            QStringLiteral("CONIN$"), QStringLiteral("CONOUT$"),
            QStringLiteral("COM\u00B9"), QStringLiteral("COM\u00B2"), QStringLiteral("COM\u00B3"),
            QStringLiteral("LPT\u00B9"), QStringLiteral("LPT\u00B2"), QStringLiteral("LPT\u00B3"),
        };
        for (qint32 i = kDosDeviceMinDigit; i <= kDosDeviceMaxDigit; ++i)
        {
            names.insert(QStringLiteral("COM") + QString::number(i));
            names.insert(QStringLiteral("LPT") + QString::number(i));
        }
        return names;
    }

    bool segmentClean(const QString & seg)
    {
        static const QSet<QChar> kInvalid = {
            QChar('<'), QChar('>'), QChar(':'), QChar('"'),
            QChar('|'), QChar('?'), QChar('*'),
        };
        if (seg.isEmpty() || seg == QStringLiteral(".") || seg == QStringLiteral(".."))
        {
            return false;
        }
        if (seg.endsWith(QChar('.')) || seg.endsWith(QChar(' ')))
        {
            return false;
        }
        for (const QChar c : seg)
        {
            if (c.unicode() < kSpaceCodePoint || c.unicode() == kDelCodePoint)
            {
                return false;
            }
            if (kInvalid.contains(c))
            {
                return false;
            }
        }
        static const QSet<QString> kReserved = buildReservedDosDeviceNames();
        const qint32 dotIdx = seg.indexOf(QChar('.'));
        const QString basename = (dotIdx >= 0) ? seg.left(dotIdx) : seg;
        if (kReserved.contains(basename.toUpper()))
        {
            return false;
        }
        return true;
    }

    QString joinPath(const QString & a, const QString & b)
    {
        if (a.isEmpty())
        {
            return b;
        }
        if (a.endsWith(QChar('/')))
        {
            return a + b;
        }
        return a + QChar('/') + b;
    }

    // Deliberately ASCII-only; QChar::isDigit also accepts non-ASCII Unicode digits.
    bool isAsciiDigit(QChar c)
    {
        return c >= QChar('0') && c <= QChar('9');
    }

    // QDataStream::operator>> pre-resizes to the declared length; the bounded readers reject the header before allocation.
    constexpr quint32 kInt32MaxAsU32 = 0x7FFFFFFFu;
    constexpr qint32 kBytesPerUtf16CodeUnit = 2;
    constexpr qint32 kBitsPerByte = 8;

    // Match only exact slice-2-generated shapes; substring matching would catch legitimate mod folder names.
    bool matchStagingShape(const QString & name, QString & outPrefix)
    {
        const qint32 idx = name.lastIndexOf(kSyncStagingMarker);
        if (idx <= 0)
        {
            return false;
        }
        const QString prefix = name.left(idx);
        const QString suffix = name.mid(idx + kSyncStagingMarker.size());
        if (suffix.isEmpty() || !Filesupport::validateModPath(kModsPrefix + prefix))
        {
            return false;
        }
        for (const QChar c : suffix)
        {
            if (!isAsciiDigit(c))
            {
                return false;
            }
        }
        outPrefix = prefix;
        return true;
    }

    bool matchBackupShape(const QString & name, QString & outPrefix)
    {
        const qint32 idx = name.lastIndexOf(kBakMarker);
        if (idx <= 0)
        {
            return false;
        }
        const QString prefix = name.left(idx);
        const QString suffix = name.mid(idx + kBakMarker.size());
        if (!Filesupport::validateModPath(kModsPrefix + prefix))
        {
            return false;
        }
        // Generated suffix is exactly yyyyMMdd-HHmmsszzzZ with optional -<digits> collision counter.
        if (suffix.size() < kBakTimestampLength)
        {
            return false;
        }
        for (qint32 i = 0; i < kBakTimestampDateLength; ++i)
        {
            if (!isAsciiDigit(suffix[i])) return false;
        }
        if (suffix[kBakTimestampDateLength] != QChar('-')) return false;
        for (qint32 i = kBakTimestampDateLength + 1; i < kBakTimestampDateTimeLength; ++i)
        {
            if (!isAsciiDigit(suffix[i])) return false;
        }
        if (suffix[kBakTimestampDateTimeLength] != QChar('Z')) return false;
        if (suffix.size() > kBakTimestampLength)
        {
            // Collision-counter form requires `-<digits>` after the timestamp; bare `-` is invalid.
            if (suffix[kBakTimestampLength] != QChar('-') || suffix.size() < kBakCollisionCounterMinLength) return false;
            for (qint32 i = kBakTimestampLength + 1; i < suffix.size(); ++i)
            {
                if (!isAsciiDigit(suffix[i])) return false;
            }
        }
        outPrefix = prefix;
        return true;
    }
}

bool Filesupport::validateModPath(const QString & modPath, qint32 maxLen)
{
    if (modPath.isEmpty() || modPath.size() > maxLen)
    {
        return false;
    }
    if (!modPath.startsWith(kModsPrefix))
    {
        return false;
    }
    if (modPath.contains(QChar('\\')))
    {
        return false;
    }
    const QStringList segments = modPath.split(QChar('/'));
    if (segments.size() != kModPathSegments)
    {
        return false;
    }
    for (const auto & seg : segments)
    {
        if (!segmentClean(seg))
        {
            return false;
        }
    }
    
    return true;
}

void Filesupport::writeByteArray(QDataStream& stream, const QByteArray& array)
{
    stream << static_cast<qint32>(array.size());
    for (qint32 i = 0; i < array.size(); i++)
    {
        stream << static_cast<qint8>(array[i]);
    }
}

void Filesupport::writeBytes(QDataStream& stream, const QByteArray& array)
{
    for (qint32 i = 0; i < array.size(); i++)
    {
        stream << static_cast<qint8>(array[i]);
    }
}

QByteArray Filesupport::readByteArray(QDataStream& stream)
{
    QByteArray array;
    qint32 size = 0;
    stream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        qint8 value = 0;
        stream >> value;
        array.append(value);
    }
    return array;
}

void Filesupport::storeList(const QString & file, const QStringList & items, const QString & folder)
{
    QDir dir(folder);
    dir.mkpath(".");
    QFile dataFile(folder + file + LIST_FILENAME_ENDING);
    dataFile.open(QIODevice::WriteOnly);
    QDataStream stream(&dataFile);
    stream.setVersion(QDataStream::Version::Qt_6_5);
    stream << file;
    stream << static_cast<qint32>(items.size());
    for (qint32 i = 0; i < items.size(); i++)
    {
        stream << items[i];
    }
}

Filesupport::StringList Filesupport::readList(const QString & file, const QString & folder)
{
    return readList(folder + file);
}

Filesupport::StringList Filesupport::readList(const QString & file)
{
    QFile dataFile(file);
    StringList ret;
    if (dataFile.exists())
    {
        dataFile.open(QIODevice::ReadOnly);
        QDataStream stream(&dataFile);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        stream >> ret.name;
        qint32 size = 0;
        stream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            QString name;
            stream >> name;
            ret.items.append(name);
        }
    }
    else
    {
        CONSOLE_PRINT("Unable to open file: " + file + " using empty list", GameConsole::eWARNING);
    }
    return ret;
}


bool Filesupport::validateRelativeFilePath(const QString & relPath, qint32 maxLen)
{
    if (relPath.isEmpty() || relPath.size() > maxLen)
    {
        return false;
    }
    if (relPath.startsWith(QChar('/')) || relPath.contains(QChar('\\')))
    {
        return false;
    }
    if (relPath.size() >= kWindowsDrivePrefixLength && relPath[1] == QChar(':'))
    {
        return false;
    }
    const QStringList segments = relPath.split(QChar('/'));
    for (const auto & seg : segments)
    {
        if (!segmentClean(seg))
        {
            return false;
        }
    }
    return true;
}

Filesupport::ModSyncPackage Filesupport::buildModSyncPackage(const QString & installRoot, const QString & modPath, const ModSyncCaps & caps)
{
    ModSyncPackage pkg;
    if (!validateModPath(modPath))
    {
        pkg.rejectReason = kModSyncInvalidPath;
        return pkg;
    }
    const QString modRoot = joinPath(installRoot, modPath);
    QDir modDir(modRoot);
    if (!modDir.exists())
    {
        pkg.rejectReason = kModSyncUnknownMod;
        return pkg;
    }
    // Resolve to an absolute root once; QDir::relativeFilePath short-circuits to verbatim when either operand is relative.
    const QDir absModDir(modDir.absolutePath());
    QMap<QString, QByteArray> files;
    qint64 uncompressedTotal = 0;
    qint32 fileCount = 0;
    QDirIterator it(modRoot, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        const QString absolute = QFileInfo(it.next()).absoluteFilePath();
        const QString rel = absModDir.relativeFilePath(absolute);
        const QStringList relSegs = rel.split(QChar('/'));
        const QString basename = relSegs.isEmpty() ? rel : relSegs.last();
        // Segment-based filter; substrings would false-positive legit filenames containing .bak- or .sync-staging-.
        bool cruft = false;
        for (qint32 si = 0; si < relSegs.size(); ++si)
        {
            const QString & seg = relSegs[si];
            if (seg == QStringLiteral(".git") || seg == QStringLiteral(".svn") || seg == QStringLiteral("__pycache__"))
            {
                cruft = true;
                break;
            }
            const bool isDir = (si + 1 < relSegs.size());
            if (isDir && (seg.startsWith(kSyncStagingMarker) || seg.startsWith(kBakMarker)))
            {
                cruft = true;
                break;
            }
        }
        if (cruft)
        {
            continue;
        }
        if (basename == QStringLiteral(".DS_Store") || basename == QStringLiteral("Thumbs.db") || basename == QStringLiteral("desktop.ini"))
        {
            continue;
        }
        if (!validateRelativeFilePath(rel, caps.relPathMaxLen))
        {
            pkg.rejectReason = kModSyncInvalidPath;
            return pkg;
        }
        QFile f(absolute);
        if (!f.open(QIODevice::ReadOnly))
        {
            pkg.rejectReason = kModSyncInternalError;
            return pkg;
        }
        const qint64 size = f.size();
        if (size > caps.perModBytes)
        {
            pkg.rejectReason = kModSyncSizeCapExceeded;
            return pkg;
        }
        uncompressedTotal += size;
        if (uncompressedTotal > caps.perModBytes)
        {
            pkg.rejectReason = kModSyncSizeCapExceeded;
            return pkg;
        }
        ++fileCount;
        if (fileCount > caps.fileCountMax)
        {
            pkg.rejectReason = kModSyncFileCountCapExceeded;
            return pkg;
        }
        files.insert(rel, f.readAll());
        f.close();
    }
    QByteArray serialized;
    {
        QDataStream stream(&serialized, QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        // Custom format: keys as UTF-8 QByteArrays so the receiver can run a length-bounded reader.
        stream << static_cast<qint32>(files.size());
        for (auto iter = files.constBegin(); iter != files.constEnd(); ++iter)
        {
            stream << iter.key().toUtf8();
            stream << iter.value();
        }
    }
    if (serialized.size() > caps.perModBytes || serialized.size() > std::numeric_limits<qint32>::max())
    {
        pkg.rejectReason = kModSyncSizeCapExceeded;
        return pkg;
    }
    pkg.declaredUncompressedSize = static_cast<qint32>(serialized.size());
    pkg.compressedBlob = qCompress(serialized, kQCompressBestSpeedLevel);
    pkg.fileCount = fileCount;
    if (pkg.compressedBlob.size() > caps.perModBytes)
    {
        pkg.rejectReason = kModSyncSizeCapExceeded;
        pkg.compressedBlob.clear();
        return pkg;
    }
    return pkg;
}

bool Filesupport::readBoundedBytes(QDataStream & stream, QByteArray & out, qint64 maxBytes)
{
    if (maxBytes < 0)
    {
        return false;
    }
    quint32 declared = 0;
    stream >> declared;
    if (stream.status() != QDataStream::Ok)
    {
        return false;
    }
    if (declared == kQDataStreamNullSentinel)
    {
        out.clear();
        return true;
    }
    if (declared > kInt32MaxAsU32 || static_cast<qint64>(declared) > maxBytes)
    {
        return false;
    }
    out.resize(static_cast<qint32>(declared));
    if (out.size() > 0 && stream.readRawData(out.data(), out.size()) != out.size())
    {
        return false;
    }
    return true;
}

bool Filesupport::readBoundedString(QDataStream & stream, QString & out, qint32 maxChars)
{
    if (maxChars < 0)
    {
        return false;
    }
    quint32 declared = 0;
    stream >> declared;
    if (stream.status() != QDataStream::Ok)
    {
        return false;
    }
    if (declared == kQDataStreamNullSentinel)
    {
        out.clear();
        return true;
    }
    const qint64 maxBytes = static_cast<qint64>(maxChars) * kBytesPerUtf16CodeUnit;
    if (declared > kInt32MaxAsU32 || static_cast<qint64>(declared) > maxBytes || (declared % kBytesPerUtf16CodeUnit) != 0)
    {
        return false;
    }
    QByteArray buf;
    buf.resize(static_cast<qint32>(declared));
    if (buf.size() > 0 && stream.readRawData(buf.data(), buf.size()) != buf.size())
    {
        return false;
    }
    const qint32 codeUnits = buf.size() / kBytesPerUtf16CodeUnit;
    out.resize(codeUnits);
    const uchar * src = reinterpret_cast<const uchar *>(buf.constData());
    // Wire is big-endian; build each code unit explicitly to skip platform-endian conversion in fromUtf16.
    for (qint32 i = 0; i < codeUnits; ++i)
    {
        out[i] = QChar(static_cast<ushort>((src[i * kBytesPerUtf16CodeUnit] << kBitsPerByte) | src[i * kBytesPerUtf16CodeUnit + 1]));
    }
    return true;
}

QMap<QString, QByteArray> Filesupport::extractModSyncPackage(const QByteArray & compressedBlob, qint32 declaredUncompressedSize, const ModSyncCaps & caps, qint32 & rejectReason)
{
    rejectReason = 0;
    QMap<QString, QByteArray> files;
    if (caps.perModBytes < 0 || caps.fileCountMax < 0 || caps.relPathMaxLen <= 0)
    {
        rejectReason = kModSyncInternalError;
        return files;
    }
    if (compressedBlob.size() > caps.perModBytes)
    {
        rejectReason = kModSyncSizeCapExceeded;
        return files;
    }
    if (declaredUncompressedSize <= 0 || declaredUncompressedSize > caps.perModBytes)
    {
        rejectReason = kModSyncSizeCapExceeded;
        return files;
    }
    if (compressedBlob.size() < kQCompressFramingHeaderBytes)
    {
        rejectReason = kModSyncInternalError;
        return files;
    }
    const quint32 framingSize = qFromBigEndian<quint32>(reinterpret_cast<const uchar *>(compressedBlob.constData()));
    if (framingSize > static_cast<quint32>(caps.perModBytes) || static_cast<qint32>(framingSize) != declaredUncompressedSize)
    {
        rejectReason = kModSyncSizeCapExceeded;
        return files;
    }
    const QByteArray serialized = qUncompress(compressedBlob);
    if (serialized.isEmpty() || serialized.size() != declaredUncompressedSize)
    {
        rejectReason = kModSyncInternalError;
        return files;
    }
    QDataStream stream(serialized);
    stream.setVersion(QDataStream::Version::Qt_6_5);
    qint32 mapSize = 0;
    stream >> mapSize;
    if (stream.status() != QDataStream::Ok || mapSize < 0 || mapSize > caps.fileCountMax)
    {
        rejectReason = kModSyncFileCountCapExceeded;
        return QMap<QString, QByteArray>();
    }
    qint64 uncompressedTotal = 0;
    for (qint32 i = 0; i < mapSize; ++i)
    {
        QByteArray keyUtf8;
        QByteArray value;
        if (!readBoundedBytes(stream, keyUtf8, static_cast<qint64>(caps.relPathMaxLen) * kUtf8MaxBytesPerCodePoint))
        {
            rejectReason = kModSyncInvalidPath;
            return QMap<QString, QByteArray>();
        }
        if (!readBoundedBytes(stream, value, caps.perModBytes))
        {
            rejectReason = kModSyncSizeCapExceeded;
            return QMap<QString, QByteArray>();
        }
        const QString key = QString::fromUtf8(keyUtf8);
        if (!validateRelativeFilePath(key, caps.relPathMaxLen))
        {
            rejectReason = kModSyncInvalidPath;
            return QMap<QString, QByteArray>();
        }
        uncompressedTotal += value.size();
        if (uncompressedTotal > caps.perModBytes)
        {
            rejectReason = kModSyncSizeCapExceeded;
            return QMap<QString, QByteArray>();
        }
        if (files.contains(key))
        {
            rejectReason = kModSyncInvalidPath;
            return QMap<QString, QByteArray>();
        }
        files.insert(key, value);
    }
    return files;
}

QString Filesupport::stageModSync(const QString & installRoot, const QString & modPath, const QMap<QString, QByteArray> & files, const ModSyncCaps & caps, qint32 & rejectReason)
{
    rejectReason = 0;
    if (!validateModPath(modPath))
    {
        rejectReason = kModSyncInvalidPath;
        return QString();
    }
    if (files.size() > caps.fileCountMax)
    {
        rejectReason = kModSyncFileCountCapExceeded;
        return QString();
    }
    qint64 totalBytes = 0;
    for (auto iter = files.constBegin(); iter != files.constEnd(); ++iter)
    {
        if (!validateRelativeFilePath(iter.key(), caps.relPathMaxLen))
        {
            rejectReason = kModSyncInvalidPath;
            return QString();
        }
        if (iter.value().size() > caps.perModBytes)
        {
            rejectReason = kModSyncSizeCapExceeded;
            return QString();
        }
        totalBytes += iter.value().size();
        if (totalBytes > caps.perModBytes)
        {
            rejectReason = kModSyncSizeCapExceeded;
            return QString();
        }
    }
    const qint64 pid = QCoreApplication::applicationPid();
    const QString stagingRel = modPath + kSyncStagingMarker + QString::number(pid);
    const QString stagingAbs = joinPath(installRoot, stagingRel);
    QDir stagingDir(stagingAbs);
    if (stagingDir.exists() && !stagingDir.removeRecursively())
    {
        rejectReason = kModSyncInternalError;
        return QString();
    }
    if (!QDir().mkpath(stagingAbs))
    {
        rejectReason = kModSyncInternalError;
        return QString();
    }
    for (auto iter = files.constBegin(); iter != files.constEnd(); ++iter)
    {
        const QString full = joinPath(stagingAbs, iter.key());
        const QFileInfo fi(full);
        if (!QDir().mkpath(fi.absolutePath()))
        {
            QDir(stagingAbs).removeRecursively();
            rejectReason = kModSyncInternalError;
            return QString();
        }
        QSaveFile f(full);
        if (!f.open(QIODevice::WriteOnly))
        {
            QDir(stagingAbs).removeRecursively();
            rejectReason = kModSyncInternalError;
            return QString();
        }
        f.write(iter.value());
        if (!f.commit())
        {
            QDir(stagingAbs).removeRecursively();
            rejectReason = kModSyncInternalError;
            return QString();
        }
    }
    return stagingRel;
}

void Filesupport::reapModSyncFolders(const QString & installRoot, qint32 backupKeep)
{
    const QString modsRoot = joinPath(installRoot, QStringLiteral("mods"));
    QDir modsDir(modsRoot);
    if (!modsDir.exists())
    {
        return;
    }
    const auto entries = modsDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    QMap<QString, QList<QFileInfo>> backupsByMod;
    const QDateTime cutoff = QDateTime::currentDateTime().addSecs(-kStaleStagingMaxAgeSecs);
    for (const auto & entry : entries)
    {
        const QString name = entry.fileName();
        QString prefix;
        if (matchStagingShape(name, prefix))
        {
            // Mtime fallback heuristic; cheap, no platform-specific PID liveness check.
            if (entry.lastModified() < cutoff)
            {
                CONSOLE_PRINT("Reaping stale staging dir: " + entry.absoluteFilePath(), GameConsole::eINFO);
                QDir(entry.absoluteFilePath()).removeRecursively();
            }
            continue;
        }
        if (matchBackupShape(name, prefix))
        {
            backupsByMod[prefix].append(entry);
        }
    }
    for (auto iter = backupsByMod.begin(); iter != backupsByMod.end(); ++iter)
    {
        auto & list = iter.value();
        if (list.size() <= backupKeep)
        {
            continue;
        }
        std::sort(list.begin(), list.end(), [](const QFileInfo & a, const QFileInfo & b)
        {
            return a.lastModified() > b.lastModified();
        });
        for (qint32 i = backupKeep; i < list.size(); ++i)
        {
            CONSOLE_PRINT("Pruning old mod-sync backup: " + list[i].absoluteFilePath(), GameConsole::eINFO);
            QDir(list[i].absoluteFilePath()).removeRecursively();
        }
    }
}

QString Filesupport::pendingModSyncManifestPath(const QString & userDataPath)
{
    return joinPath(userDataPath, QStringLiteral(".pending-mod-sync.json"));
}

bool Filesupport::writePendingModSyncManifest(const QString & userDataPath, const QList<QPair<QString, QString>> & swaps)
{
    QJsonArray jsonSwaps;
    for (const auto & pair : swaps)
    {
        QJsonObject entry;
        entry.insert(kJsonKeyStaging, pair.first);
        entry.insert(kJsonKeyFinal, pair.second);
        jsonSwaps.append(entry);
    }
    QJsonObject root;
    root.insert(kJsonKeyVersion, kPendingManifestVersion);
    root.insert(kJsonKeySwaps, jsonSwaps);
    const QString path = pendingModSyncManifestPath(userDataPath);
    QSaveFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        CONSOLE_PRINT("Failed to open pending mod-sync manifest for write: " + path, GameConsole::eERROR);
        return false;
    }
    f.write(QJsonDocument(root).toJson(QJsonDocument::Compact));
    if (!f.commit())
    {
        CONSOLE_PRINT("Failed to commit pending mod-sync manifest: " + path, GameConsole::eERROR);
        return false;
    }
    return true;
}

QStringList Filesupport::executePendingModSyncManifest(const QString & installRoot, const QString & userDataPath)
{
    QStringList applied;
    const QString path = pendingModSyncManifestPath(userDataPath);
    QFile f(path);
    if (!f.exists())
    {
        return applied;
    }
    // Oversize manifest is treated as corrupt or tampered: discard rather than try to parse partial state.
    if (f.size() > kMaxPendingManifestBytes)
    {
        CONSOLE_PRINT("Pending mod-sync manifest oversize, discarding: " + path, GameConsole::eERROR);
        QFile::remove(path);
        return applied;
    }
    if (!f.open(QIODevice::ReadOnly))
    {
        CONSOLE_PRINT("Pending mod-sync manifest exists but cannot be read: " + path, GameConsole::eERROR);
        return applied;
    }
    const QByteArray data = f.readAll();
    f.close();
    QJsonParseError parseErr;
    const QJsonDocument doc = QJsonDocument::fromJson(data, &parseErr);
    if (parseErr.error != QJsonParseError::NoError || !doc.isObject())
    {
        CONSOLE_PRINT("Pending mod-sync manifest is invalid JSON: " + parseErr.errorString(), GameConsole::eERROR);
        QFile::remove(path);
        return applied;
    }
    const QJsonObject root = doc.object();
    if (root.value(kJsonKeyVersion).toInt(0) != kPendingManifestVersion)
    {
        CONSOLE_PRINT("Pending mod-sync manifest has unknown version, discarding", GameConsole::eERROR);
        QFile::remove(path);
        return applied;
    }
    const QJsonArray swaps = root.value(kJsonKeySwaps).toArray();
    // UTC + ms keeps backup names lexically sorted across DST and avoids same-second collision on rapid retries.
    const QString isoStamp = QDateTime::currentDateTimeUtc().toString(QStringLiteral("yyyyMMdd-HHmmsszzzZ"));
    // Read the flag once before the swap loop so settings cannot drift mid-batch. Must be called after Settings::loadSettings has run its binding loop; today the boot order at settings.cpp:1557 satisfies that.
    const bool keepBackups = Settings::getInstance()->getModSyncKeepBackups();
    for (const auto & v : swaps)
    {
        const QJsonObject entry = v.toObject();
        const QString stagingRel = entry.value(kJsonKeyStaging).toString();
        const QString finalRel = entry.value(kJsonKeyFinal).toString();
        if (!validateModPath(finalRel))
        {
            CONSOLE_PRINT("Manifest entry has invalid final path, skipping: " + finalRel, GameConsole::eERROR);
            continue;
        }
        // Pin staging to the exact shape stageModSync writes: <finalRel>.sync-staging-<digits>.
        const QString stagingPrefix = finalRel + kSyncStagingMarker;
        if (!stagingRel.startsWith(stagingPrefix))
        {
            CONSOLE_PRINT("Manifest staging path shape mismatch, skipping: " + stagingRel, GameConsole::eERROR);
            continue;
        }
        const QString suffix = stagingRel.mid(stagingPrefix.size());
        bool suffixDigits = !suffix.isEmpty();
        for (const QChar c : suffix)
        {
            if (!isAsciiDigit(c))
            {
                suffixDigits = false;
                break;
            }
        }
        if (!suffixDigits)
        {
            CONSOLE_PRINT("Manifest staging suffix not numeric pid, skipping: " + stagingRel, GameConsole::eERROR);
            continue;
        }
        const QString stagingAbs = joinPath(installRoot, stagingRel);
        const QString finalAbs = joinPath(installRoot, finalRel);
        const QFileInfo stagingInfo(stagingAbs);
        if (!stagingInfo.exists())
        {
            CONSOLE_PRINT("Manifest staging missing, skipping: " + stagingAbs, GameConsole::eERROR);
            continue;
        }
        // Defends against a tampered manifest pointing the staging slot at a regular file.
        if (!stagingInfo.isDir())
        {
            CONSOLE_PRINT("Manifest staging path is not a directory, skipping: " + stagingAbs, GameConsole::eERROR);
            continue;
        }
        QString backupAbs;
        const QFileInfo finalInfo(finalAbs);
        const bool finalExisted = finalInfo.exists();
        if (finalExisted)
        {
            // Symmetric guard with stagingInfo.isDir(); skip the swap if a regular file occupies the mod slot.
            if (!finalInfo.isDir())
            {
                CONSOLE_PRINT("Manifest final path is not a directory, skipping: " + finalAbs, GameConsole::eERROR);
                continue;
            }
            // Loop suffix defends against duplicate manifest entries for the same finalRel sharing one isoStamp.
            backupAbs = finalAbs + kBakMarker + isoStamp;
            for (qint32 n = 1; QFileInfo::exists(backupAbs); ++n)
            {
                backupAbs = finalAbs + kBakMarker + isoStamp + QStringLiteral("-") + QString::number(n);
            }
            if (!QDir().rename(finalAbs, backupAbs))
            {
                CONSOLE_PRINT("Failed to back up existing mod folder: " + finalAbs + " -> " + backupAbs, GameConsole::eERROR);
                continue;
            }
        }
        if (!QDir().rename(stagingAbs, finalAbs))
        {
            CONSOLE_PRINT("Failed to swap staging into place: " + stagingAbs + " -> " + finalAbs, GameConsole::eERROR);
            // Roll the backup back so the active mod folder is not left missing.
            if (finalExisted && !QDir().rename(backupAbs, finalAbs))
            {
                CONSOLE_PRINT("CRITICAL: rollback also failed; mod folder is at " + backupAbs, GameConsole::eERROR);
            }
            continue;
        }
        CONSOLE_PRINT("Mod sync applied: " + finalRel, GameConsole::eINFO);
        applied.append(finalRel);
        // Backup retention is opt-in; remove the .bak directory now if disabled. Failure to remove just leaves the dir for the next reapModSyncFolders pass.
        if (finalExisted && !keepBackups && !backupAbs.isEmpty())
        {
            if (!QDir(backupAbs).removeRecursively())
            {
                CONSOLE_PRINT("Failed to remove .bak after successful sync; reaper will clean up: " + backupAbs, GameConsole::eWARNING);
            }
        }
    }
    QFile::remove(path);
    return applied;
}

QString Filesupport::rejoinManifestPath(const QString & userDataPath)
{
    return joinPath(userDataPath, QStringLiteral(".rejoin.json"));
}

bool Filesupport::writeRejoinManifest(const QString & userDataPath, const QString & host, quint16 port)
{
    if (host.isEmpty() || port == 0)
    {
        return false;
    }
    QJsonObject root;
    root.insert(kJsonKeyVersion, kRejoinManifestVersion);
    root.insert(kJsonKeyHost, host);
    root.insert(kJsonKeyPort, static_cast<qint32>(port));
    root.insert(kJsonKeyTimestamp, QDateTime::currentSecsSinceEpoch());
    const QString path = rejoinManifestPath(userDataPath);
    QSaveFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        CONSOLE_PRINT("Failed to open rejoin manifest for write: " + path, GameConsole::eERROR);
        return false;
    }
    f.write(QJsonDocument(root).toJson(QJsonDocument::Compact));
    if (!f.commit())
    {
        CONSOLE_PRINT("Failed to commit rejoin manifest: " + path, GameConsole::eERROR);
        return false;
    }
    return true;
}

Filesupport::RejoinManifest Filesupport::consumeRejoinManifest(const QString & userDataPath)
{
    RejoinManifest result;
    const QString path = rejoinManifestPath(userDataPath);
    QFile f(path);
    if (!f.exists())
    {
        return result;
    }
    if (f.size() > kMaxRejoinManifestBytes)
    {
        CONSOLE_PRINT("Rejoin manifest oversize, discarding: " + path, GameConsole::eERROR);
        QFile::remove(path);
        return result;
    }
    if (!f.open(QIODevice::ReadOnly))
    {
        CONSOLE_PRINT("Rejoin manifest exists but cannot be read: " + path, GameConsole::eERROR);
        QFile::remove(path);
        return result;
    }
    const QByteArray data = f.readAll();
    f.close();
    QFile::remove(path);
    QJsonParseError parseErr;
    const QJsonDocument doc = QJsonDocument::fromJson(data, &parseErr);
    if (parseErr.error != QJsonParseError::NoError || !doc.isObject())
    {
        CONSOLE_PRINT("Rejoin manifest invalid JSON: " + parseErr.errorString(), GameConsole::eERROR);
        return result;
    }
    const QJsonObject root = doc.object();
    if (root.value(kJsonKeyVersion).toInt(0) != kRejoinManifestVersion)
    {
        CONSOLE_PRINT("Rejoin manifest unknown version, discarding", GameConsole::eERROR);
        return result;
    }
    const QString host = root.value(kJsonKeyHost).toString();
    const qint32 port = root.value(kJsonKeyPort).toInt(0);
    const qint64 timestamp = root.value(kJsonKeyTimestamp).toVariant().toLongLong();
    if (host.isEmpty() || port <= 0 || port > kTcpPortMax)
    {
        CONSOLE_PRINT("Rejoin manifest has invalid host or port, discarding", GameConsole::eERROR);
        return result;
    }
    result.valid = true;
    result.host = host;
    result.port = static_cast<quint16>(port);
    result.timestamp = timestamp;
    return result;
}
