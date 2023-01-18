#ifndef FILESERIALIZABLE_H
#define FILESERIALIZABLE_H

#include <QDataStream>

class FileSerializable
{
public:
    explicit FileSerializable() = default;
    virtual ~FileSerializable() = default;
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& stream) const = 0;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& stream) = 0;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() const = 0;
};

#endif // FILESERIALIZABLE_H
