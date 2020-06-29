#ifndef SCRIPTEVENTGENERIC_H
#define SCRIPTEVENTGENERIC_H

#include <qvector.h>

#include "scriptevent.h"

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
        Selection
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
    };

    ScriptEventGeneric(EventType type, QString eventIdentifier);
    /**
     * @brief readEvent
     * @param rStream
     */
    virtual void readEvent(QTextStream& rStream) override;
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
    virtual qint32 getVersion() override
    {
        return 0;
    }
    virtual QString getDescription() override
    {
        return tr(m_eventIdentifier.toStdString().c_str());
    }
protected:
    QVector<Data> m_Items;
    QString m_eventIdentifier;
};

#endif // SCRIPTEVENTGENERIC_H
