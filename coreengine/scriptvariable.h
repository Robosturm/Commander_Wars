#ifndef SCRIPTVARIABLE_H
#define SCRIPTVARIABLE_H

#include <QObject>
#include <QBuffer>
#include <QDataStream>
#include <QVector>
#include <QPoint>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "coreengine/fileserializable.h"

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
    virtual void serializeObject(QDataStream& pStream) const override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    inline virtual qint32 getVersion() const override
    {
        return 1;
    }
    /**
     * @brief writes a vector of the given type
     */
    template<typename ListType>
    void writeList(QList <ListType> list)
    {
        // remove the buffer content
        m_buffer.close();
        m_buffer.setData(QByteArray());
        m_buffer.open(QIODevice::ReadWrite);
        m_buffer.seek(0);
        m_actionData << static_cast<qint32>(list.size());
        for (qint32 i = 0; i < list.size(); i++)
        {
            m_actionData << list[i];
        }
    }
    /**
     * @brief reads a vector of the given type
     */
    template<typename ListType>
    QList<ListType> readList()
    {
        m_buffer.seek(0);
        QList<ListType> ret;
        if (m_buffer.size() > 0)
        {
            qint32 size = 0;
            m_actionData >> size;
            for (qint32 i = 0; i < size; i++)
            {
                ListType type;
                m_actionData >> type;
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
        // remove the buffer content
        m_buffer.close();
        m_buffer.setData(QByteArray());
        m_buffer.open(QIODevice::ReadWrite);
        m_buffer.seek(0);
        m_actionData << data;
    }
    /**
     * @brief readData
     * @return reads a data from the action data
     */
    template<typename type>
    type readData()
    {
        m_buffer.seek(0);
        type data = 0;
        if (m_buffer.size() > 0)
        {
            m_actionData >> data;
        }
        return data;
    }

public slots:
    inline QString getId()
    {
        return m_Id;
    }
    /**
     * @brief writeDataListInt32
     * @param data writes a QList<qint32> to the action data
     */
    void writeDataListInt32(QList<qint32> data)
    {
        writeList(data);
    }
    /**
     * @brief readDataListInt
     * @return reads a QList<int> from the action data
     */
    QList<int> readDataListInt32()
    {
        return readList<qint32>();
    }
    /**
     * @brief readDataListString
     * @return reads a QList<QString> from the action data
     */
    QList<QString> readDataListString()
    {
        return readList<QString>();
    }
    /**
     * @brief writeDataListString
     * @param data writes a QList<QString> to the action data
     */
    void writeDataListString(QList<QString> data)
    {
        writeList(data);
    }
    /**
     * @brief writeDataString adds a string to the action data
     * @param data
     */
    void writeDataString(QString data)
    {
        m_buffer.seek(0);
        m_actionData << data;
    }
    /**
     * @brief readDataString
     * @return reads a string from the action data
     */
    QString readDataString()
    {
        m_buffer.seek(0);
        QString data;
        if (m_buffer.size() > 0)
        {
            m_actionData >> data;
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
    QBuffer m_buffer;
    QDataStream m_actionData{&m_buffer};
};

#endif // SCRIPTVARIABLE_H
