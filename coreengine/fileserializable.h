#ifndef FILESERIALIZABLE_H
#define FILESERIALIZABLE_H

#include <QDataStream>

class FileSerializable
{
public:
    FileSerializable() = default;
    ~FileSerializable() = default;
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serialize(QDataStream& pStream) = 0;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserialize(QDataStream& pStream) = 0;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() = 0;
};

#endif // FILESERIALIZABLE_H
