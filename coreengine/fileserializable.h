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
    virtual void serializeObject(QDataStream& pStream) = 0;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) = 0;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() = 0;
};

#endif // FILESERIALIZABLE_H
