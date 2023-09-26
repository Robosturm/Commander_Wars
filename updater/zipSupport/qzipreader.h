#pragma once

#include <QtCore/qglobal.h>

#include <QDateTime>
#include <QFile>
#include <QString>
#include <QObject>

class QZipReaderPrivate;

class QZipReader final : public QObject
{
    Q_OBJECT
public:
    explicit QZipReader(const QString &fileName, QIODevice::OpenMode mode = QIODevice::ReadOnly );

    explicit QZipReader(QIODevice *device);
    virtual ~QZipReader();

    QIODevice* device() const;

    bool isReadable() const;
    bool exists() const;

    struct FileInfo
    {
        FileInfo() noexcept
            : isDir(false), isFile(false), isSymLink(false), crc(0), size(0)
        {}

        bool isValid() const noexcept { return isDir || isFile || isSymLink; }

        QString filePath;
        uint isDir : 1;
        uint isFile : 1;
        uint isSymLink : 1;
        QFile::Permissions permissions;
        uint crc;
        qint64 size;
        QDateTime lastModified;
    };

    QList<FileInfo> fileInfoList() const;
    int count() const;

    FileInfo entryInfoAt(int index) const;
    QByteArray fileData(const QString &fileName) const;
    bool extractAll(const QString &destinationDir);

    enum Status {
        NoError,
        FileReadError,
        FileOpenError,
        FilePermissionsError,
        FileError
    };

    Status status() const;

    void close();

signals:
    void sigExtractProgress(qint32 progress);

private:
    QZipReaderPrivate *d;
    Q_DISABLE_COPY_MOVE(QZipReader)
};
