#ifndef SCRIPTEVENTGENERIC_H
#define SCRIPTEVENTGENERIC_H

#include <QVector>

#include "ingamescriptsupport/events/scriptevent.h"
#include "ingamescriptsupport/genericbox.h"

#include "objects/base/textbox.h"

class ScriptEventGeneric;
using spScriptEventGeneric = std::shared_ptr<ScriptEventGeneric>;

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
        File,
        IconSelection,
        PointList,
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
        QStringList items;
        QStringList data;
        // drop down with icon
        std::function<oxygine::spActor(QString item)> creator;
        // file selection
        QString filter;
        QString startFolder;

        QVariantList pointData;
        // generic reader writer cb
        std::function<QString(const Data &)> m_writer;
    };

    explicit ScriptEventGeneric(GameMap* pMap, EventType type, QString eventIdentifier, const QString & description);
    virtual ~ScriptEventGeneric() = default;

    /**
     * @brief readEvent
     * @param rStream
     */
    virtual void readEvent(QTextStream& rStream, QString line) override;
    /**
     * @brief removeCustomStart
     * @param text
     */
    virtual void removeCustomStart(QString &){}
    /**
     * @brief writeCustomStart
     */
    virtual void writeCustomStart(QTextStream&){}
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
        return m_description;
    }

signals:
    void sigShowSelectFile (QString filter, QString startFolder, QString currentFile, spTextbox pTextbox);
    void sigLoad(spGenericBox pBox);
protected slots:
    void showSelectFile (QString filter, QString startFolder, QString currentFile, spTextbox pTextbox);
private:
    void load(spGenericBox pBox);
    void addFloatEditor(spGenericBox & pBox, qint32 i, qint32 & y);
    void addIntegerEditor(spGenericBox & pBox, qint32 i, qint32 & y);
    void addStringEditor(spGenericBox & pBox, qint32 i, qint32 & y);
    void addBoolEditor(spGenericBox & pBox, qint32 i, qint32 & y);
    void addSelectionEditor(spGenericBox & pBox, qint32 i, qint32 & y);
    void addFileEditor(spGenericBox & pBox, qint32 i, qint32 & y);
    void addIconEditor(spGenericBox & pBox, qint32 i, qint32 & y);
    void addPointListEditor(spGenericBox pBox, qint32 i, qint32 & y);
protected:
    QVector<Data> m_Items;
    QString m_eventIdentifier;
    QString m_description;
};

#endif // SCRIPTEVENTGENERIC_H
