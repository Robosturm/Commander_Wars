#ifndef SCRIPTVARIABLE_H
#define SCRIPTVARIABLE_H

#include <QObject>
#include <QBuffer>
#include <QDataStream>
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
    virtual void serialize(QDataStream& pStream) override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserialize(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    inline virtual qint32 getVersion() override
    {
        return 1;
    }
public slots:
    inline QString getId()
    {
        return m_Id;
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
        actionData >> data;
        return data;
    }
    /**
     * @brief writeDataInt32 adds a int32 to the action data
     * @param data
     */
    void writeDataInt32(qint32 data)
    {
        buffer.seek(0);
        actionData << data;
    }
    /**
     * @brief readDataInt32
     * @return reads a int32 from the action data
     */
    qint32 readDataInt32()
    {
        buffer.seek(0);
        qint32 data;
        actionData >> data;
        return data;
    }
    /**
     * @brief writeDataFloat adds a float to the action data
     * @param data
     */
    void writeDataFloat(float data)
    {
        buffer.seek(0);
        actionData << data;
    }
    /**
     * @brief readDataFloat
     * @return reads a float from the action data
     */
    float readDataFloat()
    {
        buffer.seek(0);
        float data;
        actionData >> data;
        return data;
    }
    /**
     * @brief writeDataFloat adds a float to the action data
     * @param data
     */
    void writeDataBool(bool data)
    {
        buffer.seek(0);
        actionData << data;
    }
    /**
     * @brief readDataBool
     * @return reads a float from the action data
     */
    bool readDataBool()
    {
        buffer.seek(0);
        bool data;
        actionData >> data;
        return data;
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
