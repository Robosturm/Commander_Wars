#ifndef SCRIPTEVENTGENERIC_H
#define SCRIPTEVENTGENERIC_H

#include <qvector.h>

#include "ingamescriptsupport/events/scriptevent.h"

#include "objects/base/textbox.h"

class ScriptEventGeneric;
using spScriptEventGeneric = oxygine::intrusive_ptr<ScriptEventGeneric>;

class ScriptEventGeneric : public ScriptEvent
{
    Q_OBJECT
public:
    enum EditTypes
    {
        Integer,
        Float,
        String,
        Bool,
        Selection,
        File
    };

    struct Data
    {
        QString beforeCode;
        QString item;
        QString afterCode;
        EditTypes types;
        QString tooltip;
        QString labelText;

        // spinbox
        qreal min{1};
        qreal max{9999};
        qreal infinite{0};
        // dropdown
        QVector<QString> items;
        QStringList data;
        // file selection
        QString filter;
        QString startFolder;
    };

    ScriptEventGeneric(EventType type, QString eventIdentifier);
    /**
     * @brief readEvent
     * @param rStream
     */
    virtual void readEvent(QTextStream& rStream, QString line) override;
    /**
     * @brief removeCustomStart
     * @param text
     */
    virtual void removeCustomStart(QString &){};
    /**
     * @brief writeCustomStart
     */
    virtual void writeCustomStart(QTextStream&){};
    /**
     * @brief writeEvent
     * @param rStream
     */
    virtual void writeEvent(QTextStream& rStream) override;
    /**
     * @brief showEditEvent
     */
    virtual void showEditEvent(spScriptEditor pScriptEditor) override;
    /**
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 0;
    }
    virtual QString getDescription() override
    {
        return tr(m_eventIdentifier.toStdString().c_str());
    }

signals:
    void sigShowSelectFile (QString filter, QString startFolder, QString currentFile, spTextbox pTextbox);
protected slots:
    void showSelectFile (QString filter, QString startFolder, QString currentFile, spTextbox pTextbox);
protected:
    QVector<Data> m_Items;
    QString m_eventIdentifier;
};

#endif // SCRIPTEVENTGENERIC_H
