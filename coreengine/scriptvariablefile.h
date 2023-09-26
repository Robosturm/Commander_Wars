#pragma once

#include <QObject>
#include "coreengine/scriptvariables.h"

class ScriptVariableFile;
using spScriptVariableFile = std::shared_ptr<ScriptVariableFile>;

class ScriptVariableFile final : public QObject, public FileSerializable
{
        Q_OBJECT
    public:
        explicit ScriptVariableFile(const QString & filename);
        virtual ~ScriptVariableFile() = default;
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
         * @brief getVersion stream version for serialization
         * @return
         */
        inline virtual qint32 getVersion() const override
        {
            return 1;
        }
        /**
         * @brief getFilename
         * @return
         */
        const QString &getFilename() const;

    public slots:
        /**
         * @brief writeFile
         */
        void writeFile();
        /**
         * @brief getVariables
         * @return
         */
        inline ScriptVariables* getVariables()
        {
            return &m_Variables;
        }
    private:
        ScriptVariables m_Variables;
        QString m_filename;
};

Q_DECLARE_INTERFACE(ScriptVariableFile, "ScriptVariableFile");
