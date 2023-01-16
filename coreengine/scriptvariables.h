#ifndef SCRIPTVARIABLES_H
#define SCRIPTVARIABLES_H

#include <QObject>

#include <QVector>

#include "coreengine/scriptvariable.h"
#include "coreengine/fileserializable.h"

/**
 * @brief The ScriptVariables class holds variables for javascript that belong to a specific object
 * The variables are never used by the c++ engine
 */
class ScriptVariables final : public QObject, public FileSerializable
{
    Q_OBJECT
public:
    explicit ScriptVariables();
    ~ScriptVariables() = default;

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
public slots:
    /**
     * @brief createScriptVariable
     * @param id identifier for the variable
     * @return the created variables or the existing one if one was already created
     */
    ScriptVariable* createVariable(const QString & id);
    ScriptVariable* getVariable(const QString & id);
    /**
     * @brief clear deletes all variables hold by this object.
     */
    void clear();
private:
    QVector<spScriptVariable> m_Variables;
};

Q_DECLARE_INTERFACE(ScriptVariables, "ScriptVariables");

#endif // SCRIPTVARIABLES_H
