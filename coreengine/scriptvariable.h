#ifndef SCRIPTVARIABLE_H
#define SCRIPTVARIABLE_H

#include <QObject>
#include <QBuffer>
#include <QDataStream>
#include <QVector>
#include <QPoint>
#include "fileserializable.h"

#include "oxygine-framework.h"

class ScriptVariable;
typedef oxygine::intrusive_ptr<ScriptVariable> spScriptVariable;

class ScriptVariable : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit ScriptVariable(QString id);
    explicit ScriptVariable();
    virtual ~ScriptVariable() = default;

    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    inline virtual qint32 getVersion() override
    {
        return 1;
    }
    /**
     * @brief writes a vector of the given type
     */
    template<typename VectorType>
    void writeVector(QVector<VectorType> vector)
    {
        buffer.seek(0);
        actionData << static_cast<qint32>(vector.size());
        for (qint32 i = 0; i < vector.size(); i++)
        {
            actionData << vector[i];
        }
    }
    /**
     * @brief reads a vector of the given type
     */
    template<typename VectorType>
    QVector<VectorType> readVector()
    {
        buffer.seek(0);
        QVector<VectorType> ret;
        if (buffer.size() > 0)
        {
            qint32 size = 0;
            actionData >> size;
            for (qint32 i = 0; i < size; i++)
            {
                VectorType type;
                actionData >> type;
                ret.append(type);
            }
        }
        return ret;
    }
    template<typename type>
    /**
     * @brief writeData writes a data from the action data
     * @param data
     */
    void writeData(type data)
    {
        buffer.seek(0);
        actionData << data;
    }
    /**
     * @brief readData
     * @return reads a data from the action data
     */
    template<typename type>
    qint32 readData()
    {
        buffer.seek(0);
        type data = 0;
        if (buffer.size() > 0)
        {
            actionData >> data;
        }
        return data;
    }
public slots:
    inline QString getId()
    {
        return m_Id;
    }
    /**
     * @brief writeDataVectorPoint
     * @param data writes a Qector<QPoint> to the action data
     */
    void writeDataVectorPoint(QVector<QPoint> data)
    {
        writeVector(data);
    }
    /**
     * @brief readDataVectorPoint
     * @return reads a QVector<QPoint> from the action data
     */
    QVector<QPoint> readDataVectorPoint()
    {
        return readVector<QPoint>();
    }
    /**
     * @brief writeDataVectorUint32
     * @param data writes a QVector<quint32> to the action data
     */
    void writeDataVectorUint32(QVector<quint32> data)
    {
        writeVector(data);
    }
    /**
     * @brief readDataVectorUint32
     * @return reads a QVector<quint32> from the action data
     */
    QVector<quint32> readDataVectorUint32()
    {
        return readVector<quint32>();
    }
    /**
     * @brief writeDataString adds a string to the action data
     * @param data
     */
    void writeDataString(QString data)
    {
        buffer.seek(0);
        actionData << data;
    }
    /**
     * @brief readDataString
     * @return reads a string from the action data
     */
    QString readDataString()
    {
        buffer.seek(0);
        QString data;
        if (buffer.size() > 0)
        {
            actionData >> data;
        }
        return data;
    }
    /**
     * @brief writeDataInt32 adds a int32 to the action data
     * @param data
     */
    void writeDataInt32(qint32 data)
    {
        writeData(data);
    }
    /**
     * @brief readDataInt32
     * @return reads a int32 from the action data
     */
    qint32 readDataInt32()
    {
        return readData<qint32>();
    }
    /**
     * @brief writeDataUint64 adds a uint64 to the action data
     * @param data
     */
    void writeDataUint32(quint32 data)
    {
        writeData(data);
    }
    /**
     * @brief readDataUint32
     * @return reads a quint32 from the action data
     */
    quint32 readDataUint32()
    {
        return readData<quint32>();
    }
    /**
     * @brief writeDataFloat adds a float to the action data
     * @param data
     */
    void writeDataFloat(float data)
    {
        writeData(data);
    }
    /**
     * @brief readDataFloat
     * @return reads a float from the action data
     */
    float readDataFloat()
    {
        return readData<float>();
    }
    /**
     * @brief writeDataFloat adds a float to the action data
     * @param data
     */
    void writeDataBool(bool data)
    {
        writeData(data);
    }
    /**
     * @brief readDataBool
     * @return reads a float from the action data
     */
    bool readDataBool()
    {
        return readData<bool>();
    }
private:
    QString m_Id;
    /**
     * @brief actionData data needed to perform this action
     */
    QBuffer buffer;
    QDataStream actionData{&buffer};
};

#endif // SCRIPTVARIABLE_H
